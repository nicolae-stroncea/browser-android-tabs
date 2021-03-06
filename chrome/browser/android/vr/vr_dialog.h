// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_VR_VR_DIALOG_H_
#define CHROME_BROWSER_ANDROID_VR_VR_DIALOG_H_

#include <jni.h>

#include <map>
#include <memory>

#include "base/android/jni_android.h"
#include "base/android/jni_weak_ref.h"
#include "base/callback.h"
#include "base/cancelable_callback.h"
#include "base/macros.h"
#include "chrome/browser/vr/content_input_delegate.h"
#include "ui/gfx/geometry/quaternion.h"
#include "ui/gfx/geometry/rect_f.h"
#include "ui/gfx/geometry/size_f.h"
#include "ui/gfx/native_widget_types.h"

using base::android::JavaParamRef;

namespace vr {
// TODO(bshe): refactor content_input_delegate and vr_dialog so the common
// code can be shared in a more clear way (https://crbug.com/838925).
class VrDialog : public ContentInputDelegate {
 public:
  VrDialog(int width, int height);
  ~VrDialog() override;

  void SetEventForwarder(ContentInputForwarder* dialog);
  void SetSize(int width, int height);

 private:
  ContentInputForwarder* dialog_ = nullptr;

  // ContentInputDelegate.
  void SendGestureToTarget(
      std::unique_ptr<blink::WebInputEvent> event) override;
  void UpdateGesture(const gfx::PointF& normalized_content_hit_point,
                     blink::WebGestureEvent& gesture) override;
  std::unique_ptr<blink::WebMouseEvent> MakeMouseEvent(
      blink::WebInputEvent::Type type,
      const gfx::PointF& normalized_web_content_location) override;

  int width_;
  int height_;

  DISALLOW_COPY_AND_ASSIGN(VrDialog);
};

}  //  namespace vr

#endif  // CHROME_BROWSER_ANDROID_VR_VR_DIALOG_H_
