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

enum anne_pro_layers {
    BASE,
    FN1,
    FN2,
};

// combos
#include "g/keymap_combo.h"

// tap dances
enum { _TD_TAB_ESC = 0, _TD_BSPC_DELW, _TD_SPC_ENT };

#define TAB_ESC TD(_TD_TAB_ESC)
#define BSPC_DELW TD(_TD_BSPC_DELW)
#define SPC_ENT TD(_TD_SPC_ENT)

tap_dance_action_t tap_dance_actions[] = {
    [_TD_TAB_ESC]   = ACTION_TAP_DANCE_DOUBLE(KC_TAB, KC_ESC),
    [_TD_BSPC_DELW] = ACTION_TAP_DANCE_DOUBLE(KC_BSPC, LALT(KC_BSPC)),
    [_TD_SPC_ENT]   = ACTION_TAP_DANCE_DOUBLE(KC_SPC, KC_ENT),
};

// custom keys
#define Q_CMD LT(0, KC_Q)
#define W_CMD LT(0, KC_W)
#define R_CMD LT(0, KC_R)
#define T_CMD LT(0, KC_T)
#define O_CMD LT(0, KC_O)
#define LBRC_CMD LT(0, KC_LBRC)
#define RBRC_CMD LT(0, KC_RBRC)
#define Z_CMD LT(0, KC_Z)
#define X_CMD LT(0, KC_X)
#define C_CMD LT(0, KC_C)
#define V_CMD LT(0, KC_V)
#define N_CMD LT(0, KC_N)

// Left-hand home row mods
#define CTL_A LCTL_T(KC_A)
#define ALT_S LALT_T(KC_S)
#define GUI_D LGUI_T(KC_D)
#define SFT_F LSFT_T(KC_F)

// Right-hand home row mods
#define SFT_J RSFT_T(KC_J)
#define GUI_K RGUI_T(KC_K)
#define ALT_L LALT_T(KC_L)
#define CTL_SCLN RCTL_T(KC_SCLN)

// clang-format off
// Key symbols are based on QMK. Use them to remap your keyboard
/*
* Layer BASE
* ,-----------------------------------------------------------------------------------------.
* | esc |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |    Bksp   |
* |-----------------------------------------------------------------------------------------+
* | Tab    |  q  |  w  |  e  |  r  |  t  |  y  |  u  |  i  |  o  |  p  |  [  |  ]  |   \    |
* |-----------------------------------------------------------------------------------------+
* | Ctrl    |  a  |  s  |  d  |  f  |  g  |  h  |  j  |  k  |  l  |  ;  |  '  |    Enter    |
* |-----------------------------------------------------------------------------------------+
* | Shift      |  z  |  x  |  c  |  v  |  b  |  n  |  m  |  ,  |  .  |  /  |    Shift       |
* |-----------------------------------------------------------------------------------------+
* | Ctrl  |  Alt   |  L1  |               space             |  L1   |  FN1  |  FN2  | Ctrl  |
* \-----------------------------------------------------------------------------------------/
* Layer TAP in BASE
* ,-----------------------------------------------------------------------------------------.
* |     |     |     |     |     |     |     |     |     |     |     |     |     |           |
* |-----------------------------------------------------------------------------------------+
* |        |     |     |     |     |     |     |     |     |     |     |     |     |        |
* |-----------------------------------------------------------------------------------------+
* |         |     |     |     |     |     |     |     |     |     |     |     |             |
* |-----------------------------------------------------------------------------------------+
* |            |     |     |     |     |     |     |     |     |     |     |       UP       |
* |-----------------------------------------------------------------------------------------+
* |       |       |       |                                 |       |  LEFT | DOWN  | RIGHT |
* \-----------------------------------------------------------------------------------------/
*/
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 [BASE] = LAYOUT_60_ansi( /* Base */
    QK_GESC,  KC_1,    KC_2,    KC_3,  KC_4,  KC_5,  KC_6,   KC_7,  KC_8,  KC_9,    KC_0,             KC_MINS,          KC_EQL,        BSPC_DELW,
    TAB_ESC,  Q_CMD,   W_CMD,   KC_E,  R_CMD, T_CMD, KC_Y,   KC_U,  KC_I,  O_CMD,   KC_P,             LBRC_CMD,         RBRC_CMD,      KC_BSLS,
    KC_LCTL,  CTL_A,   ALT_S,   GUI_D, SFT_F, KC_G,  KC_H,   SFT_J, GUI_K, ALT_L,   CTL_SCLN,         KC_QUOT,          KC_ENT,
    KC_LSFT,           Z_CMD,   X_CMD, C_CMD, V_CMD, KC_B,   N_CMD, KC_M,  KC_COMM, KC_DOT,           KC_SLSH,          RSFT_T(KC_UP),
    KC_LCTL,  KC_LALT, KC_LGUI,                   SPC_ENT,                 KC_RGUI, LT(FN1, KC_LEFT), LT(FN2, KC_DOWN), RCTL_T(KC_RGHT)
),
  /*
  * Layer FN1
  * ,-----------------------------------------------------------------------------------------.
  * |  `  |  F1 |  F2 |  F3 |  F4 |  F5 |  F6 |  F7 |  F8 |  F9 | F10 | F11 | F12 |  DELETE   |
  * |-----------------------------------------------------------------------------------------+
  * | Tab    |  q  | UP  |  e  |  r  |  t  |  y  |  u  |  i  |  o  | PS | HOME | END |   \    |
  * |-----------------------------------------------------------------------------------------+
  * | Esc     |LEFT |DOWN |RIGHT|  f  |  g  |  h  |  j  |  k  |  l  | PGUP|PGDN |    Enter    |
  * |-----------------------------------------------------------------------------------------+
  * | Shift      |V-UP |V-DWN|MUTE |  v  |  b  |  n  |  m  |  ,  |INSRT| DEL |    Shift       |
  * |-----------------------------------------------------------------------------------------+
  * | Ctrl  |  Alt   |  L1  |               space             |  L1   |  FN1  |  FN2  | Ctrl  |
  * \-----------------------------------------------------------------------------------------/
  *
  */
 [FN1] = LAYOUT_60_ansi( /* FN1 */
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
    _______, _______, KC_UP,   _______, _______, _______, _______, _______, _______, _______, KC_PSCR, KC_HOME, KC_END,  _______,
    _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, KC_PGUP, KC_PGDN, _______,
    _______,          KC_VOLU, KC_VOLD, KC_MUTE, _______, _______, _______, _______, _______, KC_INS,  KC_DEL,  _______,
    _______, _______, _______,                            _______,                   _______, _______, MO(FN2), _______
),
  /*
  * Layer FN2
  * ,-----------------------------------------------------------------------------------------.
  * |  ~  | BT1 | BT2 | BT3 | BT4 |  F5 |  F6 | F7 | F8 | MOD | TOG | BRI- | BRI+ |    Bksp   |
  * |-----------------------------------------------------------------------------------------+
  * | Tab    |  q  | UP  |  e  |  r  |  t  |  y  |  u  |  i  |  o  | PS | HOME | END |   \    |
  * |-----------------------------------------------------------------------------------------+
  * | Esc     |LEFT |DOWN |RIGHT|  f  |  g  |  h  |  j  |  k  |  l  | PGUP|PGDN |    Enter    |
  * |-----------------------------------------------------------------------------------------+
  * | Shift      |  z  |  x  |  c  |  v  |  b  |  n  |  m  |  ,  |INSRT| DEL |    Shift       |
  * |-----------------------------------------------------------------------------------------+
  * | Ctrl  |  Alt   |  L1  |               space             |  L1   |  FN1  |  FN2  | Ctrl  |
  * \-----------------------------------------------------------------------------------------/
  *
  */
 [FN2] = LAYOUT_60_ansi( /* FN2 */
    _______, KC_AP2_BT1, KC_AP2_BT2, KC_AP2_BT3, KC_AP2_BT4, _______, _______, _______, _______, KC_AP_RGB_MOD, KC_AP_RGB_TOG, KC_AP_RGB_VAD, KC_AP_RGB_VAI, _______,
    _______, RM_SPDU,    RM_SATU,    RM_HUEU,    RM_NEXT,    LUMINO,  EE_CLR,  AS_UP,   _______, _______,       KC_PSCR,       KC_HOME,       KC_END,        _______,
    _______, RM_SPDD,    RM_SATD,    RM_HUED,    RM_PREV,    _______, QK_RBT,  AS_DOWN, _______, _______,       KC_PGUP,       KC_PGDN,       _______,
    _______,             _______,    _______,    _______,    _______, _______, QK_BOOT, AS_TOGG, AS_RPT,        KC_INS,        KC_DEL,        _______,
    _______, _______,    _______,                                     _______,                   _______,       _______,       _______,       _______
 ),
};
// clang-format on

void keyboard_post_init_user(void) {
    autoshift_disable();

    ap2_led_set_manual_control(1);
    ap2_led_enable();

    rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_PALETTEFX_REACTIVE);
    uint8_t palette_index = PALETTEFX_VIRIDIS;
    rgb_matrix_sethsv_noeeprom(RGB_MATRIX_HUE_STEP * palette_index, 255, 255);
    rgb_matrix_set_speed_noeeprom(128);
    rgb_matrix_enable_noeeprom();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case FN1:
            ap2_led_t turquoise = {.p.red   = 0x77,
                                   .p.green = 0xBB,
                                   .p.blue  = 0x77,
                                   .p.alpha = 0xFF};
            ap2_led_blink(4, 10, turquoise, 5, 10);
            break;
        case FN2:
            ap2_led_t green = {.p.red   = 0x77,
                               .p.green = 0xFF,
                               .p.blue  = 0x00,
                               .p.alpha = 0xFF};
            ap2_led_blink(4, 11, green, 5, 10);
            break;
        default:
            ap2_led_reset_foreground_color();
            break;
    }
    return state;
}

bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    if (IS_RETRO(keycode)) return true;

    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
            return true;
        default:
            return false;
    }
}

bool send_cmd_on_hold(keyrecord_t *record, uint8_t keycode) {
    if (!record->tap.count && record->event.pressed) {
        tap_code16(G(keycode));
        return false;
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case Q_CMD:
            return send_cmd_on_hold(record, KC_Q);
        case W_CMD:
            return send_cmd_on_hold(record, KC_W);
        case R_CMD:
            return send_cmd_on_hold(record, KC_R);
        case T_CMD:
            return send_cmd_on_hold(record, KC_T);
        case O_CMD:
            return send_cmd_on_hold(record, KC_O);
        case LBRC_CMD:
            return send_cmd_on_hold(record, KC_LBRC);
        case RBRC_CMD:
            return send_cmd_on_hold(record, KC_RBRC);
        case Z_CMD:
            return send_cmd_on_hold(record, KC_Z);
        case X_CMD:
            return send_cmd_on_hold(record, KC_X);
        case C_CMD:
            return send_cmd_on_hold(record, KC_C);
        case V_CMD:
            return send_cmd_on_hold(record, KC_V);
        case N_CMD:
            return send_cmd_on_hold(record, KC_N);
    }
    return true;
}
