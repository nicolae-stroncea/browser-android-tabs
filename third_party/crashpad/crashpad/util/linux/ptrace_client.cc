// Copyright 2017 The Crashpad Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "util/linux/ptrace_client.h"

#include <errno.h>

#include <string>

#include "base/logging.h"
#include "util/file/file_io.h"
#include "util/linux/ptrace_broker.h"

namespace crashpad {

namespace {

bool ReceiveAndLogError(int sock, const std::string& operation) {
  Errno error;
  if (!LoggingReadFileExactly(sock, &error, sizeof(error))) {
    return false;
  }
  errno = error;
  PLOG(ERROR) << operation;
  return true;
}

bool AttachImpl(int sock, pid_t tid) {
  PtraceBroker::Request request;
  request.type = PtraceBroker::Request::kTypeAttach;
  request.tid = tid;
  if (!LoggingWriteFile(sock, &request, sizeof(request))) {
    return false;
  }

  Bool success;
  if (!LoggingReadFileExactly(sock, &success, sizeof(success))) {
    return false;
  }

  if (success != kBoolTrue) {
    ReceiveAndLogError(sock, "PtraceBroker Attach");
    return false;
  }

  return true;
}

}  // namespace

PtraceClient::PtraceClient()
    : PtraceConnection(),
      sock_(kInvalidFileHandle),
      pid_(-1),
      is_64_bit_(false),
      initialized_() {}

PtraceClient::~PtraceClient() {
  if (sock_ != kInvalidFileHandle) {
    PtraceBroker::Request request;
    request.type = PtraceBroker::Request::kTypeExit;
    LoggingWriteFile(sock_, &request, sizeof(request));
  }
}

bool PtraceClient::Initialize(int sock, pid_t pid) {
  INITIALIZATION_STATE_SET_INITIALIZING(initialized_);
  sock_ = sock;
  pid_ = pid;

  if (!AttachImpl(sock_, pid_)) {
    return false;
  }

  PtraceBroker::Request request;
  request.type = PtraceBroker::Request::kTypeIs64Bit;
  request.tid = pid_;

  if (!LoggingWriteFile(sock_, &request, sizeof(request))) {
    return false;
  }

  Bool is_64_bit;
  if (!LoggingReadFileExactly(sock_, &is_64_bit, sizeof(is_64_bit))) {
    return false;
  }
  is_64_bit_ = is_64_bit == kBoolTrue;

  INITIALIZATION_STATE_SET_VALID(initialized_);
  return true;
}

bool PtraceClient::Read(VMAddress address, size_t size, void* buffer) {
  INITIALIZATION_STATE_DCHECK_VALID(initialized_);
  char* buffer_c = reinterpret_cast<char*>(buffer);

  PtraceBroker::Request request;
  request.type = PtraceBroker::Request::kTypeReadMemory;
  request.tid = pid_;
  request.iov.base = address;
  request.iov.size = size;

  if (!LoggingWriteFile(sock_, &request, sizeof(request))) {
    return false;
  }

  while (size > 0) {
    VMSize bytes_read;
    if (!LoggingReadFileExactly(sock_, &bytes_read, sizeof(bytes_read))) {
      return false;
    }

    if (!bytes_read) {
      ReceiveAndLogError(sock_, "PtraceBroker ReadMemory");
      return false;
    }

    if (!LoggingReadFileExactly(sock_, buffer_c, bytes_read)) {
      return false;
    }

    size -= bytes_read;
    buffer_c += bytes_read;
  }

  return true;
}

pid_t PtraceClient::GetProcessID() {
  INITIALIZATION_STATE_DCHECK_VALID(initialized_);
  return pid_;
}

bool PtraceClient::Attach(pid_t tid) {
  INITIALIZATION_STATE_DCHECK_VALID(initialized_);
  return AttachImpl(sock_, tid);
}

bool PtraceClient::Is64Bit() {
  INITIALIZATION_STATE_DCHECK_VALID(initialized_);
  return is_64_bit_;
}

bool PtraceClient::GetThreadInfo(pid_t tid, ThreadInfo* info) {
  INITIALIZATION_STATE_DCHECK_VALID(initialized_);

  PtraceBroker::Request request;
  request.type = PtraceBroker::Request::kTypeGetThreadInfo;
  request.tid = tid;
  if (!LoggingWriteFile(sock_, &request, sizeof(request))) {
    return false;
  }

  PtraceBroker::GetThreadInfoResponse response;
  if (!LoggingReadFileExactly(sock_, &response, sizeof(response))) {
    return false;
  }

  if (response.success == kBoolTrue) {
    *info = response.info;
    return true;
  }

  ReceiveAndLogError(sock_, "PtraceBroker GetThreadInfo");
  return false;
}

bool PtraceClient::ReadFileContents(const base::FilePath& path,
                                    std::string* contents) {
  INITIALIZATION_STATE_DCHECK_VALID(initialized_);

  PtraceBroker::Request request;
  request.type = PtraceBroker::Request::kTypeReadFile;
  request.path.path_length = path.value().size();

  if (!LoggingWriteFile(sock_, &request, sizeof(request)) ||
      !SendFilePath(path.value().c_str(), request.path.path_length)) {
    return false;
  }

  std::string local_contents;
  int32_t read_result;
  do {
    if (!LoggingReadFileExactly(sock_, &read_result, sizeof(read_result))) {
      return false;
    }

    if (read_result < 0) {
      ReceiveAndLogError(sock_, "ReadFileContents");
      return false;
    }

    if (read_result > 0) {
      size_t old_length = local_contents.size();
      local_contents.resize(old_length + read_result);
      if (!LoggingReadFileExactly(
              sock_, &local_contents[old_length], read_result)) {
        return false;
      }
    }
  } while (read_result > 0);

  contents->swap(local_contents);
  return true;
}

bool PtraceClient::SendFilePath(const char* path, size_t length) {
  if (!LoggingWriteFile(sock_, path, length)) {
    return false;
  }

  PtraceBroker::OpenResult result;
  if (!LoggingReadFileExactly(sock_, &result, sizeof(result))) {
    return false;
  }

  switch (result) {
    case PtraceBroker::kOpenResultAccessDenied:
      LOG(ERROR) << "Broker Open: access denied";
      return false;

    case PtraceBroker::kOpenResultTooLong:
      LOG(ERROR) << "Broker Open: path too long";
      return false;

    case PtraceBroker::kOpenResultSuccess:
      return true;

    default:
      if (result < 0) {
        LOG(ERROR) << "Broker Open: invalid result " << result;
        DCHECK(false);
      } else {
        errno = result;
        PLOG(ERROR) << "Broker Open";
      }
      return false;
  }
}

}  // namespace crashpad
