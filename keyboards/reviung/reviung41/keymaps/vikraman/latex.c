// Copyright 2020 gtips
// SPDX-License-Identifier: GPL-2.0-or-later

static bool process_latex(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT_BEG:
            if (record->event.pressed) {
                SEND_STRING("\\begin{}" SS_TAP(X_LEFT));
            }
            return false;
        case LT_END:
            if (record->event.pressed) {
                SEND_STRING("\\end{}" SS_TAP(X_LEFT));
            }
            return false;
        case LT_FRAC:
            if (record->event.pressed) {
                SEND_STRING("\\frac{}{}");
            }
            return false;
        case LT_SQRT:
            if (record->event.pressed) {
                SEND_STRING("\\sqrt{}" SS_TAP(X_LEFT));
            }
            return false;
        case LT_SUM:
            if (record->event.pressed) {
                SEND_STRING("\\sum_{}^{}");
            }
            return false;
        case LT_INT:
            if (record->event.pressed) {
                SEND_STRING("\\int_{}^{}");
            }
            return false;
        case LT_TEXT:
            if (record->event.pressed) {
                SEND_STRING("\\text{}" SS_TAP(X_LEFT));
            }
            return false;
        case LT_MATH:
            if (record->event.pressed) {
                SEND_STRING("$$" SS_TAP(X_LEFT));
            }
            return false;
        case LT_DMATH:
            if (record->event.pressed) {
                SEND_STRING("\\[\\]" SS_TAP(X_LEFT) SS_TAP(X_LEFT));
            }
            return false;
        case LT_SUB:
            if (record->event.pressed) {
                SEND_STRING("_{}" SS_TAP(X_LEFT));
            }
            return false;
        case LT_SUP:
            if (record->event.pressed) {
                SEND_STRING("^{}" SS_TAP(X_LEFT));
            }
            return false;
        case LT_EQN:
            if (record->event.pressed) {
                SEND_STRING("\\begin{equation}\n  \n\\end{equation}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_ALN:
            if (record->event.pressed) {
                SEND_STRING("\\begin{align}\n  \n\\end{align}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_CAS:
            if (record->event.pressed) {
                SEND_STRING("\\begin{cases}\n  \n\\end{cases}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_BMAT:
            if (record->event.pressed) {
                SEND_STRING("\\begin{bmatrix}\n  \n\\end{bmatrix}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_THM:
            if (record->event.pressed) {
                SEND_STRING("\\begin{theorem}\n  \n\\end{theorem}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_LEM:
            if (record->event.pressed) {
                SEND_STRING("\\begin{lemma}\n  \n\\end{lemma}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_PRF:
            if (record->event.pressed) {
                SEND_STRING("\\begin{proof}\n  \n\\end{proof}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_ITEM:
            if (record->event.pressed) {
                SEND_STRING("\\begin{itemize}\n  \\item \n\\end{itemize}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_ENUM:
            if (record->event.pressed) {
                SEND_STRING("\\begin{enumerate}\n  \\item \n\\end{enumerate}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_DEF:
            if (record->event.pressed) {
                SEND_STRING("\\begin{definition}\n  \n\\end{definition}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_COR:
            if (record->event.pressed) {
                SEND_STRING("\\begin{corollary}\n  \n\\end{corollary}" SS_TAP(X_UP) SS_TAP(X_END));
            }
            return false;
        case LT_SEC:
            if (record->event.pressed) {
                SEND_STRING("\\section{}" SS_TAP(X_LEFT));
            }
            return false;
        case LT_SSEC:
            if (record->event.pressed) {
                SEND_STRING("\\subsection{}" SS_TAP(X_LEFT));
            }
            return false;
        case LT_REF:
            if (record->event.pressed) {
                SEND_STRING("\\ref{}" SS_TAP(X_LEFT));
            }
            return false;
        case LT_LBL:
            if (record->event.pressed) {
                SEND_STRING("\\label{}" SS_TAP(X_LEFT));
            }
            return false;
        case SC_TOGG:
            if (record->event.pressed) {
                sentence_case_toggle();
            }
            return false;
        default:
            return true;
    }
}
