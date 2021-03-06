// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "extensions/browser/browsertest_util.h"

#include "content/public/test/browser_test_utils.h"
#include "extensions/browser/extension_host.h"
#include "extensions/browser/process_manager.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace extensions {
namespace browsertest_util {

std::string ExecuteScriptInBackgroundPage(content::BrowserContext* context,
                                          const std::string& extension_id,
                                          const std::string& script) {
  ExtensionHost* host =
      ProcessManager::Get(context)->GetBackgroundHostForExtension(extension_id);
  if (!host) {
    ADD_FAILURE() << "Extension " << extension_id << " has no background page.";
    return "";
  }

  std::string result;
  if (!content::ExecuteScriptAndExtractString(host->host_contents(), script,
                                              &result)) {
    ADD_FAILURE() << "Executing script failed: " << script;
    result.clear();
  }
  return result;
}

bool ExecuteScriptInBackgroundPageNoWait(content::BrowserContext* context,
                                         const std::string& extension_id,
                                         const std::string& script) {
  ExtensionHost* host =
      ProcessManager::Get(context)->GetBackgroundHostForExtension(extension_id);
  if (!host) {
    ADD_FAILURE() << "Extension " << extension_id << " has no background page.";
    return false;
  }
  content::ExecuteScriptAsync(host->host_contents(), script);
  return true;
}

}  // namespace browsertest_util
}  // namespace extensions
