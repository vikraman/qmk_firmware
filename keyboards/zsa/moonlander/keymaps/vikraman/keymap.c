#include QMK_KEYBOARD_H
#include "version.h"
#include "magic.h"
#define MOON_LED_LEVEL LED_LEVEL
#ifndef ZSA_SAFE_RANGE
#    define ZSA_SAFE_RANGE SAFE_RANGE
#endif

enum custom_keycodes {
    RGB_SLD = ZSA_SAFE_RANGE,
    ST_MACRO_0,
};

enum tap_dance_codes {
    TD_GRAPHITE,
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
#define HY_B HYPR_T(KC_B)
#define HY_N HYPR_T(KC_N)

// GRAPHITE-MOD
#define LC_R LCTL_T(KC_R)
#define LA_T LALT_T(KC_T)
#define LG_S LGUI_T(KC_S)
#define LS_W LSFT_T(KC_W)
#define RS_P RSFT_T(KC_P)
#define RG_H RGUI_T(KC_H)
#define RA_E LALT_T(KC_E)
#define RC_I RCTL_T(KC_I)
#define ME_G MEH_T(KC_G)
#define ME_H MEH_T(KC_H)

#define GU_SPC MT(MOD_LGUI, KC_SPACE)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_moonlander(
    TD(TD_ESC_GRV), KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           KC_AUDIO_VOL_DOWN,                              KC_AUDIO_VOL_UP,KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_MINUS,
    LT(3, KC_TAB),  KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,           TG(4),                                          TG(5),          KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           KC_BSLS,
    OSM(MOD_LCTL),  KC_A,           LC_S,           LA_D,           LG_F,           ME_G,           QK_REP,                                         QK_AREP,        ME_H,           RG_J,           RA_K,           RC_L,           KC_SCLN,        KC_QUOTE,
    OSM(MOD_LSFT),  KC_Z,           KC_X,           KC_C,           LS_V,           HY_B,                                                                           HY_N,           RS_M,           KC_COMMA,       KC_DOT,         KC_UP,          KC_SLASH,
    AC_TOGG,        KC_LEFT_CTRL,   KC_LEFT_ALT,    KC_LEFT_GUI,    MO(4),          KC_NO,                                                                          KC_NO,          MO(5),          TD(TD_GRAPHITE),KC_LEFT,        KC_DOWN,        KC_RIGHT,
    GU_SPC,         LT(2, KC_TAB),  MAGIC,                          TD(TD_SELWORD), TD(TD_BSPC),    LT(3, KC_ENTER)
  ),
  [1] = LAYOUT_moonlander(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_Z,           KC_L,           KC_D,           KC_C,           KC_B,           KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_J,           KC_F,           KC_O,           KC_U,           KC_SCLN,        KC_TRANSPARENT,
    KC_TRANSPARENT, KC_N,           LC_R,           LA_T,           LG_S,           KC_G,           KC_TRANSPARENT,                                                                 KC_TRANSPARENT, KC_Y,           RG_H,           RA_E,           RC_I,           KC_A,           KC_TRANSPARENT,
    KC_TRANSPARENT, KC_Q,           KC_X,           KC_M,           LS_W,           KC_V,                                           KC_K,           RS_P,           KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                                                 KC_TRANSPARENT, KC_TRANSPARENT, TO(0),          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),
  [2] = LAYOUT_moonlander(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_PERC,        KC_MINUS,       KC_PLUS,        KC_DLR,         KC_AMPR,        KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_EQUAL,       KC_7,           KC_8,           KC_9,           KC_CIRC,        KC_TRANSPARENT,
    KC_TRANSPARENT, KC_EXLM,        KC_COLN,        KC_DOT,         KC_COMMA,       KC_PIPE,        KC_TRANSPARENT,                                                                 KC_TRANSPARENT, LALT(KC_3),     KC_1,           KC_2,           KC_3,           KC_UNDS,        KC_TRANSPARENT,
    KC_TRANSPARENT, KC_CIRC,        KC_SLASH,       KC_ASTR,        KC_HASH,        KC_TILD,                                        KC_AT,          KC_4,           KC_5,           KC_6,           KC_QUES,        KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_BSLS,        KC_TRANSPARENT, KC_TRANSPARENT,                 KC_TRANSPARENT, KC_0,           KC_1
  ),
  [3] = LAYOUT_moonlander(
    KC_GRAVE,       KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,          KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_F6,          KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,
    KC_TRANSPARENT, KC_DLR,         KC_EXLM,        KC_DQUO,        KC_AMPR,        KC_HASH,        KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_EQUAL,       KC_RCBR,        KC_LCBR,        KC_CIRC,        KC_SCLN,        KC_F12,
    KC_TRANSPARENT, KC_PIPE,        KC_LABK,        KC_MINUS,       KC_RABK,        KC_EQUAL,       KC_TRANSPARENT,                                                                 KC_TRANSPARENT, KC_LABK,        KC_RPRN,        KC_LPRN,        KC_TILD,        KC_UNDS,        KC_TRANSPARENT,
    KC_TRANSPARENT, KC_CIRC,        KC_SCLN,        KC_DOT,         KC_COMMA,       KC_TILD,                                        KC_RABK,        KC_RBRC,        KC_LBRC,        KC_EXLM,        KC_QUES,        KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_BSLS,        KC_SLASH,       KC_TRANSPARENT,                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),
  [4] = LAYOUT_moonlander(
    LGUI(KC_GRAVE), KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    LGUI(KC_TAB),   KC_PAGE_UP,     LGUI(KC_LBRC),  KC_UP,          LGUI(KC_RBRC),  QK_LLCK,        KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    LCTL(KC_LEFT_ALT),KC_PGDN,        KC_LEFT,        KC_DOWN,        KC_RIGHT,       KC_DELETE,      KC_TRANSPARENT,                                                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_HOME,        ST_MACRO_0,     KC_END,         KC_ENTER,                                       KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                 KC_TRANSPARENT, KC_0,           KC_1
  ),
  [5] = LAYOUT_moonlander(
    AU_TOGG,        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, QK_BOOT,
    MU_TOGG,        KC_TRANSPARENT, KC_MS_BTN1,     KC_MS_UP,       KC_MS_BTN2,     KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    MU_NEXT,        KC_TRANSPARENT, KC_MS_LEFT,     KC_MS_DOWN,     KC_MS_RIGHT,    KC_TRANSPARENT, RGB_TOG,                                                                        TOGGLE_LAYER_COLOR,KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_MEDIA_PLAY_PAUSE,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, RGB_TOG,                                 RGB_MODE_FORWARD, KC_TRANSPARENT, KC_MEDIA_PREV_TRACK,KC_MEDIA_NEXT_TRACK,KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, RGB_SAI,                                                                                                        RGB_SAD,        KC_TRANSPARENT, KC_AUDIO_VOL_DOWN,KC_AUDIO_MUTE,  KC_TRANSPARENT, KC_TRANSPARENT,
    RGB_SPI,        RGB_VAI,        RGB_HUI,                        RGB_HUD,        RGB_VAD,        RGB_SPD
  ),
};

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
  'L', 'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
  'L', 'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
  'L', 'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R',
  'L', 'L', 'L', 'L', 'L', '*', '*', 'R', 'R', 'R', 'R', 'R',
                 '*', '*', '*', '*', '*', '*'
);
// clang-format on

extern rgb_config_t rgb_matrix_config;

RGB hsv_to_rgb_with_value(HSV hsv) {
    RGB   rgb = hsv_to_rgb(hsv);
    float f   = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
    return (RGB){f * rgb.r, f * rgb.g, f * rgb.b};
}

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

/* RGB LED map appears to number keys thusly

0 5 10 15 20 25 29         65 61 56 51 46 41 36
1 6 11 16 21 26 30         66 62 57 52 47 42 37
2 7 12 17 22 27 31         67 63 58 53 48 43 38
3 8 13 18 23 28               64 59 54 49 44 39
4 9 14 19 24    35         71    60 55 50 45 40
             32 33 34   70 69 68
*/

// clang-format off
const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
  [0] = {{0,0,0}, {204,150,150}, {139,204,224}, {88,171,107}, {124,150,150}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {139,204,224}, {0,0,0}, {0,0,0}, {139,204,224}, {0,0,0}, {138,18,181}, {0,0,0}, {0,0,0}, {138,18,181}, {0,0,0}, {236,156,217}, {0,0,0}, {0,0,0}, {236,156,217}, {88,171,107}, {55,150,150}, {0,0,0}, {0,0,0}, {16,248,188}, {110,251,143}, {0,0,0}, {55,150,150}, {0,160,140}, {19,234,177}, {36,150,150}, {155,150,150}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {139,204,224}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {138,18,181}, {0,0,0}, {171,150,150}, {0,0,0}, {0,0,0}, {237,156,217}, {88,171,107}, {73,150,150}, {0,0,0}, {0,0,0}, {16,248,188}, {110,251,143}, {0,0,0}, {73,150,150}, {21,160,140}, {204,150,150}, {0,233,226}, {220,150,150}, {0,0,0}},

  [1] = {{0,0,0}, {204,150,150}, {139,204,224}, {88,171,107}, {124,150,150}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {139,204,224}, {0,0,0}, {0,0,0}, {139,204,224}, {0,0,0}, {138,18,181}, {0,0,0}, {0,0,0}, {138,18,181}, {0,0,0}, {236,156,217}, {0,0,0}, {0,0,0}, {236,156,217}, {88,171,107}, {55,150,150}, {0,0,0}, {0,0,0}, {16,248,188}, {110,251,143}, {0,0,0}, {55,150,150}, {0,160,140}, {19,234,177}, {36,150,150}, {155,150,150}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {139,204,224}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {138,18,181}, {0,0,0}, {0,0,140}, {0,0,0}, {0,0,0}, {237,156,217}, {88,171,107}, {73,150,150}, {0,0,0}, {0,0,0}, {16,248,188}, {110,251,143}, {0,0,0}, {73,150,150}, {21,160,140}, {204,150,150}, {0,233,226}, {220,150,150}, {0,0,0}},

  [2] = { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,245,245}, {0,0,0}, {0,245,245}, {0,0,0}, {0,0,0}, {0,245,245}, {0,0,0}, {0,245,245}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {74,255,255}, {74,255,255}, {74,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {188,255,255}, {188,255,255}, {188,255,255}, {0,0,0}, {0,0,0}, {188,255,255}, {188,255,255}, {188,255,255}, {0,0,0}, {0,0,0}, {188,255,255}, {188,255,255}, {188,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} },

  [3] = { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {152,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {152,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {74,255,206}, {152,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {74,255,206}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {41,255,255}, {41,255,255}, {41,255,255}, {0,0,0}, {0,0,0}, {41,255,255}, {41,255,255}, {41,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} },

  [4] = { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {152,255,255}, {74,255,206}, {0,0,0}, {0,0,0}, {0,0,0}, {74,255,206}, {74,255,206}, {0,0,0}, {0,0,0}, {0,0,0}, {152,255,255}, {74,255,206}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} },

  [5] = { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {188,255,255}, {131,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {131,255,255}, {131,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {188,255,255}, {131,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,245,245}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} },

};

// clang-format on

void set_layer_color(int layer) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        HSV hsv = {
            .h = pgm_read_byte(&ledmap[layer][i][0]),
            .s = pgm_read_byte(&ledmap[layer][i][1]),
            .v = pgm_read_byte(&ledmap[layer][i][2]),
        };
        if (!hsv.h && !hsv.s && !hsv.v) {
            rgb_matrix_set_color(i, 0, 0, 0);
        } else {
            RGB rgb = hsv_to_rgb_with_value(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
}

bool rgb_matrix_indicators_user(void) {
    if (rawhid_state.rgb_control) {
        return false;
    }
    if (!keyboard_config.disable_layer_led) {
        switch (biton32(layer_state)) {
            case 0:
                set_layer_color(0);
                break;
            case 1:
                set_layer_color(1);
                break;
            case 2:
                set_layer_color(2);
                break;
            case 3:
                set_layer_color(3);
                break;
            case 4:
                set_layer_color(4);
                break;
            case 5:
                set_layer_color(5);
                break;
            default:
                if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
                    rgb_matrix_set_color_all(0, 0, 0);
                }
        }
    } else {
        if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
            rgb_matrix_set_color_all(0, 0, 0);
        }
    }

    return true;
}

typedef struct {
    bool    is_press_action;
    uint8_t step;
} tap;

enum { SINGLE_TAP = 1, SINGLE_HOLD, DOUBLE_TAP, DOUBLE_HOLD, DOUBLE_SINGLE_TAP, MORE_TAPS };

static tap td_state[4];

uint8_t td_step(tap_dance_state_t *state);

uint8_t td_step(tap_dance_state_t *state) {
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

void td_graphite_finished(tap_dance_state_t *state, void *user_data);
void td_graphite_reset(tap_dance_state_t *state, void *user_data);

void td_graphite_finished(tap_dance_state_t *state, void *user_data) {
    td_state[TD_GRAPHITE].step = td_step(state);
    switch (td_state[TD_GRAPHITE].step) {
        case DOUBLE_TAP:
            layer_move(1);
            break;
    }
}

void td_graphite_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (td_state[TD_GRAPHITE].step) {}
    td_state[TD_GRAPHITE].step = 0;
}
void on_td_bspc(tap_dance_state_t *state, void *user_data);
void td_bspc_finished(tap_dance_state_t *state, void *user_data);
void td_bspc_reset(tap_dance_state_t *state, void *user_data);

void on_td_bspc(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {
        tap_code16(KC_BSPC);
        tap_code16(KC_BSPC);
        tap_code16(KC_BSPC);
    }
    if (state->count > 3) {
        tap_code16(KC_BSPC);
    }
}

void td_bspc_finished(tap_dance_state_t *state, void *user_data) {
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

void td_bspc_reset(tap_dance_state_t *state, void *user_data) {
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

void td_esc_grv_finished(tap_dance_state_t *state, void *user_data);
void td_esc_grv_reset(tap_dance_state_t *state, void *user_data);

void td_esc_grv_finished(tap_dance_state_t *state, void *user_data) {
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

void td_esc_grv_reset(tap_dance_state_t *state, void *user_data) {
    td_state[TD_ESC_GRV].step = 0;
}

// tap_code16 bypasses process_record, so module keycodes need this instead
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

void td_selword_finished(tap_dance_state_t *state, void *user_data) {
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

void td_selword_reset(tap_dance_state_t *state, void *user_data) {
    td_state[TD_SELWORD].step = 0;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_GRAPHITE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_graphite_finished, td_graphite_reset),
    [TD_BSPC]     = ACTION_TAP_DANCE_FN_ADVANCED(on_td_bspc, td_bspc_finished, td_bspc_reset),
    [TD_ESC_GRV]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_esc_grv_finished, td_esc_grv_reset),
    [TD_SELWORD]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_selword_finished, td_selword_reset),
};

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LC_S:
        case LA_D:
        case LG_F:
        case LS_V:
        case RS_M:
        case RG_J:
        case RA_K:
        case RC_L:
        case LC_R:
        case LA_T:
        case LG_S:
        case LS_W:
        case RS_P:
        case RG_H:
        case RA_E:
        case RC_I:
        case GU_SPC:
        case HY_B:
        case HY_N:
        case ME_G:
        case ME_H:
            return 100;
        default:
            return 0;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    magic_update_buffer(keycode, record);

    switch (keycode) {
        case OSM(MOD_LCTL): {
            static uint16_t last_lctl_press    = 0;
            static bool     lctl_double_tapped = false;
            if (record->event.pressed) {
                lctl_double_tapped = timer_elapsed(last_lctl_press) < 200;
                last_lctl_press    = timer_read();
            } else if (lctl_double_tapped) {
                caps_word_toggle();
                lctl_double_tapped = false;
            }
            break;
        }
        case QK_MODS ... QK_MODS_MAX:
            // Mouse and consumer keys (volume, media) with modifiers work inconsistently across operating systems,
            // this makes sure that modifiers are always applied to the key that was pressed.
            if (IS_MOUSE_KEYCODE(QK_MODS_GET_BASIC_KEYCODE(keycode)) || IS_CONSUMER_KEYCODE(QK_MODS_GET_BASIC_KEYCODE(keycode))) {
                if (record->event.pressed) {
                    add_mods(QK_MODS_GET_MODS(keycode));
                    send_keyboard_report();
                    wait_ms(2);
                    register_code(QK_MODS_GET_BASIC_KEYCODE(keycode));
                    return false;
                } else {
                    wait_ms(2);
                    del_mods(QK_MODS_GET_MODS(keycode));
                }
            }
            break;
        case ST_MACRO_0:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL(SS_TAP(X_RIGHT)) SS_DELAY(100) SS_LCTL(SS_TAP(X_LEFT)) SS_DELAY(100) SS_LCTL(SS_LSFT(SS_TAP(X_RIGHT))));
            }
            break;

        // "Magic" key
        case MAGIC:
            magic_expand(record);
            break;

        case RGB_SLD:
            if (rawhid_state.rgb_control) {
                return false;
            }
            if (record->event.pressed) {
                rgblight_mode(1);
            }
            return false;
    }
    return true;
}
