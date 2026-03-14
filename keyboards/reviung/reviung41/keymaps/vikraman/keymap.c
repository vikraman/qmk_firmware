/* Copyright 2020 gtips
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

#include <print.h>

#include "defs.h"

#define EMACS DF(_EMACS)
#define LATEX DF(_LATEX)
#define GREEK DF(_GREEK)

#define PREV G(KC_LBRC)
#define NEXT G(KC_RBRC)

enum {
    TD_TAB_ESC,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_TAB_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_TAB, KC_ESC),
};

#include "unicode.c"

// clang-format off

#define QWERTY DF(_QWERTY)
#define GRAPHITE DF(_GRAPHITE)

#define CS_SPC LT(_SYMBOL, KC_SPC)
#define CS_RT1 LT(_NUM,    REPEAT)
#define CS_LT1 LT(_NAV,    MAGIC)
#define CS_LT2 LT(_EMACS,  KC_BSLS)
#define CS_RT2 LT(_LATEX,  KC_TAB)

#define KC_POUN LALT(KC_3) // Mac+ANSI specific

// QWERTY
#define LC_S LCTL_T(KC_S)
#define LA_D LALT_T(KC_D)
#define LG_F LGUI_T(KC_F)
#define LS_V LSFT_T(KC_V)
#define RS_M RSFT_T(KC_M)
#define RG_J RGUI_T(KC_J)
#define RA_K LALT_T(KC_K)
#define RC_L RCTL_T(KC_L)

// GRAPHITE-MOD
#define LC_R LCTL_T(KC_R)
#define LA_T LALT_T(KC_T)
#define LG_S LGUI_T(KC_S)
#define LS_W LSFT_T(KC_W)
#define RS_P RSFT_T(KC_P)
#define RG_H RGUI_T(KC_H)
#define RA_E LALT_T(KC_E)
#define RC_I RCTL_T(KC_I)

// NUM
#define LC_COLN LCTL_T(KC_COLN)
#define LA_DOT  LALT_T(KC_DOT)
#define LG_COMM LGUI_T(KC_COMM)
#define LS_HASH LSFT_T(KC_HASH)

// SYMBOL
#define LC_LT   LCTL_T(KC_LT)
#define LA_MINS LALT_T(KC_MINS)
#define LG_GT   LGUI_T(KC_GT)
#define LS_COMM LSFT_T(KC_COMM)

// NAV
#define RS_RBRC RSFT_T(KC_RBRC)
#define RG_RPRN RGUI_T(KC_RPRN)
#define RA_LPRN LALT_T(KC_LPRN)
#define RC_TILD RCTL_T(KC_TILD)

#include "g/keymap_combo.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
        TD(TD_TAB_ESC),   KC_Q,   KC_W,   KC_E,  KC_R,   KC_T,            KC_Y,   KC_U,      KC_I,     KC_O,      KC_P,         KC_BSPC,
        LCTL_T(KC_ESC),   KC_A,   LC_S,   LA_D,  LG_F,   KC_G,            KC_H,   RG_J,      RA_K,     RC_L,   KC_SCLN, RCTL_T(KC_QUOT),
        KC_LSFT,          KC_Z,   KC_X,   KC_C,  LS_V,   KC_B,            KC_N,   RS_M,   KC_COMM,   KC_DOT,   KC_SLSH,  RSFT_T(KC_ENT),
                                            CS_LT2,   CS_LT1,   CS_SPC,   CS_RT1,   CS_RT2
    ),

    [_GRAPHITE] = LAYOUT(
        TD(TD_TAB_ESC),   KC_Z,   KC_L,   KC_D,   KC_C,   KC_B,           KC_J,   KC_F,      KC_O,     KC_U,   KC_SCLN,         KC_BSPC,
        LCTL_T(KC_ESC),   KC_N,   LC_R,   LA_T,   LG_S,   KC_G,           KC_Y,   RG_H,      RA_E,     RC_I,      KC_A, RCTL_T(KC_QUOT),
        KC_LSFT,          KC_Q,   KC_X,   KC_M,   LS_W,   KC_V,           KC_K,   RS_P,   KC_COMM,   KC_DOT,   KC_SLSH,  RSFT_T(KC_ENT),
                                            CS_LT2,   CS_LT1,   CS_SPC,   CS_RT1,   CS_RT2
    ),

    [_NUM] = LAYOUT(
        _______, KC_PERC, KC_MINS, KC_PLUS,  KC_DLR, KC_AMPR,                   KC_EQL,    KC_7,    KC_8,   KC_9,  KC_CIRC, _______,
        _______, KC_EXLM, LC_COLN,  LA_DOT, LG_COMM, KC_PIPE,                  KC_POUN,    KC_1,    KC_2,   KC_3,     KC_0,        ,
        _______, KC_CIRC, KC_SLSH, KC_ASTR, LS_HASH, KC_TILD,                    KC_AT,    KC_4,    KC_5,   KC_6,  KC_UNDS, _______,
                                            _______,   _______,   _______,   _______,   _______
    ),

    [_SYMBOL] = LAYOUT(
         KC_GRV,  KC_DLR, KC_EXLM, KC_DQUO, KC_AMPR, KC_HASH,                   KC_EQL, KC_RCBR, KC_LCBR, KC_CIRC, KC_SCLN, _______,
        _______, KC_PIPE,   LC_LT, LA_MINS,   LG_GT,  KC_EQL,                    KC_LT, RG_RPRN, RA_LPRN, RC_TILD, KC_UNDS, _______,
        _______, KC_CIRC, KC_COLN,  KC_DOT, LS_COMM, KC_TILD,                    KC_GT, RS_RBRC, KC_LBRC, KC_EXLM, KC_QUES, _______,
                                            _______,   _______,   _______,      KC_0,   _______
    ),

    [_NAV] = LAYOUT(
        ALT_TAB,   KC_PGUP,     PREV,   KC_UP,    NEXT, QK_LLCK,                  KC_EQL, KC_RCBR, KC_LCBR, KC_CIRC, KC_SCLN, _______,
        _______,   KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL,                   KC_LT, RG_RPRN, RA_LPRN, RC_TILD, KC_UNDS, _______,
        _______, C(KC_TAB),  KC_HOME, SELWORD,  KC_END,  KC_ENT,                   KC_GT, RS_RBRC, KC_LBRC, KC_EXLM, KC_QUES, _______,
                                                _______,   _______,  _______,  _______,  _______
    ),

    [_MOUSE] = LAYOUT(
        _______, _______, MS_BTN4,   MS_UP, MS_BTN5, QK_LLCK,                  _______, _______, _______, _______, _______, _______,
        _______, _______, MS_LEFT, MS_DOWN, MS_RGHT,  KC_DEL,                  _______, _______, _______, _______, _______, _______,
        _______, MS_WHLL, MS_WHLU, MS_WHLD, MS_WHLR,  KC_ENT,                  _______, _______, _______, _______, _______, _______,
                                                MS_BTN3,  MS_BTN2,  MS_BTN1,  MS_ACL2,  MS_ACL0
    ),

    [_EMACS] = LAYOUT(
        EMACS,   EM_MX,   EM_FILE, EM_FIND, EM_IBUF,  EM_BUF,              EM_MAGIT, EM_PROJ,  EM_RG,   EM_AGEN,  EM_CAPT, EM_SAVE,
        _______, C(KC_A), A(KC_B), C(KC_B), C(KC_F),  C(KC_E),               A(KC_F),  C(KC_P), C(KC_N), C(KC_K), C(KC_Y), EM_QUIT,
        _______, EM_MARK, C(KC_W), A(KC_W), C(KC_Y),  C(KC_D),              EM_OWIN,  EM_DWIN, EM_SWIN, EM_VWIN, C(KC_SLSH),  KC_ENT,
                                            _______,  _______,  _______,   _______,  _______
    ),

    [_LATEX] = LAYOUT(
        LATEX,   LT_BEG,  LT_END,  LT_EQN,  LT_ALN,  LT_CAS,               LT_FRAC,  LT_SQRT, LT_SUM,  LT_INT,  LT_SUB,  LT_SUP,
        LT_THM,  LT_LEM,  LT_COR,  LT_DEF,  LT_PRF, LT_TEXT,               LT_MATH, LT_DMATH, LT_BMAT, LT_SEC, LT_SSEC, LT_REF,
        LT_ITEM, LT_ENUM, LT_LBL, KC_LCBR, KC_RCBR, KC_PIPE,                 KC_LT,    KC_GT,  KC_EQL, KC_PLUS, KC_TILD, KC_ENT,
                                            _______,  _______,  _______,   _______,  _______
    ),

    [_GREEK] = LAYOUT(
        GREEK, UC_MAC, UM(GR_ALPHA), UM(GR_BETA), UM(GR_GAMMA), UM(GR_DELTA),  UM(GR_EPSILON),    UM(GR_ZETA),  UM(GR_ETA),      UM(GR_THETA), UM(GR_IOTA),    UM(GR_KAPPA),
        UM(GR_LAMBDA), UM(GR_MU), UM(GR_NU), UM(GR_XI), UM(GR_OMICRON), UM(GR_PI),    UM(GR_RHO),      UM(GR_SIGMA), UM(GR_TAU),      UM(GR_UPSILON), UM(GR_PHI),     UM(GR_CHI),
        UM(GR_PSI), UM(GR_OMEGA), UM(GR_ALPHA_CAP), UM(GR_GAMMA_CAP), UM(GR_DELTA_CAP), UM(GR_THETA_CAP), UM(GR_LAMBDA_CAP), UM(GR_XI_CAP), UM(GR_PI_CAP), UM(GR_SIGMA_CAP), UM(GR_PHI_CAP), UM(GR_PSI_CAP),
                                            UM(GR_OMEGA_CAP), UM(M_FORALL), UM(M_EXISTS), UM(M_IMPLIES), UM(M_IFF)
    ),

    [_ADJUST] = LAYOUT(
        UG_VALU,   UG_SATU, UG_HUEU,  UG_NEXT,  UG_SPDU,   UG_TOGG,            _______, _______,     EMACS,   LATEX,     GREEK,   QWERTY,
        UG_VALD,   UG_SATD, UG_HUED,  UG_PREV,  UG_SPDD,   VK_TOGG,            QK_RBT,   EE_CLR,   QK_LLCK,  QK_REP,   QK_AREP,  DB_TOGG,
        XXXXXXX,   XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX,            QK_BOOT,  UC_MAC,   XXXXXXX,  XXXXXXX,  SC_TOGG, GRAPHITE,
                                                                                        _______,   _______,  XXXXXXX,  _______, _______
    ),
};
// clang-format on

// Shift + Backspace → Forward Delete
const key_override_t shift_bksp_del = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

const key_override_t *const key_overrides[] = {
    &shift_bksp_del,
};

#include "callbacks.c"
#include "rgb.c"
