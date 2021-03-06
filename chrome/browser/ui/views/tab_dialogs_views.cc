// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/views/tab_dialogs_views.h"

#include "chrome/browser/ui/views/collected_cookies_views.h"
#include "chrome/browser/ui/views/hung_renderer_view.h"
#include "chrome/browser/ui/views/passwords/manage_passwords_bubble_view.h"
#include "chrome/browser/ui/views/sync/profile_signin_confirmation_dialog_views.h"
#include "content/public/browser/web_contents.h"

// static
void TabDialogs::CreateForWebContents(content::WebContents* contents) {
  DCHECK(contents);
  if (!FromWebContents(contents))
    contents->SetUserData(UserDataKey(), new TabDialogsViews(contents));
}

TabDialogsViews::TabDialogsViews(content::WebContents* contents)
    : web_contents_(contents) {
  DCHECK(contents);
}

TabDialogsViews::~TabDialogsViews() {
}

void TabDialogsViews::ShowCollectedCookies() {
  // Deletes itself on close.
  new CollectedCookiesViews(web_contents_);
}

void TabDialogsViews::ShowHungRendererDialog() {
  HungRendererDialogView::Show(web_contents_);
}

void TabDialogsViews::HideHungRendererDialog() {
  HungRendererDialogView::Hide(web_contents_);
}

void TabDialogsViews::ShowProfileSigninConfirmation(
    Browser* browser,
    Profile* profile,
    const std::string& username,
    ui::ProfileSigninConfirmationDelegate* delegate) {
  ProfileSigninConfirmationDialogViews::ShowDialog(
      browser, profile, username, delegate);
}

void TabDialogsViews::ShowManagePasswordsBubble(bool user_action) {
  if (ManagePasswordsBubbleView::IsShowing()) {
    // The bubble is currently shown for some other tab. We should close it now
    // and open for |web_contents_|.
    ManagePasswordsBubbleView::CloseBubble();
  }
  ManagePasswordsBubbleView::ShowBubble(
      web_contents_, user_action ? ManagePasswordsBubble::USER_ACTION
                                 : ManagePasswordsBubble::AUTOMATIC);
}

void TabDialogsViews::HideManagePasswordsBubble() {
  if (!ManagePasswordsBubbleView::IsShowing())
    return;
  content::WebContents* bubble_web_contents =
      ManagePasswordsBubbleView::manage_password_bubble()->web_contents();
  if (web_contents_ == bubble_web_contents)
    ManagePasswordsBubbleView::CloseBubble();
}
