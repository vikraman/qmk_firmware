// Copyright 2020 gtips
// SPDX-License-Identifier: GPL-2.0-or-later

static inline bool is_hrm(uint16_t keycode) {
    switch (keycode) {
        // QWERTY:
        case LC_S:
        case LA_D:
        case LG_F:
        case LS_V:
        case RS_M:
        case RG_J:
        case RA_K:
        case RC_L:

        // GRAPHITE-MOD:
        case LC_R:
        case LA_T:
        case LG_S:

        case RS_P:
        case RG_H:
        case RA_E:
        case RC_I:

        // NUM:
        case LC_COLN:
        case LA_DOT:
        case LG_COMM:
        case LS_HASH:

        // SYMBOL:
        case LC_LT:
        case LA_MINS:
        case LG_GT:
        case LS_COMM:

        // NAV:
        case RS_RBRC:
        case RG_RPRN:
        case RA_LPRN:
        case RC_TILD:

            return true;
    }
    return false;
}

// Combos
uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) {
    (void)combo_index;
    // Give 3-key combos a little extra time to register
    uint8_t key_count = 0;
    for (const uint16_t *key = combo->keys; *key != COMBO_END; key++) {
        key_count++;
    }
    return key_count >= 3 ? COMBO_TERM + 10 : COMBO_TERM;
}

bool get_combo_must_hold(uint16_t combo_index, combo_t *combo) {
    if (combo_index == CM_MOUSE) {
        return true;
    }
    return false;
}

/*
static bool is_home_row_mod(uint16_t keycode) {
    if (!IS_QK_MOD_TAP(keycode)) {
        return false;
    }

    switch (QK_MOD_TAP_GET_TAP_KEYCODE(keycode)) {
        case KC_A:
        case KC_S:
        case KC_D:
        case KC_F:
        case KC_J:
        case KC_K:
        case KC_L:
        case KC_SCLN:
        case KC_QUOT:
        case KC_N:
        case KC_R:
        case KC_T:
        case KC_E:
        case KC_I:
        case KC_H:
            return true;
        default:
            return false;
    }
}
 */
bool is_flow_tap_key(uint16_t keycode) {
    if ((get_mods() & (MOD_MASK_CG)) != 0) {
        return false;
    }
    switch (get_tap_keycode(keycode)) {
        case KC_SPC:
        case KC_A ... KC_Z:
        case KC_DOT:
        case KC_COMM:
        case KC_SCLN:
        case KC_SLSH:
        case REPEAT:
        case MAGIC:
            return true;
    }
    return false;
}

uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    if (get_tap_keycode(prev_keycode) == KC_BSPC) {
        return 0;
    }

    if (is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode)) {
        uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(keycode));

        if (mod & MOD_LSFT || mod & MOD_RSFT) {
            return 20;
        }
        if (mod & MOD_LCTL || mod & MOD_RCTL) {
            return 20;
        }
        if (mod & MOD_LALT || mod & MOD_RALT) {
            return 20;
        }
        if (mod & MOD_LGUI || mod & MOD_RGUI) {
            return 20;
        }
    }
    return 0;
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // special exceptions go here
        default:
            break;
    }

    // Disable key repeating on other home row mod keys
    if (is_hrm(keycode)) {
        return 0;
    }
    return QUICK_TAP_TERM;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // special exceptions go here
        default:
            break;
    }

    uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(keycode));

    if (mod & MOD_LSFT || mod & MOD_RSFT) {
        return 200;
    }
    if (mod & MOD_LCTL || mod & MOD_RCTL) {
        return 200;
    }
    if (mod & MOD_LALT || mod & MOD_RALT) {
        return 200;
    }
    if (mod & MOD_LGUI || mod & MOD_RGUI) {
        return 200;
    }

    return TAPPING_TERM;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LGUI_T(KC_D):
            return true;
        default:
            return false;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(keycode));

    return true;
}

bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode,
                      keyrecord_t *other_record) {
    if (is_hrm(tap_hold_keycode)) {
        if (other_record->event.key.col == 0 || other_record->event.key.col == 11) {
            return true;
        }
    } else {
        return get_chordal_hold_default(tap_hold_record, other_record);
    }

    switch (tap_hold_keycode) {
        case CS_LT1:
            return true;
            // special exceptions go here
        default:
            break;
    }

    return get_chordal_hold_default(tap_hold_record, other_record);
}

bool process_mouse_combo(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MOUSE_L:
            if (record->event.pressed) {
                layer_on(_MOUSE);
            } else if (!is_layer_locked(_MOUSE)) {
                layer_off(_MOUSE);
            }
            return false;
    }
    return true;
}

static bool alt_on = false;

bool process_alt_tab(uint16_t keycode, keyrecord_t *record) {
    if (keycode == ALT_TAB) {
        if (record->event.pressed) {
            if (!alt_on) {
                alt_on = true;
                register_mods(MOD_BIT(KC_LALT));
            }
            tap_code(KC_TAB);
        }
        return false;
    }
    if (keycode == CS_LT1) {
        if (!record->event.pressed) {
            if (alt_on) {
                alt_on = false;
                unregister_mods(MOD_BIT(KC_LALT));
            }
        }
    }
    return true;
}

bool process_home_row_mods(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RG_RPRN:
            if (record->event.pressed && record->tap.count) {
                tap_code16(KC_RPRN);
                return false;
            } else {
                return true;
            }
        case RA_LPRN:
            if (record->event.pressed && record->tap.count) {
                tap_code16(KC_LPRN);
                return false;
            } else {
                return true;
            }
        case LC_LT:
            if (record->event.pressed && record->tap.count) {
                tap_code16(KC_LT);
                return false;
            } else {
                return true;
            }
        case LG_GT:
            if (record->event.pressed && record->tap.count) {
                tap_code16(KC_GT);
                return false;
            } else {
                return true;
            }
            /* case LC_TILDE:
                if (record->event.pressed && record->tap.count) {
                    tap_code16(KC_TILDE);
                    return false;
                } else {
                    return true;
                } */
    }
    return true;
}

bool process_magic(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CS_LT1:
            if (record->event.pressed && record->tap.count) {
                dprintf("magic tap ignored");
                return false;
            } else if (record->event.pressed) {
                return true;
            }
            return true;
        case CS_RT1:
            if (record->event.pressed && record->tap.count) {
                dprintf("repeat tap ignored");
                return false;
            } else if (record->event.pressed) {
                return true;
            }
            return true;
    }
    return true;
}

#include "emacs.c"
#include "latex.c"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_mouse_combo(keycode, record)) return false;
    if (!process_alt_tab(keycode, record)) return false;
    if (!process_home_row_mods(keycode, record)) return false;
    if (!process_magic(keycode, record)) return false;
    if (!process_emacs(keycode, record)) return false;
    if (!process_latex(keycode, record)) return false;

    return true;
}
