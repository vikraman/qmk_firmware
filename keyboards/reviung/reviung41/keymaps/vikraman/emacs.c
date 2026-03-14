// Copyright 2020 gtips
// SPDX-License-Identifier: GPL-2.0-or-later

static bool process_emacs(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case EM_MX:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT("x"));
            }
            return false;
        case EM_MAGIT:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT("x") "magit-status" SS_TAP(X_ENT));
            }
            return false;
        case EM_PROJ:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT("x") "project-find-file" SS_TAP(X_ENT));
            }
            return false;
        case EM_FILE:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("x") SS_LCTL("f"));
            }
            return false;
        case EM_RG:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT("x") "consult-ripgrep" SS_TAP(X_ENT));
            }
            return false;
        case EM_AGEN:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT("x") "org-agenda" SS_TAP(X_ENT));
            }
            return false;
        case EM_CAPT:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT("x") "org-capture" SS_TAP(X_ENT));
            }
            return false;
        case EM_FIND:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("s"));
            }
            return false;
        case EM_IBUF:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("x") "b");
            }
            return false;
        case EM_BUF:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("x") SS_TAP(X_B));
            }
            return false;
        case EM_OWIN:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("x") "o");
            }
            return false;
        case EM_DWIN:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("x") "0");
            }
            return false;
        case EM_SWIN:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("x") "2");
            }
            return false;
        case EM_VWIN:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("x") "3");
            }
            return false;
        case EM_SAVE:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("x") SS_LCTL("s"));
            }
            return false;
        case EM_QUIT:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("x") SS_LCTL("c"));
            }
            return false;
        case EM_MARK:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL(" "));
            }
            return false;
        default:
            return true;
    }
}
