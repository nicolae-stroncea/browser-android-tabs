// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_SHAPEDETECTION_DETECTED_FACE_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_SHAPEDETECTION_DETECTED_FACE_H_

#include "third_party/blink/renderer/modules/modules_export.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"

namespace blink {

class DOMRect;
class Landmark;

class MODULES_EXPORT DetectedFace final : public ScriptWrappable {
  DEFINE_WRAPPERTYPEINFO();

 public:
  static DetectedFace* Create();
  static DetectedFace* Create(DOMRect*);
  static DetectedFace* Create(DOMRect*, const HeapVector<Landmark>&);

  DOMRect* boundingBox() const;
  const HeapVector<Landmark>& landmarks() const;

  void Trace(blink::Visitor*);

 private:
  explicit DetectedFace(DOMRect*);
  DetectedFace(DOMRect*, const HeapVector<Landmark>&);

  const Member<DOMRect> bounding_box_;
  const HeapVector<Landmark> landmarks_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_SHAPEDETECTION_DETECTED_FACE_H_
