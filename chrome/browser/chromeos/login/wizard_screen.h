// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_SCREEN_H_

#include "gfx/canvas.h"

class WizardScreen;
namespace chromeos {
class ScreenObserver;
}  // namespace chromeos
namespace views {
class View;
}  // namespace views

// Interface that login wizard exposes to its screens.
class WizardScreenDelegate {
 public:
  // Returns top level view of the wizard.
  virtual views::View* GetWizardView() = 0;

  // Returns observer screen should notify.
  virtual chromeos::ScreenObserver* GetObserver(WizardScreen* screen) = 0;

 protected:
  virtual ~WizardScreenDelegate() {}
};

// Interface that defines login wizard screens.
// Also holds a reference to a delegate.
class WizardScreen {
 public:
  // Makes wizard screen visible.
  virtual void Show() = 0;
  // Makes wizard screen invisible.
  virtual void Hide() = 0;

 protected:
  explicit WizardScreen(WizardScreenDelegate* delegate): delegate_(delegate) {}
  virtual ~WizardScreen() {}

  WizardScreenDelegate* delegate() { return delegate_; }

 private:
  WizardScreenDelegate* delegate_;
};

#endif  // CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_SCREEN_H_

