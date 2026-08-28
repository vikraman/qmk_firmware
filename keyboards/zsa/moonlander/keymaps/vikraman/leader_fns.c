#include QMK_KEYBOARD_H
#include "leader_fns.h"

void leader_wrap(void *data) {
    send_string_P((const char *)data);
    tap_code(KC_LEFT);
}

void leader_tex_env(void *data) {
    const char *name = (const char *)data;
    send_string("\\begin{");
    send_string_P(name);
    send_string("}\n\n\\end{");
    send_string_P(name);
    send_string("}");
    tap_code(KC_UP);
}
