/* Copyright 2021 OpenAnnePro community
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "magic.h"

enum anne_pro_layers {
    BASE,
    GRAPHITE,
    SYM,
    NAV,
    MOUSE, // tri-layer: SYM + NAV
    FN2,
};

enum custom_keycodes {
    ST_MACRO_0 = SAFE_RANGE,
};

enum tap_dance_codes {
    TD_BSPC,
    TD_ESC_GRV,
    TD_SELWORD,
};

// QWERTY
#define LC_S LCTL_T(KC_S)
#define LA_D LALT_T(KC_D)
#define LG_F LGUI_T(KC_F)
#define LS_V LSFT_T(KC_V)
#define RS_M RSFT_T(KC_M)
#define RG_J RGUI_T(KC_J)
#define RA_K LALT_T(KC_K)
#define RC_L RCTL_T(KC_L)
#define ME_G MEH_T(KC_G)
#define ME_H MEH_T(KC_H)
#define HY_B HYPR_T(KC_B)
#define HY_N HYPR_T(KC_N)

// GRAPHITE-MOD
#define LC_R LCTL_T(KC_R)
#define LA_T LALT_T(KC_T)
#define LG_S LGUI_T(KC_S)
#define LS_C LSFT_T(KC_C)
#define RS_P RSFT_T(KC_P)
#define RG_H RGUI_T(KC_H)
#define RA_E LALT_T(KC_E)
#define RC_I RCTL_T(KC_I)
#define HY_V HYPR_T(KC_V)

// thumb-ish
#define ME_TAB MEH_T(KC_TAB)
#define HY_SPC HYPR_T(KC_SPC)

// clang-format off
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
* BASE
* ,-----------------------------------------------------------------------------------------.
* |esc/`|  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |   bspc    |
* |-----------------------------------------------------------------------------------------+
* | tab/meh|  q  |  w  |  e  |  r  |  t  |  y  |  u  |  i  |  o  |  p  |  [  |  ]  |   \    |
* |-----------------------------------------------------------------------------------------+
* | osm ctl |  a  |  s  |  d  |  f  |  g  |  h  |  j  |  k  |  l  |  ;  |  '  |    enter    |
* |-----------------------------------------------------------------------------------------+
* | osm sft    |  z  |  x  |  c  |  v  |  b  |  n  |  m  |  ,  |  .  |  /  |    sft/up      |
* |-----------------------------------------------------------------------------------------+
* |leader |  alt   |  gui  |          space / hyper          | magic | NAV/< | FN2/v | SYM/> |
* \-----------------------------------------------------------------------------------------/
*/
 [BASE] = LAYOUT_60_ansi(
    TD(TD_ESC_GRV), KC_1,    KC_2,    KC_3,  KC_4,  KC_5,  KC_6,  KC_7,  KC_8,    KC_9,    KC_0,     KC_MINS,          KC_EQL,           TD(TD_BSPC),
    ME_TAB,         KC_Q,    KC_W,    KC_E,  KC_R,  KC_T,  KC_Y,  KC_U,  KC_I,    KC_O,    KC_P,     KC_LBRC,          KC_RBRC,          KC_BSLS,
    OSM(MOD_LCTL),  KC_A,    LC_S,    LA_D,  LG_F,  ME_G,  ME_H,  RG_J,  RA_K,    RC_L,    KC_SCLN,  KC_QUOT,          KC_ENT,
    OSM(MOD_LSFT),           KC_Z,    KC_X,  KC_C,  LS_V,  HY_B,  HY_N,  RS_M,    KC_COMM, KC_DOT,   KC_SLSH,          RSFT_T(KC_UP),
    LEADER,  KC_LALT, KC_LGUI,                      HY_SPC,               MAGIC,   LT(NAV,KC_LEFT),  LT(FN2,KC_DOWN),  LT(SYM,KC_RGHT)
),
/*
* GRAPHITE  alphas only
* ,-----------------------------------------------------------------------------------------.
* |     |     |     |     |     |     |     |     |     |     |     |     |     |           |
* |-----------------------------------------------------------------------------------------+
* |        |  q  |  l  |  d  |  w  |  b  |  j  |  f  |  o  |  u  |  ;  |     |     |        |
* |-----------------------------------------------------------------------------------------+
* |         |  n  |  r  |  t  |  s  |  g  |  y  |  h  |  e  |  i  |  a  |     |             |
* |-----------------------------------------------------------------------------------------+
* |            |  z  |  x  |  m  |  c  |  v  |  k  |  p  |     |     |     |                |
* |-----------------------------------------------------------------------------------------+
* |       |       |       |                                 |       |       |       |       |
* \-----------------------------------------------------------------------------------------/
*/
 [GRAPHITE] = LAYOUT_60_ansi(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, KC_Q,    KC_L,    KC_D,    KC_W,    KC_B,    KC_J,    KC_F,    KC_O,    KC_U,    KC_SCLN, _______, _______, _______,
    _______, KC_N,    LC_R,    LA_T,    LG_S,    ME_G,    KC_Y,    RG_H,    RA_E,    RC_I,    KC_A,    _______, _______,
    _______,          KC_Z,    KC_X,    KC_M,    LS_C,    HY_V,    KC_K,    RS_P,    _______, _______, _______, _______,
    _______, _______, _______,                   _______,                   _______, _______, _______, _______
),
/*
* SYM  two-handed symbol layer
* ,-----------------------------------------------------------------------------------------.
* |  `  |  F1 |  F2 |  F3 |  F4 |  F5 |  F6 |  F7 |  F8 |  F9 | F10 | F11 | F12 |    del    |
* |-----------------------------------------------------------------------------------------+
* |        |  %  |  -  |  +  |  $  |  &  |  =  |  7  |  8  |  9  |  }  |  {  |  _  |   ?    |
* |-----------------------------------------------------------------------------------------+
* | lock    |  !  |  :  |  .  |  ,  | \|  | A-3 |  1  |  2  |  3  |  )  |  (  |             |
* |-----------------------------------------------------------------------------------------+
* |            |  ^  |  /  |  *  |  #  |  ~  |  @  |  4  |  5  |  6  |  ]  |       [        |
* |-----------------------------------------------------------------------------------------+
* |       |       |       |                0                |       |       |       |       |
* \-----------------------------------------------------------------------------------------/
*/
 [SYM] = LAYOUT_60_ansi(
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
    _______, KC_PERC, KC_MINS, KC_PLUS, KC_DLR,  KC_AMPR,    KC_EQL,  KC_7,    KC_8,    KC_9,    KC_RCBR, KC_LCBR, KC_UNDS, KC_QUES,
    QK_LLCK, KC_EXLM, KC_COLN, KC_DOT,  KC_COMM, KC_PIPE,    LALT(KC_3), KC_1, KC_2,    KC_3,    KC_RPRN, KC_LPRN, _______,
    _______,          KC_CIRC, KC_SLSH, KC_ASTR, KC_HASH,    KC_TILD, KC_AT,   KC_4,    KC_5,    KC_6,    KC_RBRC, KC_LBRC,
    _______, _______, _______,                   KC_0,                         _______, _______, _______, _______
),
/*
* NAV  left hand navigation layer
* ,-----------------------------------------------------------------------------------------.
* | G-` |     |     |     |     |     |     |     |     |     |     |     |     |    del    |
* |-----------------------------------------------------------------------------------------+
* | G-tab  | G-[ | up  | G-] |arep |pgup |     |     |     |     |     |     |     |        |
* |-----------------------------------------------------------------------------------------+
* | lock    |left |down |rght | rep |pgdn |     |     |     |     |     |     |             |
* |-----------------------------------------------------------------------------------------+
* |            |selw |home |macro| end |C-alt|     |     |     |     |     |                |
* |-----------------------------------------------------------------------------------------+
* |       |       |       |                                 |       |       |       |       |
* \-----------------------------------------------------------------------------------------/
*/
 [NAV] = LAYOUT_60_ansi(
    LGUI(KC_GRV), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,
    LGUI(KC_TAB), LGUI(KC_LBRC), KC_UP, LGUI(KC_RBRC), QK_AREP, KC_PGUP, _______, _______, _______, _______, _______, _______, _______, _______,
    QK_LLCK,      KC_LEFT, KC_DOWN, KC_RGHT, QK_REP,  KC_PGDN, _______, _______, _______, _______, _______, _______, _______,
    _______,               TD(TD_SELWORD), KC_HOME, ST_MACRO_0, KC_END, LCTL(KC_LALT), _______, _______, _______, _______, _______, _______,
    _______, _______, _______,                _______,                   _______, _______, _______, _______
),
/*
* MOUSE  left hand orbital mouse layer
* ,-----------------------------------------------------------------------------------------.
* |     |sel1 |sel2 |sel3 |     |     |     |     |     |     |     |     |     |           |
* |-----------------------------------------------------------------------------------------+
* |        |whl< | up  |whl> |dbls |whl+ |     |     |     |     |     |     |     |        |
* |-----------------------------------------------------------------------------------------+
* | lock    |left |down |rght |btns |whl- |     |     |     |     |     |     |             |
* |-----------------------------------------------------------------------------------------+
* |            |rels |hlds |fast |slow |     |     |     |     |     |     |                |
* |-----------------------------------------------------------------------------------------+
* |       |       |       |                                 |       |       |       |       |
* \-----------------------------------------------------------------------------------------/
*/
 [MOUSE] = LAYOUT_60_ansi(
    _______, OM_SEL1, OM_SEL2, OM_SEL3, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, OM_W_L,  OM_U,    OM_W_R,  OM_DBLS, OM_W_U,  _______, _______, _______, _______, _______, _______, _______, _______,
    QK_LLCK, OM_L,    OM_D,    OM_R,    OM_BTNS, OM_W_D,  _______, _______, _______, _______, _______, _______, _______,
    _______,          OM_RELS, OM_HLDS, OM_FAST, OM_SLOW, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______,                   _______,                   _______, _______, _______, _______
 ),
/*
* FN2  anne pro 2 hardware function layer
* ,-----------------------------------------------------------------------------------------.
* |boot | bt1 | bt2 | bt3 | bt4 | usb |unpar|     |     |     |     |     |     |           |
* |-----------------------------------------------------------------------------------------+
* |        |spd+ |sat+ |hue+ |next |lumi |     |     |     |     |prtsc|home | end |        |
* |-----------------------------------------------------------------------------------------+
* |         |spd- |sat- |hue- |prev | tog |     |     |     |     |pgup |pgdn |             |
* |-----------------------------------------------------------------------------------------+
* |            |actg |sentc|eeclr|rboot|     |     |     |     | ins | del |                |
* |-----------------------------------------------------------------------------------------+
* |graph  |       |       |                                 |       |       |       |       |
* \-----------------------------------------------------------------------------------------/
*/
 [FN2] = LAYOUT_60_ansi(
    QK_BOOT, KC_AP2_BT1, KC_AP2_BT2, KC_AP2_BT3, KC_AP2_BT4, KC_AP2_USB, KC_AP2_BT_UNPAIR, _______, _______, _______, _______, _______, _______, _______,
    _______, RM_SPDU,    RM_SATU,    RM_HUEU,    RM_NEXT,    LUMINO,     _______, _______, _______, _______, KC_PSCR,  KC_HOME, KC_END,  _______,
    QK_LLCK, RM_SPDD,    RM_SATD,    RM_HUED,    RM_PREV,    RM_TOGG,    _______, _______, _______, _______, KC_PGUP,  KC_PGDN, _______,
    _______,             AC_TOGG,    SENTENCE_CASE_TOGGLE,   EE_CLR,     QK_RBT,  _______, _______, _______, _______, KC_INS, KC_DEL, _______,
    TG(GRAPHITE), _______, _______,                                        _______,          _______, _______, _______, _______
 ),
};
// clang-format on

// chordal hold exceptions
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    // same mod both hands is a roll
    if (IS_QK_MOD_TAP(tap_hold_keycode) && IS_QK_MOD_TAP(other_keycode)) {
        // mask off the left/right flag
        uint8_t mods_a = QK_MOD_TAP_GET_MODS(tap_hold_keycode) & 0x0F;
        uint8_t mods_b = QK_MOD_TAP_GET_MODS(other_keycode) & 0x0F;
        if (mods_a == mods_b) {
            return false;
        }
    }
    // layer taps always hold; the tri-layer is two keys on one hand
    if (IS_QK_LAYER_TAP(tap_hold_keycode) || IS_QK_LAYER_TAP_TOGGLE(tap_hold_keycode)) return true;
    return get_chordal_hold_default(tap_hold_record, other_record);
}

// mods held means hotkey, not chord; bypass tap-hold so hrms type their letter
#define FORCED_TAPS_MAX 4
static struct {
    uint8_t  row, col;
    uint16_t kc;
} forced_taps[FORCED_TAPS_MAX];

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!IS_QK_MOD_TAP(keycode)) {
        return true; // layer taps still hold
    }

    if (record->event.pressed) {
        if (((get_mods() | get_oneshot_mods()) & MOD_MASK_CSAG) == 0) {
            return true;
        }
        uint16_t tap_kc = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        for (uint8_t i = 0; i < FORCED_TAPS_MAX; i++) {
            if (forced_taps[i].kc == KC_NO) {
                forced_taps[i].row = record->event.key.row;
                forced_taps[i].col = record->event.key.col;
                forced_taps[i].kc  = tap_kc;
                register_code16(tap_kc);
                return false;
            }
        }
        return true; // table full, normal tap-hold
    }

    for (uint8_t i = 0; i < FORCED_TAPS_MAX; i++) {
        if (forced_taps[i].kc != KC_NO && forced_taps[i].row == record->event.key.row && forced_taps[i].col == record->event.key.col) {
            unregister_code16(forced_taps[i].kc);
            forced_taps[i].kc = KC_NO;
            return false;
        }
    }
    return true;
}

// qmk's flow tap guard only sees real mods; count one-shot mods too
bool is_flow_tap_key(uint16_t keycode) {
    if (((get_mods() | get_oneshot_mods()) & (MOD_MASK_CG | MOD_BIT_LALT)) != 0) {
        return false;
    }
    switch (get_tap_keycode(keycode)) {
        case KC_SPC:
        case KC_A ... KC_Z:
        case KC_DOT:
        case KC_COMM:
        case KC_SCLN:
        case KC_SLSH:
            return true;
    }
    return false;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(TD_BSPC):
            return 120; // shorter dance window, backspace is high frequency
        case HY_SPC:
            return 150; // thumb, strongest
        case ME_TAB:
            return 200; // pinky
        case LC_S:
        case RC_L:
            return 200; // ring
        default:
            return TAPPING_TERM;
    }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    if (keycode == HY_SPC) return 150; // double space
    if (IS_QK_MOD_TAP(keycode)) return 100;
    return 0;
}

typedef struct {
    bool    is_press_action;
    uint8_t step;
} tap;

enum { SINGLE_TAP = 1, SINGLE_HOLD, DOUBLE_TAP, DOUBLE_HOLD, DOUBLE_SINGLE_TAP, MORE_TAPS };

static tap td_state[3];

static uint8_t td_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}

static void on_td_bspc(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_BSPC);
        tap_code16(KC_BSPC);
        tap_code16(KC_BSPC);
    }
    if (state->count > 3) {
        tap_code16(KC_BSPC);
    }
}

static void td_bspc_finished(tap_dance_state_t *state, void *user_data) {
    td_state[TD_BSPC].step = td_step(state);
    switch (td_state[TD_BSPC].step) {
        case SINGLE_TAP:
        case SINGLE_HOLD:
            register_code16(KC_BSPC);
            break;
        case DOUBLE_TAP:
            register_code16(LALT(KC_BSPC));
            break;
        case DOUBLE_SINGLE_TAP:
            tap_code16(KC_BSPC);
            register_code16(KC_BSPC);
    }
}

static void td_bspc_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (td_state[TD_BSPC].step) {
        case SINGLE_TAP:
        case SINGLE_HOLD:
            unregister_code16(KC_BSPC);
            break;
        case DOUBLE_TAP:
            unregister_code16(LALT(KC_BSPC));
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code16(KC_BSPC);
            break;
    }
    td_state[TD_BSPC].step = 0;
}

static void td_esc_grv_finished(tap_dance_state_t *state, void *user_data) {
    td_state[TD_ESC_GRV].step = td_step(state);
    switch (td_state[TD_ESC_GRV].step) {
        case SINGLE_TAP:
            tap_code16(KC_ESC);
            break;
        case SINGLE_HOLD:
            tap_code16(KC_GRV);
            break;
    }
}

static void td_esc_grv_reset(tap_dance_state_t *state, void *user_data) {
    td_state[TD_ESC_GRV].step = 0;
}

// tap_code16 skips process_record, module keycodes need this
static void tap_module_keycode(uint16_t keycode) {
    keyrecord_t record = {
        .event =
            {
                .key     = {.row = 254, .col = 254},
                .time    = (uint16_t)(timer_read() | 1),
                .type    = KEY_EVENT,
                .pressed = true,
            },
        .keycode = keycode,
    };
    process_record(&record);
    record.event.pressed = false;
    record.event.time    = (uint16_t)(timer_read() | 1);
    process_record(&record);
}

static void td_selword_finished(tap_dance_state_t *state, void *user_data) {
    td_state[TD_SELWORD].step = td_step(state);
    switch (td_state[TD_SELWORD].step) {
        case SINGLE_TAP:
            tap_module_keycode(SELWORD); // forward word
            break;
        case SINGLE_HOLD:
            tap_module_keycode(SELLUP); // line up
            break;
        case DOUBLE_TAP:
            tap_module_keycode(SELWBAK); // backward word
            break;
    }
}

static void td_selword_reset(tap_dance_state_t *state, void *user_data) {
    td_state[TD_SELWORD].step = 0;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_BSPC]    = ACTION_TAP_DANCE_FN_ADVANCED(on_td_bspc, td_bspc_finished, td_bspc_reset),
    [TD_ESC_GRV] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_esc_grv_finished, td_esc_grv_reset),
    [TD_SELWORD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_selword_finished, td_selword_reset),
};

void keyboard_post_init_user(void) {
    ap2_led_set_manual_control(1);
    ap2_led_enable();

    rgb_matrix_mode_noeeprom(RGB_MATRIX_COMMUNITY_MODULE_PALETTEFX_REACTIVE);
    uint8_t palette_index = PALETTEFX_VIRIDIS;
    rgb_matrix_sethsv_noeeprom(RGB_MATRIX_HUE_STEP * palette_index, 255, 255);
    rgb_matrix_set_speed_noeeprom(128);
    rgb_matrix_enable_noeeprom();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // tri-layer would clear a locked MOUSE
    if (!is_layer_locked(MOUSE)) {
        state = update_tri_layer_state(state, SYM, NAV, MOUSE);
    }
    return state;
}

// per-layer fallback colour
static const uint8_t layer_rgb[][3] = {
    [GRAPHITE] = {0xFF, 0x00, 0x88}, // magenta
    [SYM]      = {0xFF, 0xAA, 0x00}, // amber
    [NAV]      = {0x00, 0xCC, 0x99}, // turquoise
    [MOUSE]    = {0x00, 0xAA, 0xFF}, // cyan
    [FN2]      = {0x66, 0xFF, 0x00}, // green
};

// keyed off keycode, not position
static bool key_group_rgb(uint16_t kc, uint8_t rgb[3]) {
#define GROUP(cr, cg, cb) \
    do {                  \
        rgb[0] = cr;      \
        rgb[1] = cg;      \
        rgb[2] = cb;      \
        return true;      \
    } while (0)

    switch (kc) {
        // cursor and pointer motion
        case KC_UP:
        case KC_DOWN:
        case KC_LEFT:
        case KC_RGHT:
        case OM_U:
        case OM_D:
        case OM_L:
        case OM_R:
            GROUP(0x00, 0x88, 0xFF); // blue

        // paging and scrolling
        case KC_PGUP:
        case KC_PGDN:
        case OM_W_U:
        case OM_W_D:
        case OM_W_L:
        case OM_W_R:
            GROUP(0x00, 0xFF, 0xCC); // teal

        // jump and edit
        case KC_HOME:
        case KC_END:
        case KC_INS:
        case KC_DEL:
        case ST_MACRO_0:
        case TD(TD_SELWORD):
            GROUP(0xFF, 0x99, 0x00); // amber

        // clicks
        case OM_BTNS:
        case OM_DBLS:
        case OM_HLDS:
        case OM_RELS:
            GROUP(0xFF, 0x00, 0x66); // magenta
        case OM_SEL1:
        case OM_SEL2:
        case OM_SEL3:
            GROUP(0x88, 0x00, 0x44); // dim magenta

        // pointer speed
        case OM_FAST:
        case OM_SLOW:
            GROUP(0x88, 0x44, 0xFF); // violet

        // repeat
        case QK_REP:
        case QK_AREP:
            GROUP(0xFF, 0x55, 0x00); // orange

        // window management
        case LGUI(KC_GRV):
        case LGUI(KC_TAB):
        case LGUI(KC_LBRC):
        case LGUI(KC_RBRC):
        case LCTL(KC_LALT):
            GROUP(0xAA, 0x00, 0xFF); // purple

        // digits and f-keys
        case KC_1 ... KC_0:
            GROUP(0x00, 0xFF, 0x66); // green
        case KC_F1 ... KC_F12:
            GROUP(0x44, 0x66, 0xFF); // dim blue

        // bracket pairs
        case KC_LPRN:
        case KC_RPRN:
        case KC_LCBR:
        case KC_RCBR:
        case KC_LBRC:
        case KC_RBRC:
            GROUP(0xFF, 0xFF, 0x00); // yellow

        // graphite switch
        case TG(GRAPHITE):
            GROUP(0xFF, 0x00, 0x88); // magenta

        // leader
        case LEADER:
            GROUP(0xFF, 0x00, 0xCC); // pink

        // bluetooth and usb
        case KC_AP2_BT1:
        case KC_AP2_BT2:
        case KC_AP2_BT3:
        case KC_AP2_BT4:
        case KC_AP2_USB:
        case KC_AP2_BT_UNPAIR:
            GROUP(0x00, 0x66, 0xFF); // blue

        // rgb controls
        case RM_TOGG:
        case RM_NEXT:
        case RM_PREV:
        case RM_HUEU:
        case RM_HUED:
        case RM_SATU:
        case RM_SATD:
        case RM_SPDU:
        case RM_SPDD:
        case LUMINO:
            GROUP(0xCC, 0xCC, 0xCC); // white

        // typing toggles
        case AC_TOGG:
        case SENTENCE_CASE_TOGGLE:
            GROUP(0xFF, 0xCC, 0x00); // gold

        // destructive
        case QK_BOOT:
        case QK_RBT:
        case EE_CLR:
            GROUP(0xFF, 0x00, 0x00); // red
    }
    return false;
#undef GROUP
}

// light every key the layer defines, read from the keymap
bool rgb_matrix_indicators_user(void) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer == BASE) {
        return false; // base shows the effect
    }

    bool locked = is_layer_locked(layer);

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led = g_led_config.matrix_co[row][col];
            if (led == NO_LED) {
                continue;
            }
            uint16_t kc = keymap_key_to_keycode(layer, (keypos_t){.row = row, .col = col});
            if (kc <= KC_TRANSPARENT) {
                continue; // transparent, leave the effect
            }
            // white locked, grey held
            if (kc == QK_LLCK) {
                if (locked) {
                    rgb_matrix_set_color(led, RGB_WHITE);
                } else {
                    rgb_matrix_set_color(led, 0x55, 0x55, 0x55);
                }
                continue;
            }
            uint8_t rgb[3];
            if (!key_group_rgb(kc, rgb)) {
                rgb[0] = layer_rgb[layer][0];
                rgb[1] = layer_rgb[layer][1];
                rgb[2] = layer_rgb[layer][2];
            }
            rgb_matrix_set_color(led, rgb[0], rgb[1], rgb[2]);
        }
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    magic_update_buffer(keycode, record);

    switch (keycode) {
        case ST_MACRO_0:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL(SS_TAP(X_RIGHT)) SS_DELAY(100) SS_LCTL(SS_TAP(X_LEFT)) SS_DELAY(100) SS_LCTL(SS_LSFT(SS_TAP(X_RIGHT))));
            }
            break;

        case MAGIC:
            magic_expand(record);
            break;
    }
    return true;
}
