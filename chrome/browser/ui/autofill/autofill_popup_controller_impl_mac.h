// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_POPUP_CONTROLLER_IMPL_MAC_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_POPUP_CONTROLLER_IMPL_MAC_H_

#include "chrome/browser/ui/autofill/autofill_popup_controller_impl.h"

@class WebTextfieldTouchBarController;

namespace autofill {

class AutofillPopupControllerImplMac : public AutofillPopupControllerImpl {
 public:
  AutofillPopupControllerImplMac(base::WeakPtr<AutofillPopupDelegate> delegate,
                                 content::WebContents* web_contents,
                                 gfx::NativeView container_view,
                                 const gfx::RectF& element_bounds,
                                 base::i18n::TextDirection text_direction);

  ~AutofillPopupControllerImplMac() override;

  // Shows the popup, or updates the existing popup with the given values.
  // If the popup contains credit card items, find and set
  // |touchBarController_| and show the credit card autofill touch bar.
  void Show(const std::vector<autofill::Suggestion>& suggestions) override;

  // Updates the data list values currently shown with the popup. Calls
  // -invalidateTouchBar from |touchBarController_|.
  void UpdateDataListValues(const std::vector<base::string16>& values,
                            const std::vector<base::string16>& labels) override;

  // Hides the popup and destroys the controller. This also invalidates
  // |delegate_|.
  void Hide() override;

 private:
  // The controller providing the autofill touch bar.
  WebTextfieldTouchBarController* touchBarController_;  // weak.

  DISALLOW_COPY_AND_ASSIGN(AutofillPopupControllerImplMac);
};

}  // namespace autofill

#endif  // CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_POPUP_CONTROLLER_IMPL_H_
