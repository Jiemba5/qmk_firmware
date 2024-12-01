/* Copyright 2022 DZTECH <moyi4681@live.cn>
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
#define _APEX 3
// Timer and counter variables
static uint16_t tap_timer = 0;
static uint8_t tap_count = 0;
#define TAP_TIME 200 // Timeframe in milliseconds

// Define custom keycodes
enum custom_keycodes {
    TAP_HANDLER = SAFE_RANGE,
    SWITCH_APEX = SAFE_RANGE+1,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_65_ansi(
        KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_HOME,
        KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
        KC_CAPS,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGDN,
        KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   SWITCH_APEX,
        KC_LCTL,  KC_LGUI, KC_LALT,                            KC_SPC,           KC_RALT, KC_RGUI, KC_RCTL,          KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT_65_ansi(
        QK_GESC, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_HOME,
        _______, RM_TOGG, RM_NEXT, RM_HUEU, RM_HUED, RM_SATU, RM_SATD, RM_VALU, RM_VALD, _______, KC_PSCR, KC_SCRL, KC_PAUS, QK_BOOT, KC_PGUP,
        KC_CAPS, RM_SPDU, RM_SPDD, _______, _______, _______, _______, _______, _______, _______, _______, _______,         EE_CLR,  KC_PGDN,
        KC_LSFT, _______, _______, _______, _______, _______, NK_TOGG, _______, _______, _______, _______, _______,          KC_VOLU, KC_MUTE,
        _______, _______, _______,                            _______,          _______, _______, _______,          KC_MPRV, KC_VOLD, KC_MNXT
    ),
    [2] = LAYOUT_65_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______,                            _______,          _______, _______, _______,          _______, _______, _______
    ),
    [_APEX] = LAYOUT_65_ansi(
        KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_HOME,
        KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
        KC_CAPS,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGDN,
        KC_LSFT,  KC_Z,    KC_X,    TAP_HANDLER,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   SWITCH_APEX,
        KC_LCTL,  KC_LGUI, KC_LALT,                            KC_SPC,           KC_RALT, KC_RGUI, KC_RCTL,          KC_LEFT, KC_DOWN, KC_RGHT
    ),
};

// Process custom keycode
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TAP_HANDLER:
        {
            if (record->event.pressed) {
                tap_timer = timer_read();
                tap_count++;
            }
            return false;
        }
        case SWITCH_APEX:
        {
            if (record->event.pressed) {
                if (layer_state & (1 << _APEX)) {
                    layer_off(_APEX);
                } else {
                    layer_on(_APEX);
                };
            }
        }
        default:
            return true;
    }
}



void matrix_scan_user(void) {
   if (timer_elapsed(tap_timer) >= TAP_TIME && tap_count > 0) {
        if (tap_count == 1) {
            send_string(",");
        } else if (tap_count == 2) {
            send_string(".");
        } else if (tap_count == 3) {
            send_string("/");
        } else {
            send_string("'");
        }
        tap_count = 0;
    }

    if (tap_count >= 4) {
        tap_count = 0;
        send_string("'");
    }
}
