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
#define DRIVER_LED_TOTAL 48
#include QMK_KEYBOARD_H
#include "print.h"
#include "color_maps.c"
// Timer and counter variables
static uint16_t tap_timer = 0;
static uint8_t tap_count = 0;
bool reset_to_default_light = false;
#define TAP_TIME 200 // Timeframe in milliseconds
#define _DEFAULT 0
#define _APEX 3
// Define custom keycodes
enum custom_keycodes {
    TAP_HANDLER = SAFE_RANGE,
    SWITCH_APEX = SAFE_RANGE+1,
};

extern struct ApexColorMap apex_color_map;
extern struct DefaultColorMap default_color_map;

typedef enum {
    DEFAULT = 0,
    APEX = 3,
} Profile;

Profile current_profile = DEFAULT;

void set_defaut_light_mode(void){
    // MY_RGB DEFAULT_RGB = {255, 255, 255};
    rgb_matrix_mode(RGB_MATRIX_HUE_PENDULUM);
        
    reset_to_default_light = false;
}

void switchToProfile(Profile new_profile){
    if (current_profile == new_profile) {
        reset_to_default_light = true;
        layer_move(_DEFAULT);
        current_profile = DEFAULT;
        return;
    }

    switch (new_profile) {
        case APEX:
            layer_move(_APEX);
            current_profile = APEX;
            break;
        default:
            reset_to_default_light = true;
            layer_move(_DEFAULT);
            current_profile = DEFAULT;
            break;
        }
    uprintf("%d urrent profile: \n", current_profile);
    uprintf("%d new profile: \n", new_profile);
}



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_DEFAULT] = LAYOUT_65_ansi(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
                         KC_HOME, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC,
                         KC_BSLS, KC_PGUP, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN,
                         KC_QUOT, KC_ENT, KC_PGDN, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT,
                         KC_SLSH, KC_RSFT, KC_UP, SWITCH_APEX, KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RALT, KC_RGUI,
                         KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),
    [1] = LAYOUT_65_ansi(QK_GESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
                         KC_DEL, KC_HOME, _______, RM_TOGG, RM_NEXT, RM_HUEU, RM_HUED, RM_SATU, RM_SATD, RM_VALU,
                         RM_VALD, _______, KC_PSCR, KC_SCRL, KC_PAUS, QK_BOOT, KC_PGUP, KC_CAPS, RM_SPDU, RM_SPDD,
                         _______, _______, _______, _______, _______, _______, _______, _______, _______, EE_CLR,
                         KC_PGDN, KC_LSFT, _______, _______, _______, _______, _______, NK_TOGG, _______, _______,
                         _______, _______, _______, KC_VOLU, KC_MUTE, _______, _______, _______, _______, _______,
                         _______, _______, KC_MPRV, KC_VOLD, KC_MNXT),
    [2] = LAYOUT_65_ansi(_______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                         _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                         _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                         _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                         _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                         _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                         _______, _______, _______, _______, _______, _______, _______, _______),
    [_APEX] = LAYOUT_65_ansi(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL,
                             KC_BSPC, KC_HOME, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P,
                             KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J,
                             KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGDN, KC_LSFT, KC_Z, KC_X, TAP_HANDLER, KC_V,
                             KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, SWITCH_APEX, KC_LCTL, KC_LGUI,
                             KC_LALT, KC_SPC, KC_RALT, KC_RGUI, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),
};


void keyboard_post_init_user(void) {
    initialize_color_maps();
    rgb_matrix_init();
    set_defaut_light_mode();
    // debug_enable = true;
    // debug_matrix=true;
}

void displayRgbMatrix(ColorKeys* color_keys, int num_keys, MY_RGB default_rgb)
{
    for (int i = 0; i < num_keys; i++)
    {
        if (timer_elapsed(color_keys[i].timer.start) <= color_keys[i].timer.period){
            rgb_matrix_set_color(color_keys[i].index, color_keys[i].rgb.r, color_keys[i].rgb.g, color_keys[i].rgb.b);
        } else {
            rgb_matrix_set_color(color_keys[i].index, default_rgb.r, default_rgb.g, default_rgb.b);
        }
    }
}

bool rgb_matrix_indicators_user(void)
{
    switch (current_profile) {
        case DEFAULT:
        {
            if (reset_to_default_light){
                set_defaut_light_mode();
            }
            return true;
        }
        case APEX:
        {   
            MY_RGB APEX_RGB = {20, 255, 0};
            displayRgbMatrix(apex_color_map.color_keys, apex_color_map.num_keys, APEX_RGB);
            return true;
        }

        default:
            return true;
        }
    return true;
}

// Process custom keycode
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case TAP_HANDLER:
            {
                tap_timer = timer_read();
                tap_count++;
                
                return false;
            }
            case SWITCH_APEX:
            {
                print("pressed swith to apex\n");
                switchToProfile(APEX);
                return false;
            }
            default:
                return true;
        }
    }
    return true;
}

ColorKeys tickApexColorKey(ColorKeys color_key){
    ColorKeys new_color_key = {color_key.index, color_key.rgb, {color_key.timer.period, timer_read()}};
    return new_color_key;
}

void matrix_scan_user(void) {
    if (current_profile == APEX) {
        if (timer_elapsed(tap_timer) >= TAP_TIME && tap_count > 0) {
            if (tap_count == 1) {
                send_string(",");
                apex_color_map.color_keys[0] = tickApexColorKey(apex_color_map.color_keys[0]);
            }
            else if (tap_count == 2)
            {
                send_string(".");
                apex_color_map.color_keys[1] = tickApexColorKey(apex_color_map.color_keys[1]);
            }
            else if (tap_count == 3)
            {
                send_string("/");
                apex_color_map.color_keys[2] = tickApexColorKey(apex_color_map.color_keys[2]);
            }
            else
            {
                send_string("'");
                apex_color_map.color_keys[3] = tickApexColorKey(apex_color_map.color_keys[3]);
            }
            tap_count = 0;
        }
        if (tap_count >= 4) {
            tap_count = 0;
            send_string("'");
            apex_color_map.color_keys[3] = tickApexColorKey(apex_color_map.color_keys[3]);
        }
    }
    
}
