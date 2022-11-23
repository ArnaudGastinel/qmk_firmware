/* Copyright 2022 Arnaud Gastinel
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

#pragma once

#ifdef AUDIO_ENABLE
   #define STARTUP_SONG SONG(PLANCK_SOUND)

   #define DEFAULT_LAYER_SONGS \
         { SONG(QWERTY_SOUND), SONG(COLEMAK_SOUND), SONG(DVORAK_SOUND) }
#endif


/*
 * MIDI options
 */

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/

#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
//#define MIDI_ADVANCED

/* override number of MIDI tone keycodes (each octave adds 12 keycodes and allocates 12 bytes) */
//#define MIDI_TONE_KEYCODE_OCTAVES 2

// Window manager keys
#define WM_FULL LGUI(KC_UP)
#define WM_MV_DESK_L LSFT(LGUI(KC_LEFT))
#define WM_MV_DESK_R LSFT(LGUI(KC_RIGHT))

#define CTL_ESC CTL_T(KC_ESC)
#define CTL_ENT CTL_T(KC_ENT)

#define GUI_L LT(FN_LAYER, KC_LBRC)
#define GUI_R LT(FN_LAYER, KC_RBRC)

#define NAV_SPC LT(NAV_LAYER, KC_SPC)

#define FORCE_NKRO

#define TAPPING_TOGGLE 2

#define TAPPING_TERM 175

// Caps word
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#define CAPS_WORD_IDLE_TIMEOUT 10000

#define COMBO_TERM 20

#define IGNORE_MOD_TAP_INTERRUPT
#define PERMISSIVE_HOLD

// LeaderKey
#define LEADER_TIMEOUT 1000
// https://docs.qmk.fm/#/feature_leader_key?id=infinite-leader-key-timeout
#define LEADER_NO_TIMEOUT

#define NO_MUSIC_MODE

#define AUDIO_CLICKY
// LEDs
#define BOTTOM_RIGHT_LED 1
#define BOTTOM_LEFT_LED 8
#define TOP_LEFT_LED 6
#define TOP_MIDDLE_LEFT_LED 5
#define TOP_MIDDLE_RIGHT_LED 4
#define TOP_RIGHT_LED 3

#define MIN_INDICATION_DURATION 1500