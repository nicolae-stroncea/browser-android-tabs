// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

(async function() {
  TestRunner.addResult(`Test the set of visible records is correctly update when category filter changes\n`);
  await TestRunner.loadModule('performance_test_runner');
  await TestRunner.showPanel('timeline');

  var sessionId = '4.20';
  var mainThread = 1;
  var pid = 100;

  var testData = [
    {
      'args': {'data': {'sessionId': sessionId, 'frames': [
        {'frame': 'frame1', 'url': 'frameurl', 'name': 'frame-name'}
      ]}},
      'cat': 'disabled-by-default-devtools.timeline',
      'name': 'TracingStartedInPage',
      'ph': 'I',
      'pid': pid,
      'tid': mainThread,
      'ts': 100,
    },
    {'name': 'foooooo', 'ts': 1000000, 'ph': 'B', 'tid': mainThread, 'pid': pid, 'cat': 'toplevel', 'args': {}}, {
      'name': 'FunctionCall',
      'ts': 1000001,
      'ph': 'B',
      'tid': mainThread,
      'pid': pid,
      'cat': 'disabled-by-default-devtools.timeline',
      'args': {'data': {'frame': '0x2', 'scriptId': '3', 'scriptLine': 5, 'scriptName': 'http://example.com'}}
    },

    {
      'name': 'ResourceSendRequest',
      'ts': 1000002,
      'tid': mainThread,
      'pid': pid,
      'cat': 'disabled-by-default-devtools.timeline',
      'ph': 'I',
      's': 'g',
      'args':
          {'data': {'frame': '0x2', 'requestId': '44.1', 'requestMethod': 'GET', 'url': 'http://example.com/foo.js'}}
    },

    {
      'name': 'RecalculateStyles',
      'ts': 1000003,
      'ph': 'B',
      'tid': mainThread,
      'pid': pid,
      'cat': 'disabled-by-default-devtools.timeline',
      'args': {'beginData': {'frame': '0x2'}}
    },
    {
      'name': 'RecalculateStyles',
      'ts': 1009999,
      'ph': 'E',
      'tid': mainThread,
      'pid': pid,
      'cat': 'disabled-by-default-devtools.timeline',
      'args': {'elementCount': 3}
    },

    {
      'name': 'Layout',
      'ts': 1010000,
      'ph': 'B',
      'tid': mainThread,
      'pid': pid,
      'cat': 'disabled-by-default-devtools.timeline',
      'args': {'beginData': {'dirtyObjects': 1, 'frame': '0x2', 'partialLayout': false, 'totalObjects': 1}}
    },
    {
      'name': 'Layout',
      'ts': 1020999,
      'ph': 'E',
      'tid': mainThread,
      'pid': pid,
      'cat': 'disabled-by-default-devtools.timeline',
      'args': {'endData': {'root': [0, 0, 1570, 0, 1570, 243, 0, 243], 'rootNode': 1}}
    },

    {
      'name': 'FunctionCall',
      'ts': 1099999,
      'ph': 'E',
      'tid': mainThread,
      'pid': pid,
      'cat': 'disabled-by-default-devtools.timeline',
      'args': {}
    },
    {'name': 'foooooo', 'ts': 1099999, 'ph': 'E', 'tid': mainThread, 'pid': pid, 'cat': 'toplevel', 'args': {}}
  ];

  var model = PerformanceTestRunner.createPerformanceModelWithEvents(testData);
  var view = new Timeline.EventsTimelineTreeView(UI.panels.timeline._filters, null);
  view.setModel(model, PerformanceTestRunner.mainTrack());
  view.updateContents(Timeline.TimelineSelection.fromRange(
      model.timelineModel().minimumRecordTime(), model.timelineModel().maximumRecordTime()));
  var filtersControl = view._filtersControl;

  TestRunner.addResult('Original records');
  filtersControl._notifyFiltersChanged();
  dumpVisibleRecords();

  TestRunner.addResult('Visible records when \'loading\' is disabled');
  Timeline.TimelineUIUtils.categories().loading.hidden = true;
  filtersControl._notifyFiltersChanged();
  dumpVisibleRecords();

  TestRunner.addResult('Visible records when \'scripting\' is disabled');
  Timeline.TimelineUIUtils.categories().scripting.hidden = true;
  filtersControl._notifyFiltersChanged();
  dumpVisibleRecords();

  TestRunner.completeTest();

  function dumpVisibleRecords() {
    PerformanceTestRunner.walkTimelineEventTreeUnderNode(event => TestRunner.addResult(event.name), view._currentTree);
  }
})();
