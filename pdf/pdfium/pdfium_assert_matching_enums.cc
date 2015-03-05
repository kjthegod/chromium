// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "ppapi/c/pp_input_event.h"
#include "ppapi/c/private/ppb_pdf.h"
#include "ppapi/c/private/ppp_pdf.h"
#include "third_party/pdfium/fpdfsdk/include/fpdf_fwlevent.h"
#include "third_party/pdfium/fpdfsdk/include/fpdf_sysfontinfo.h"
#include "ui/events/keycodes/keyboard_codes.h"

#define STATIC_ASSERT_MATCH(np_name, pdfium_name) \
    static_assert(int(np_name) == int(pdfium_name), \
    "mismatching enums: " #np_name)

STATIC_ASSERT_MATCH(PP_INPUTEVENT_MODIFIER_SHIFTKEY, FWL_EVENTFLAG_ShiftKey);
STATIC_ASSERT_MATCH(PP_INPUTEVENT_MODIFIER_CONTROLKEY,
                    FWL_EVENTFLAG_ControlKey);
STATIC_ASSERT_MATCH(PP_INPUTEVENT_MODIFIER_ALTKEY, FWL_EVENTFLAG_AltKey);
STATIC_ASSERT_MATCH(PP_INPUTEVENT_MODIFIER_METAKEY, FWL_EVENTFLAG_MetaKey);
STATIC_ASSERT_MATCH(PP_INPUTEVENT_MODIFIER_ISKEYPAD, FWL_EVENTFLAG_KeyPad);
STATIC_ASSERT_MATCH(PP_INPUTEVENT_MODIFIER_ISAUTOREPEAT,
                    FWL_EVENTFLAG_AutoRepeat);
STATIC_ASSERT_MATCH(PP_INPUTEVENT_MODIFIER_LEFTBUTTONDOWN,
                    FWL_EVENTFLAG_LeftButtonDown);
STATIC_ASSERT_MATCH(PP_INPUTEVENT_MODIFIER_MIDDLEBUTTONDOWN,
                    FWL_EVENTFLAG_MiddleButtonDown);
STATIC_ASSERT_MATCH(PP_INPUTEVENT_MODIFIER_RIGHTBUTTONDOWN,
                    FWL_EVENTFLAG_RightButtonDown);

STATIC_ASSERT_MATCH(ui::VKEY_BACK, FWL_VKEY_Back);
STATIC_ASSERT_MATCH(ui::VKEY_TAB, FWL_VKEY_Tab);
STATIC_ASSERT_MATCH(ui::VKEY_CLEAR, FWL_VKEY_Clear);
STATIC_ASSERT_MATCH(ui::VKEY_RETURN, FWL_VKEY_Return);
STATIC_ASSERT_MATCH(ui::VKEY_SHIFT, FWL_VKEY_Shift);
STATIC_ASSERT_MATCH(ui::VKEY_CONTROL, FWL_VKEY_Control);
STATIC_ASSERT_MATCH(ui::VKEY_MENU, FWL_VKEY_Menu);
STATIC_ASSERT_MATCH(ui::VKEY_PAUSE, FWL_VKEY_Pause);
STATIC_ASSERT_MATCH(ui::VKEY_CAPITAL, FWL_VKEY_Capital);
STATIC_ASSERT_MATCH(ui::VKEY_KANA, FWL_VKEY_Kana);
STATIC_ASSERT_MATCH(ui::VKEY_HANGUL, FWL_VKEY_Hangul);
STATIC_ASSERT_MATCH(ui::VKEY_JUNJA, FWL_VKEY_Junja);
STATIC_ASSERT_MATCH(ui::VKEY_FINAL, FWL_VKEY_Final);
STATIC_ASSERT_MATCH(ui::VKEY_HANJA, FWL_VKEY_Hanja);
STATIC_ASSERT_MATCH(ui::VKEY_KANJI, FWL_VKEY_Kanji);
STATIC_ASSERT_MATCH(ui::VKEY_ESCAPE, FWL_VKEY_Escape);
STATIC_ASSERT_MATCH(ui::VKEY_CONVERT, FWL_VKEY_Convert);
STATIC_ASSERT_MATCH(ui::VKEY_NONCONVERT, FWL_VKEY_NonConvert);
STATIC_ASSERT_MATCH(ui::VKEY_ACCEPT, FWL_VKEY_Accept);
STATIC_ASSERT_MATCH(ui::VKEY_MODECHANGE, FWL_VKEY_ModeChange);
STATIC_ASSERT_MATCH(ui::VKEY_SPACE, FWL_VKEY_Space);
STATIC_ASSERT_MATCH(ui::VKEY_PRIOR, FWL_VKEY_Prior);
STATIC_ASSERT_MATCH(ui::VKEY_NEXT, FWL_VKEY_Next);
STATIC_ASSERT_MATCH(ui::VKEY_END, FWL_VKEY_End);
STATIC_ASSERT_MATCH(ui::VKEY_HOME, FWL_VKEY_Home);
STATIC_ASSERT_MATCH(ui::VKEY_LEFT, FWL_VKEY_Left);
STATIC_ASSERT_MATCH(ui::VKEY_UP, FWL_VKEY_Up);
STATIC_ASSERT_MATCH(ui::VKEY_RIGHT, FWL_VKEY_Right);
STATIC_ASSERT_MATCH(ui::VKEY_DOWN, FWL_VKEY_Down);
STATIC_ASSERT_MATCH(ui::VKEY_SELECT, FWL_VKEY_Select);
STATIC_ASSERT_MATCH(ui::VKEY_PRINT, FWL_VKEY_Print);
STATIC_ASSERT_MATCH(ui::VKEY_EXECUTE, FWL_VKEY_Execute);
STATIC_ASSERT_MATCH(ui::VKEY_SNAPSHOT, FWL_VKEY_Snapshot);
STATIC_ASSERT_MATCH(ui::VKEY_INSERT, FWL_VKEY_Insert);
STATIC_ASSERT_MATCH(ui::VKEY_DELETE, FWL_VKEY_Delete);
STATIC_ASSERT_MATCH(ui::VKEY_HELP, FWL_VKEY_Help);
STATIC_ASSERT_MATCH(ui::VKEY_0, FWL_VKEY_0);
STATIC_ASSERT_MATCH(ui::VKEY_1, FWL_VKEY_1);
STATIC_ASSERT_MATCH(ui::VKEY_2, FWL_VKEY_2);
STATIC_ASSERT_MATCH(ui::VKEY_3, FWL_VKEY_3);
STATIC_ASSERT_MATCH(ui::VKEY_4, FWL_VKEY_4);
STATIC_ASSERT_MATCH(ui::VKEY_5, FWL_VKEY_5);
STATIC_ASSERT_MATCH(ui::VKEY_6, FWL_VKEY_6);
STATIC_ASSERT_MATCH(ui::VKEY_7, FWL_VKEY_7);
STATIC_ASSERT_MATCH(ui::VKEY_8, FWL_VKEY_8);
STATIC_ASSERT_MATCH(ui::VKEY_9, FWL_VKEY_9);
STATIC_ASSERT_MATCH(ui::VKEY_A, FWL_VKEY_A);
STATIC_ASSERT_MATCH(ui::VKEY_B, FWL_VKEY_B);
STATIC_ASSERT_MATCH(ui::VKEY_C, FWL_VKEY_C);
STATIC_ASSERT_MATCH(ui::VKEY_D, FWL_VKEY_D);
STATIC_ASSERT_MATCH(ui::VKEY_E, FWL_VKEY_E);
STATIC_ASSERT_MATCH(ui::VKEY_F, FWL_VKEY_F);
STATIC_ASSERT_MATCH(ui::VKEY_G, FWL_VKEY_G);
STATIC_ASSERT_MATCH(ui::VKEY_H, FWL_VKEY_H);
STATIC_ASSERT_MATCH(ui::VKEY_I, FWL_VKEY_I);
STATIC_ASSERT_MATCH(ui::VKEY_J, FWL_VKEY_J);
STATIC_ASSERT_MATCH(ui::VKEY_K, FWL_VKEY_K);
STATIC_ASSERT_MATCH(ui::VKEY_L, FWL_VKEY_L);
STATIC_ASSERT_MATCH(ui::VKEY_M, FWL_VKEY_M);
STATIC_ASSERT_MATCH(ui::VKEY_N, FWL_VKEY_N);
STATIC_ASSERT_MATCH(ui::VKEY_O, FWL_VKEY_O);
STATIC_ASSERT_MATCH(ui::VKEY_P, FWL_VKEY_P);
STATIC_ASSERT_MATCH(ui::VKEY_Q, FWL_VKEY_Q);
STATIC_ASSERT_MATCH(ui::VKEY_R, FWL_VKEY_R);
STATIC_ASSERT_MATCH(ui::VKEY_S, FWL_VKEY_S);
STATIC_ASSERT_MATCH(ui::VKEY_T, FWL_VKEY_T);
STATIC_ASSERT_MATCH(ui::VKEY_U, FWL_VKEY_U);
STATIC_ASSERT_MATCH(ui::VKEY_V, FWL_VKEY_V);
STATIC_ASSERT_MATCH(ui::VKEY_W, FWL_VKEY_W);
STATIC_ASSERT_MATCH(ui::VKEY_X, FWL_VKEY_X);
STATIC_ASSERT_MATCH(ui::VKEY_Y, FWL_VKEY_Y);
STATIC_ASSERT_MATCH(ui::VKEY_Z, FWL_VKEY_Z);
STATIC_ASSERT_MATCH(ui::VKEY_LWIN, FWL_VKEY_LWin);
STATIC_ASSERT_MATCH(ui::VKEY_COMMAND, FWL_VKEY_Command);
STATIC_ASSERT_MATCH(ui::VKEY_RWIN, FWL_VKEY_RWin);
STATIC_ASSERT_MATCH(ui::VKEY_APPS, FWL_VKEY_Apps);
STATIC_ASSERT_MATCH(ui::VKEY_SLEEP, FWL_VKEY_Sleep);
STATIC_ASSERT_MATCH(ui::VKEY_NUMPAD0, FWL_VKEY_NumPad0);
STATIC_ASSERT_MATCH(ui::VKEY_NUMPAD1, FWL_VKEY_NumPad1);
STATIC_ASSERT_MATCH(ui::VKEY_NUMPAD2, FWL_VKEY_NumPad2);
STATIC_ASSERT_MATCH(ui::VKEY_NUMPAD3, FWL_VKEY_NumPad3);
STATIC_ASSERT_MATCH(ui::VKEY_NUMPAD4, FWL_VKEY_NumPad4);
STATIC_ASSERT_MATCH(ui::VKEY_NUMPAD5, FWL_VKEY_NumPad5);
STATIC_ASSERT_MATCH(ui::VKEY_NUMPAD6, FWL_VKEY_NumPad6);
STATIC_ASSERT_MATCH(ui::VKEY_NUMPAD7, FWL_VKEY_NumPad7);
STATIC_ASSERT_MATCH(ui::VKEY_NUMPAD8, FWL_VKEY_NumPad8);
STATIC_ASSERT_MATCH(ui::VKEY_NUMPAD9, FWL_VKEY_NumPad9);
STATIC_ASSERT_MATCH(ui::VKEY_MULTIPLY, FWL_VKEY_Multiply);
STATIC_ASSERT_MATCH(ui::VKEY_ADD, FWL_VKEY_Add);
STATIC_ASSERT_MATCH(ui::VKEY_SEPARATOR, FWL_VKEY_Separator);
STATIC_ASSERT_MATCH(ui::VKEY_SUBTRACT, FWL_VKEY_Subtract);
STATIC_ASSERT_MATCH(ui::VKEY_DECIMAL, FWL_VKEY_Decimal);
STATIC_ASSERT_MATCH(ui::VKEY_DIVIDE, FWL_VKEY_Divide);
STATIC_ASSERT_MATCH(ui::VKEY_F1, FWL_VKEY_F1);
STATIC_ASSERT_MATCH(ui::VKEY_F2, FWL_VKEY_F2);
STATIC_ASSERT_MATCH(ui::VKEY_F3, FWL_VKEY_F3);
STATIC_ASSERT_MATCH(ui::VKEY_F4, FWL_VKEY_F4);
STATIC_ASSERT_MATCH(ui::VKEY_F5, FWL_VKEY_F5);
STATIC_ASSERT_MATCH(ui::VKEY_F6, FWL_VKEY_F6);
STATIC_ASSERT_MATCH(ui::VKEY_F7, FWL_VKEY_F7);
STATIC_ASSERT_MATCH(ui::VKEY_F8, FWL_VKEY_F8);
STATIC_ASSERT_MATCH(ui::VKEY_F9, FWL_VKEY_F9);
STATIC_ASSERT_MATCH(ui::VKEY_F10, FWL_VKEY_F10);
STATIC_ASSERT_MATCH(ui::VKEY_F11, FWL_VKEY_F11);
STATIC_ASSERT_MATCH(ui::VKEY_F12, FWL_VKEY_F12);
STATIC_ASSERT_MATCH(ui::VKEY_F13, FWL_VKEY_F13);
STATIC_ASSERT_MATCH(ui::VKEY_F14, FWL_VKEY_F14);
STATIC_ASSERT_MATCH(ui::VKEY_F15, FWL_VKEY_F15);
STATIC_ASSERT_MATCH(ui::VKEY_F16, FWL_VKEY_F16);
STATIC_ASSERT_MATCH(ui::VKEY_F17, FWL_VKEY_F17);
STATIC_ASSERT_MATCH(ui::VKEY_F18, FWL_VKEY_F18);
STATIC_ASSERT_MATCH(ui::VKEY_F19, FWL_VKEY_F19);
STATIC_ASSERT_MATCH(ui::VKEY_F20, FWL_VKEY_F20);
STATIC_ASSERT_MATCH(ui::VKEY_F21, FWL_VKEY_F21);
STATIC_ASSERT_MATCH(ui::VKEY_F22, FWL_VKEY_F22);
STATIC_ASSERT_MATCH(ui::VKEY_F23, FWL_VKEY_F23);
STATIC_ASSERT_MATCH(ui::VKEY_F24, FWL_VKEY_F24);
STATIC_ASSERT_MATCH(ui::VKEY_NUMLOCK, FWL_VKEY_NunLock);
STATIC_ASSERT_MATCH(ui::VKEY_SCROLL, FWL_VKEY_Scroll);
STATIC_ASSERT_MATCH(ui::VKEY_LSHIFT, FWL_VKEY_LShift);
STATIC_ASSERT_MATCH(ui::VKEY_RSHIFT, FWL_VKEY_RShift);
STATIC_ASSERT_MATCH(ui::VKEY_LCONTROL, FWL_VKEY_LControl);
STATIC_ASSERT_MATCH(ui::VKEY_RCONTROL, FWL_VKEY_RControl);
STATIC_ASSERT_MATCH(ui::VKEY_LMENU, FWL_VKEY_LMenu);
STATIC_ASSERT_MATCH(ui::VKEY_RMENU, FWL_VKEY_RMenu);
STATIC_ASSERT_MATCH(ui::VKEY_BROWSER_BACK, FWL_VKEY_BROWSER_Back);
STATIC_ASSERT_MATCH(ui::VKEY_BROWSER_FORWARD, FWL_VKEY_BROWSER_Forward);
STATIC_ASSERT_MATCH(ui::VKEY_BROWSER_REFRESH, FWL_VKEY_BROWSER_Refresh);
STATIC_ASSERT_MATCH(ui::VKEY_BROWSER_STOP, FWL_VKEY_BROWSER_Stop);
STATIC_ASSERT_MATCH(ui::VKEY_BROWSER_SEARCH, FWL_VKEY_BROWSER_Search);
STATIC_ASSERT_MATCH(ui::VKEY_BROWSER_FAVORITES, FWL_VKEY_BROWSER_Favorites);
STATIC_ASSERT_MATCH(ui::VKEY_BROWSER_HOME, FWL_VKEY_BROWSER_Home);
STATIC_ASSERT_MATCH(ui::VKEY_VOLUME_MUTE, FWL_VKEY_VOLUME_Mute);
STATIC_ASSERT_MATCH(ui::VKEY_VOLUME_DOWN, FWL_VKEY_VOLUME_Down);
STATIC_ASSERT_MATCH(ui::VKEY_VOLUME_UP, FWL_VKEY_VOLUME_Up);
STATIC_ASSERT_MATCH(ui::VKEY_MEDIA_NEXT_TRACK, FWL_VKEY_MEDIA_NEXT_Track);
STATIC_ASSERT_MATCH(ui::VKEY_MEDIA_PREV_TRACK, FWL_VKEY_MEDIA_PREV_Track);
STATIC_ASSERT_MATCH(ui::VKEY_MEDIA_STOP, FWL_VKEY_MEDIA_Stop);
STATIC_ASSERT_MATCH(ui::VKEY_MEDIA_PLAY_PAUSE, FWL_VKEY_MEDIA_PLAY_Pause);
STATIC_ASSERT_MATCH(ui::VKEY_MEDIA_LAUNCH_MAIL, FWL_VKEY_MEDIA_LAUNCH_Mail);
STATIC_ASSERT_MATCH(ui::VKEY_MEDIA_LAUNCH_MEDIA_SELECT,
                    FWL_VKEY_MEDIA_LAUNCH_MEDIA_Select);
STATIC_ASSERT_MATCH(ui::VKEY_MEDIA_LAUNCH_APP1, FWL_VKEY_MEDIA_LAUNCH_APP1);
STATIC_ASSERT_MATCH(ui::VKEY_MEDIA_LAUNCH_APP2, FWL_VKEY_MEDIA_LAUNCH_APP2);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_1, FWL_VKEY_OEM_1);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_PLUS, FWL_VKEY_OEM_Plus);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_COMMA, FWL_VKEY_OEM_Comma);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_MINUS, FWL_VKEY_OEM_Minus);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_PERIOD, FWL_VKEY_OEM_Period);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_2, FWL_VKEY_OEM_2);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_3, FWL_VKEY_OEM_3);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_4, FWL_VKEY_OEM_4);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_5, FWL_VKEY_OEM_5);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_6, FWL_VKEY_OEM_6);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_7, FWL_VKEY_OEM_7);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_8, FWL_VKEY_OEM_8);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_102, FWL_VKEY_OEM_102);
STATIC_ASSERT_MATCH(ui::VKEY_PROCESSKEY, FWL_VKEY_ProcessKey);
STATIC_ASSERT_MATCH(ui::VKEY_PACKET, FWL_VKEY_Packet);
STATIC_ASSERT_MATCH(ui::VKEY_ATTN, FWL_VKEY_Attn);
STATIC_ASSERT_MATCH(ui::VKEY_CRSEL, FWL_VKEY_Crsel);
STATIC_ASSERT_MATCH(ui::VKEY_EXSEL, FWL_VKEY_Exsel);
STATIC_ASSERT_MATCH(ui::VKEY_EREOF, FWL_VKEY_Ereof);
STATIC_ASSERT_MATCH(ui::VKEY_PLAY, FWL_VKEY_Play);
STATIC_ASSERT_MATCH(ui::VKEY_ZOOM, FWL_VKEY_Zoom);
STATIC_ASSERT_MATCH(ui::VKEY_NONAME, FWL_VKEY_NoName);
STATIC_ASSERT_MATCH(ui::VKEY_PA1, FWL_VKEY_PA1);
STATIC_ASSERT_MATCH(ui::VKEY_OEM_CLEAR, FWL_VKEY_OEM_Clear);
STATIC_ASSERT_MATCH(ui::VKEY_UNKNOWN, FWL_VKEY_Unknown);

STATIC_ASSERT_MATCH(PP_PRIVATEFONTCHARSET_ANSI, FXFONT_ANSI_CHARSET);
STATIC_ASSERT_MATCH(PP_PRIVATEFONTCHARSET_DEFAULT, FXFONT_DEFAULT_CHARSET);
STATIC_ASSERT_MATCH(PP_PRIVATEFONTCHARSET_SYMBOL, FXFONT_SYMBOL_CHARSET);
STATIC_ASSERT_MATCH(PP_PRIVATEFONTCHARSET_SHIFTJIS, FXFONT_SHIFTJIS_CHARSET);
STATIC_ASSERT_MATCH(PP_PRIVATEFONTCHARSET_HANGUL, FXFONT_HANGEUL_CHARSET);
STATIC_ASSERT_MATCH(PP_PRIVATEFONTCHARSET_GB2312, FXFONT_GB2312_CHARSET);
STATIC_ASSERT_MATCH(PP_PRIVATEFONTCHARSET_CHINESEBIG5,
                    FXFONT_CHINESEBIG5_CHARSET);

STATIC_ASSERT_MATCH(PP_PRIVATEDUPLEXMODE_NONE, DuplexUndefined);
STATIC_ASSERT_MATCH(PP_PRIVATEDUPLEXMODE_SIMPLEX, Simplex);
STATIC_ASSERT_MATCH(PP_PRIVATEDUPLEXMODE_SHORT_EDGE, DuplexFlipShortEdge);
STATIC_ASSERT_MATCH(PP_PRIVATEDUPLEXMODE_LONG_EDGE, DuplexFlipLongEdge);
