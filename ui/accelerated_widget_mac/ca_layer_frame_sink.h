// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ACCELERATED_WIDGET_MAC_CA_LAYER_FRAME_SINK_H_
#define UI_ACCELERATED_WIDGET_MAC_CA_LAYER_FRAME_SINK_H_

#include "base/time/time.h"
#include "ui/accelerated_widget_mac/accelerated_widget_mac_export.h"
#include "ui/gfx/ca_layer_params.h"
#include "ui/gfx/native_widget_types.h"

namespace ui {

// An interface to an NSView that will embed content described by CALayerParams
// in its heirarchy.
class ACCELERATED_WIDGET_MAC_EXPORT CALayerFrameSink {
 public:
  // Translate from a gfx::AcceleratedWidget to the gfx::CALayerFrameSink
  // interface through which frames may be submitted. This may return nullptr.
  static CALayerFrameSink* FromAcceleratedWidget(gfx::AcceleratedWidget widget);

  // Update the embedder's CALayer tree to show the content described by
  // |ca_layer_params|.
  virtual void UpdateCALayerTree(const gfx::CALayerParams& ca_layer_params) = 0;

  // Retrieve the vsync parameters for the monitor on which the embedder is
  // currently displayed.
  virtual void GetVSyncParameters(base::TimeTicks* timebase,
                                  base::TimeDelta* interval) const = 0;

  // If suspended, the embedder will ignore updates until un-supended.
  virtual void SetSuspended(bool suspended) = 0;
};

}  // namespace ui

#endif  // UI_ACCELERATED_WIDGET_MAC_CA_LAYER_FRAME_SINK_H_
