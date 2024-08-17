#include QMK_KEYBOARD_H

enum layers {
    DEFAULT = 0,
    SYM = 1,
    NAV = 2,
    NUM = 3,
};

enum keycodes {
    SW_WIN
};

#define OS_SHFT OSM(MOD_LSFT)
#define OS_CTRL OSM(MOD_LCTL)
#define OS_ALT OSM(MOD_LALT)
#define OS_SUPR OSM(MOD_LGUI)
#define TAB_N C(KC_TAB)
#define TAB_P C(S(KC_TAB))


bool sw_win_active = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEFAULT] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                                   KC_SPC,  MO(NAV),     MO(SYM), KC_LSFT
    ),
    [SYM] = LAYOUT(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
        KC_COLN, KC_GRV,  KC_EQL,  KC_UNDS, KC_LBRC,     KC_RBRC, OS_SHFT, OS_CTRL, OS_ALT,  OS_SUPR,
        KC_SCLN, KC_TILD, KC_PLUS, KC_MINS, KC_LCBR,     KC_RCBR, _______, _______, _______, _______,
                                   _______, _______,     _______, _______
    ),
    [NAV] = LAYOUT(
        _______, _______, KC_TAB,  KC_ESC,  _______,     TAB_P,   _______,  _______, TAB_N,  KC_DEL,
        OS_SUPR, OS_ALT,  OS_CTRL, OS_SHFT, SW_WIN,      KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_BSPC,
        _______, _______, _______, _______, _______,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_ENT,
                                   _______, _______,     _______, _______
    ),
    [NUM] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_BSLS, _______, _______, _______, _______,     _______, _______, _______, _______, _______,
        KC_PIPE, _______, _______, _______, _______,     _______, _______, _______, _______, _______,
                                   _______, _______,     _______, _______
    ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, NUM);
}

void update_swapper(
    bool *active,
    uint16_t cmdish,
    uint16_t tabish,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {
    if (keycode == trigger) {
        if (record->event.pressed) {
            if (!*active) {
                *active = true;
                register_code(cmdish);
            }
            register_code(tabish);
        } else {
            unregister_code(tabish);
            // Don't unregister cmdish until some other key is hit or released.
        }
    } else if (*active) {
        unregister_code(cmdish);
        *active = false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(
        &sw_win_active, KC_LALT, KC_TAB, SW_WIN,
        keycode, record
    );

    return true;
}
