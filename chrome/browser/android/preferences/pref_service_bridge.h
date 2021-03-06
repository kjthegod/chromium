// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_PREFERENCES_PREF_SERVICE_BRIDGE_H_
#define CHROME_BROWSER_ANDROID_PREFERENCES_PREF_SERVICE_BRIDGE_H_

#include <jni.h>
#include <string>

bool RegisterPrefServiceBridge(JNIEnv* env);

// Use |locale| to create a language-region pair and language code to prepend to
// the default accept languages. For Malay, we'll end up creating
// "ms-MY,ms,en-US,en", and for Swiss-German, we will have
// "de-CH,de-DE,de,en-US,en".
void PrependToAcceptLanguagesIfNecessary(std::string locale,
                                         std::string* accept_languages);

#endif  // CHROME_BROWSER_ANDROID_PREFERENCES_PREF_SERVICE_BRIDGE_H_
