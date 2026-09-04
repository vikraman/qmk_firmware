// Magic key: a trigger string is expanded into a completion.
// Rules are added to magic_rules below. Longest match wins.
// Candidates are cycled by repeated presses.

#include "magic.h"
#include <string.h>

#define MAGIC_BUF_SIZE 24
static char magic_buf[MAGIC_BUF_SIZE + 1] = {0};

static void magic_buf_push(char c) {
    size_t len = strlen(magic_buf);
    if (len >= MAGIC_BUF_SIZE) {
        memmove(magic_buf, magic_buf + 1, MAGIC_BUF_SIZE - 1);
        len = MAGIC_BUF_SIZE - 1;
    }
    magic_buf[len]     = c;
    magic_buf[len + 1] = '\0';
}

static void magic_buf_pop(void) {
    size_t len = strlen(magic_buf);
    if (len > 0) {
        magic_buf[len - 1] = '\0';
    }
}

static void magic_buf_clear(void) {
    magic_buf[0] = '\0';
}

typedef struct {
    const char        *trigger;    // matched as a typed suffix
    const char *const *candidates; // typed as completion; cycled on repeat
    uint8_t            count;
} magic_rule_t;

#define RULE1(t, a) {t, (const char *const[]){a}, 1}
#define RULE2(t, a, b) {t, (const char *const[]){a, b}, 2}
#define RULE3(t, a, b, c) {t, (const char *const[]){a, b, c}, 3}
#define RULE4(t, a, b, c, d) {t, (const char *const[]){a, b, c, d}, 4}
#define RULE5(t, a, b, c, d, e) {t, (const char *const[]){a, b, c, d, e}, 5}

// a rule's candidates are cycled by repeated presses (nothing typed in between)
static const magic_rule_t *cycle_rule   = NULL;
static uint8_t             cycle_index  = 0;
static uint8_t             last_out_len = 0;
static bool                cycle_active = false;

static char keycode_to_char(uint16_t keycode) {
    switch (keycode) {
        case KC_A ... KC_Z:
            return 'a' + (keycode - KC_A);
        case KC_1 ... KC_9:
            return '1' + (keycode - KC_1);
        case KC_0:
            return '0';
        case KC_BSLS:
            return '\\';
        case KC_MINUS:
            return '-';
        case KC_EQUAL:
            return '=';
        case KC_DOT:
            return '.';
        case KC_COMMA:
            return ',';
        case KC_SLASH:
            return '/';
        case KC_SCLN:
            return ';';
        case KC_QUOTE:
            return '\'';
        case KC_GRAVE:
            return '`';
        case KC_EXLM:
            return '!';
        case KC_LABK:
            return '<';
        case KC_RABK:
            return '>';
        case KC_PLUS:
            return '+';
        case KC_ASTR:
            return '*';
        case KC_UNDS:
            return '_';
        case KC_COLN:
            return ':';
        case KC_PIPE:
            return '|';
        case KC_AMPR:
            return '&';
        case KC_SPACE:
        case KC_TAB:
        case KC_ENTER:
            return ' ';
        default:
            return 0;
    }
}

void magic_update_buffer(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return;
    }
    if (keycode == MAGIC) {
        return; // handled separately below; cycle state kept
    }
    cycle_active = false; // a cycle is broken by any other key
    if (keycode == KC_BSPC) {
        magic_buf_pop();
        return;
    }

    // mod-tap / layer-tap is unwrapped to the tapped key
    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
            if (record->tap.count == 0) {
                magic_buf_clear(); // held as mod
                return;
            }
            keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
            break;
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            if (record->tap.count == 0) {
                magic_buf_clear();
                return;
            }
            keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
            break;
    }

    char c = keycode_to_char(keycode);
    if (c) {
        magic_buf_push(c);
    } else {
        magic_buf_clear();
    }
}

static const magic_rule_t magic_rules[] = {
    // --- LaTeX / math ---
    RULE1("\\al", "pha"),   // alpha
    RULE1("\\be", "ta"),    // beta
    RULE1("\\ga", "mma"),   // gamma
    RULE1("\\de", "lta"),   // delta
    RULE1("\\ep", "silon"), // epsilon
    RULE1("\\ze", "ta"),    // zeta
    RULE1("\\et", "a"),     // eta
    RULE1("\\th", "eta"),   // theta
    RULE1("\\io", "ta"),    // iota
    RULE1("\\ka", "ppa"),   // kappa
    RULE1("\\la", "mbda"),  // lambda
    RULE1("\\si", "gma"),   // sigma
    RULE1("\\ta", "u"),     // tau
    RULE1("\\up", "silon"), // upsilon
    RULE1("\\ph", "i"),     // phi
    RULE1("\\ch", "i"),     // chi
    RULE1("\\ps", "i"),     // psi
    RULE1("\\om", "ega"),   // omega
    RULE1("\\rh", "o"),     // rho
    RULE1("\\fo", "rall"),  // forall
    RULE1("\\ex", "ists"),  // exists

    // --- programming ---
    RULE2("fun", "ction", "ctor"), // function , functor
    RULE1("ins", "tance"),
    RULE2("rec", "ursive", "ursion"), // recursive , recursion
    RULE2("mon", "ad", "oid"),        // monad , monoid
    RULE2("app", "licative", "end"),  // applicative , append
    RULE1("typ", "eclass"),

    // --- proofs ---
    RULE2("ind", "uction", "uctive"), // induction , inductive
    RULE1("rew", "rite"),
    RULE2("the", "orem", "refore"),        // theorem , therefore
    RULE2("con", "structor", "tinuation"), // constructor , continuation
    RULE1("hyp", "othesis"),
    RULE2("ref", "lexivity", "erence"), // reflexivity , reference
    RULE1("tra", "nsitivity"),
    RULE2("com", "mutativity", "pleteness"), // commutativity , completeness

    // --- shell ---
    RULE1("exp", "ort"),
    RULE1("gre", "p"),
    RULE1("ali", "as"),
    RULE1("env", "ironment"),

    // --- punctuation ---
    RULE2("-", ">", "-"),  // -> , --
    RULE2("=", "=", "> "), // == , =>
    RULE2(":", ":", "="),  // :: , :=
    RULE1("!", "="),       // !=
    RULE1("<", "="),       // <=
    RULE1(">", "="),       // >=
    RULE1("+", "+"),       // ++
    RULE1("*", "*"),       // **
    RULE1("_", "_"),       // __
    RULE1("/", "/"),       // //
    RULE1("`", "``"),      // ```
    RULE1(".", "."),       // ..
    RULE1("|", "|"),       // ||
    RULE1("&", "&"),       // &&
    RULE1("q", "u"),       // qu

    // --- prose: type a word prefix, then Magic completes/cycles it ---
    RULE3(" ", "the ", "then ", "their "),
    RULE5("wh", "ich ", "at ", "en ", "ere ", "o "),  // which what when where who
    RULE5("th", "e ", "is ", "at ", "ese ", "ose "),  // the this that these those
    RULE3("an", "d ", "y ", "other "),                // and any another
    RULE4("be", "cause ", "fore ", "tween ", "ing "), // because before between being
    RULE3("wi", "th ", "ll ", "thout "),              // with will without
    RULE3("yo", "u ", "ur ", "urself "),              // you your yourself
    RULE3("sh", "ould ", "e ", "all "),               // should she shall
    RULE3("co", "uld ", "nsider ", "me "),            // could consider come
    RULE2("pl", "ease ", "an "),                      // please plan
    RULE3("ab", "out ", "ove ", "le "),               // about above able
    RULE3("wo", "uld ", "rk ", "rld "),               // would work world
};

static void backspace_n(uint8_t n) {
    for (uint8_t i = 0; i < n; i++) {
        tap_code16(KC_BSPC);
        magic_buf_pop();
    }
}

static void type_and_track(const char *text) {
    send_string(text);
    for (const char *p = text; *p; p++) {
        magic_buf_push(*p);
    }
    last_out_len = strlen(text);
}

void magic_expand(keyrecord_t *record) {
    if (!record->event.pressed) {
        return;
    }

    if (cycle_active) {
        cycle_index = (cycle_index + 1) % cycle_rule->count;
        backspace_n(last_out_len);
        type_and_track(cycle_rule->candidates[cycle_index]);
        return;
    }

    size_t              buf_len  = strlen(magic_buf);
    const magic_rule_t *best     = NULL;
    size_t              best_len = 0;
    for (size_t i = 0; i < ARRAY_SIZE(magic_rules); i++) {
        size_t trig_len = strlen(magic_rules[i].trigger);
        if (trig_len > best_len && trig_len <= buf_len && strcmp(magic_buf + buf_len - trig_len, magic_rules[i].trigger) == 0) {
            best     = &magic_rules[i];
            best_len = trig_len;
        }
    }

    if (best) {
        type_and_track(best->candidates[0]);
        if (best->count > 1) {
            cycle_rule   = best;
            cycle_index  = 0;
            cycle_active = true;
        }
    } else {
        tap_code16(get_last_keycode()); // repeat key is used as fallback
    }
}
