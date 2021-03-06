#!/usr/bin/env python
# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

'''Item formatters for RC headers.
'''

from grit.node import message


def Format(root, lang='en', output_dir='.'):
  yield '''\
// This file is automatically generated by GRIT. Do not edit.

#pragma once
'''
  # Check for emit nodes under the rc_header. If any emit node
  # is present, we assume it means the GRD file wants to override
  # the default header, with no includes.
  default_includes = ['#include <atlres.h>', '']
  emit_lines = []
  for output_node in root.GetOutputFiles():
    if output_node.GetType() == 'rc_header':
      for child in output_node.children:
        if child.name == 'emit' and child.attrs['emit_type'] == 'prepend':
          emit_lines.append(child.GetCdata())
  for line in emit_lines or default_includes:
    yield line + '\n'

  for line in FormatDefines(root, root.GetRcHeaderFormat()):
    yield line


def FormatDefines(root, rc_header_format=None):
  '''Yields #define SYMBOL 1234 lines.

  Args:
    root: A GritNode.
  '''
  tids = root.GetIdMap()

  if not rc_header_format:
    rc_header_format = "#define {textual_id} {numeric_id}"
  rc_header_format += "\n"
  for item in root.ActiveDescendants():
    with item:
      for tid in item.GetTextualIds():
        yield rc_header_format.format(textual_id=tid,numeric_id=tids[tid])
