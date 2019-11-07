/* GTKSpice is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * GTKSpice is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with GTKSpice.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <app/key_accel_map.h>
#include <gtkmm.h>

void KeyAccelMap::add_pair(KeyModifiers mod, int keyval, ActionType action, bool overwrite=false)
{
    if(overwrite)
        key_map[std::pair<KeyModifiers,int>(mod,keyval)] = action;
    else
        key_map.insert(std::pair<std::pair<KeyModifiers,int>,ActionType>(std::pair<KeyModifiers,int>(mod,keyval),action));
    // insert() will return false if we try to overwrite
}

ActionType KeyAccelMap::get_action(KeyModifiers mod, int key)
{
    // Return the value associated with key
    ActionType action;
    if(key_map.find(std::pair<KeyModifiers,int>(mod,key)) != key_map.end())
        action = key_map.find(std::pair<KeyModifiers,int>(mod,key))->second;
    else
        action = NO_ACTION;
    
    return action;
}

void KeyAccelMap::set_defaults()
{
    key_map.empty();
    // These are applicable in all tools
    add_pair(CTRL,GDK_KEY_z,UNDO);
    add_pair(CTRL,GDK_KEY_r,REDO);
    add_pair(NO_MOD,GDK_KEY_l,SET_TOOL_DRAW_LINE);
    add_pair(NO_MOD,GDK_KEY_Escape,SET_TOOL_POINTER); 
    add_pair(NO_MOD,GDK_KEY_Delete,SET_TOOL_DELETE);
    // E.g. to clean up active lines before changing tools
}

/*
========= REFERENCE ===========
struct GdkEventKey {
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  guint32 time;
  guint state;
  guint keyval;
  gint length;
  gchar *string;
  guint16 hardware_keycode;
  guint8 group;
  guint is_modifier : 1;
};
#define GDK_KEY_VoidSymbol 0xffffff
#define GDK_KEY_BackSpace 0xff08
#define GDK_KEY_Tab 0xff09
#define GDK_KEY_Linefeed 0xff0a
#define GDK_KEY_Clear 0xff0b
#define GDK_KEY_Return 0xff0d
#define GDK_KEY_Pause 0xff13
#define GDK_KEY_Scroll_Lock 0xff14
#define GDK_KEY_Sys_Req 0xff15
#define GDK_KEY_Escape 0xff1b
#define GDK_KEY_Delete 0xffff
#define GDK_KEY_Multi_key 0xff20
#define GDK_KEY_Codeinput 0xff37
#define GDK_KEY_SingleCandidate 0xff3c
#define GDK_KEY_MultipleCandidate 0xff3d
#define GDK_KEY_PreviousCandidate 0xff3e
#define GDK_KEY_Kanji 0xff21
#define GDK_KEY_Muhenkan 0xff22
#define GDK_KEY_Henkan_Mode 0xff23
#define GDK_KEY_Henkan 0xff23
#define GDK_KEY_Romaji 0xff24
#define GDK_KEY_Hiragana 0xff25
#define GDK_KEY_Katakana 0xff26
#define GDK_KEY_Hiragana_Katakana 0xff27
#define GDK_KEY_Zenkaku 0xff28
#define GDK_KEY_Hankaku 0xff29
#define GDK_KEY_Zenkaku_Hankaku 0xff2a
#define GDK_KEY_Touroku 0xff2b
#define GDK_KEY_Massyo 0xff2c
#define GDK_KEY_Kana_Lock 0xff2d
#define GDK_KEY_Kana_Shift 0xff2e
#define GDK_KEY_Eisu_Shift 0xff2f
#define GDK_KEY_Eisu_toggle 0xff30
#define GDK_KEY_Kanji_Bangou 0xff37
#define GDK_KEY_Zen_Koho 0xff3d
#define GDK_KEY_Mae_Koho 0xff3e
#define GDK_KEY_Home 0xff50
#define GDK_KEY_Left 0xff51
#define GDK_KEY_Up 0xff52
#define GDK_KEY_Right 0xff53
#define GDK_KEY_Down 0xff54
#define GDK_KEY_Prior 0xff55
#define GDK_KEY_Page_Up 0xff55
#define GDK_KEY_Next 0xff56
#define GDK_KEY_Page_Down 0xff56
#define GDK_KEY_End 0xff57
#define GDK_KEY_Begin 0xff58
#define GDK_KEY_Select 0xff60
#define GDK_KEY_Print 0xff61
#define GDK_KEY_Execute 0xff62
#define GDK_KEY_Insert 0xff63
#define GDK_KEY_Undo 0xff65
#define GDK_KEY_Redo 0xff66
#define GDK_KEY_Menu 0xff67
#define GDK_KEY_Find 0xff68
#define GDK_KEY_Cancel 0xff69
#define GDK_KEY_Help 0xff6a
#define GDK_KEY_Break 0xff6b
#define GDK_KEY_Mode_switch 0xff7e
#define GDK_KEY_script_switch 0xff7e
#define GDK_KEY_Num_Lock 0xff7f
#define GDK_KEY_KP_Space 0xff80
#define GDK_KEY_KP_Tab 0xff89
#define GDK_KEY_KP_Enter 0xff8d
#define GDK_KEY_KP_F1 0xff91
#define GDK_KEY_KP_F2 0xff92
#define GDK_KEY_KP_F3 0xff93
#define GDK_KEY_KP_F4 0xff94
#define GDK_KEY_KP_Home 0xff95
#define GDK_KEY_KP_Left 0xff96
#define GDK_KEY_KP_Up 0xff97
#define GDK_KEY_KP_Right 0xff98
#define GDK_KEY_KP_Down 0xff99
#define GDK_KEY_KP_Prior 0xff9a
#define GDK_KEY_KP_Page_Up 0xff9a
#define GDK_KEY_KP_Next 0xff9b
#define GDK_KEY_KP_Page_Down 0xff9b
#define GDK_KEY_KP_End 0xff9c
#define GDK_KEY_KP_Begin 0xff9d
#define GDK_KEY_KP_Insert 0xff9e
#define GDK_KEY_KP_Delete 0xff9f
#define GDK_KEY_KP_Equal 0xffbd
#define GDK_KEY_KP_Multiply 0xffaa
#define GDK_KEY_KP_Add 0xffab
#define GDK_KEY_KP_Separator 0xffac
#define GDK_KEY_KP_Subtract 0xffad
#define GDK_KEY_KP_Decimal 0xffae
#define GDK_KEY_KP_Divide 0xffaf
#define GDK_KEY_KP_0 0xffb0
#define GDK_KEY_KP_1 0xffb1
#define GDK_KEY_KP_2 0xffb2
#define GDK_KEY_KP_3 0xffb3
#define GDK_KEY_KP_4 0xffb4
#define GDK_KEY_KP_5 0xffb5
#define GDK_KEY_KP_6 0xffb6
#define GDK_KEY_KP_7 0xffb7
#define GDK_KEY_KP_8 0xffb8
#define GDK_KEY_KP_9 0xffb9
#define GDK_KEY_F1 0xffbe
#define GDK_KEY_F2 0xffbf
#define GDK_KEY_F3 0xffc0
#define GDK_KEY_F4 0xffc1
#define GDK_KEY_F5 0xffc2
#define GDK_KEY_F6 0xffc3
#define GDK_KEY_F7 0xffc4
#define GDK_KEY_F8 0xffc5
#define GDK_KEY_F9 0xffc6
#define GDK_KEY_F10 0xffc7
#define GDK_KEY_F11 0xffc8
#define GDK_KEY_L1 0xffc8
#define GDK_KEY_F12 0xffc9
#define GDK_KEY_L2 0xffc9
#define GDK_KEY_F13 0xffca
#define GDK_KEY_L3 0xffca
#define GDK_KEY_F14 0xffcb
#define GDK_KEY_L4 0xffcb
#define GDK_KEY_F15 0xffcc
#define GDK_KEY_L5 0xffcc
#define GDK_KEY_F16 0xffcd
#define GDK_KEY_L6 0xffcd
#define GDK_KEY_F17 0xffce
#define GDK_KEY_L7 0xffce
#define GDK_KEY_F18 0xffcf
#define GDK_KEY_L8 0xffcf
#define GDK_KEY_F19 0xffd0
#define GDK_KEY_L9 0xffd0
#define GDK_KEY_F20 0xffd1
#define GDK_KEY_L10 0xffd1
#define GDK_KEY_F21 0xffd2
#define GDK_KEY_R1 0xffd2
#define GDK_KEY_F22 0xffd3
#define GDK_KEY_R2 0xffd3
#define GDK_KEY_F23 0xffd4
#define GDK_KEY_R3 0xffd4
#define GDK_KEY_F24 0xffd5
#define GDK_KEY_R4 0xffd5
#define GDK_KEY_F25 0xffd6
#define GDK_KEY_R5 0xffd6
#define GDK_KEY_F26 0xffd7
#define GDK_KEY_R6 0xffd7
#define GDK_KEY_F27 0xffd8
#define GDK_KEY_R7 0xffd8
#define GDK_KEY_F28 0xffd9
#define GDK_KEY_R8 0xffd9
#define GDK_KEY_F29 0xffda
#define GDK_KEY_R9 0xffda
#define GDK_KEY_F30 0xffdb
#define GDK_KEY_R10 0xffdb
#define GDK_KEY_F31 0xffdc
#define GDK_KEY_R11 0xffdc
#define GDK_KEY_F32 0xffdd
#define GDK_KEY_R12 0xffdd
#define GDK_KEY_F33 0xffde
#define GDK_KEY_R13 0xffde
#define GDK_KEY_F34 0xffdf
#define GDK_KEY_R14 0xffdf
#define GDK_KEY_F35 0xffe0
#define GDK_KEY_R15 0xffe0
#define GDK_KEY_Shift_L 0xffe1
#define GDK_KEY_Shift_R 0xffe2
#define GDK_KEY_Control_L 0xffe3
#define GDK_KEY_Control_R 0xffe4
#define GDK_KEY_Caps_Lock 0xffe5
#define GDK_KEY_Shift_Lock 0xffe6
#define GDK_KEY_Meta_L 0xffe7
#define GDK_KEY_Meta_R 0xffe8
#define GDK_KEY_Alt_L 0xffe9
#define GDK_KEY_Alt_R 0xffea
#define GDK_KEY_Super_L 0xffeb
#define GDK_KEY_Super_R 0xffec
#define GDK_KEY_Hyper_L 0xffed
#define GDK_KEY_Hyper_R 0xffee
#define GDK_KEY_3270_Enter 0xfd1e
#define GDK_KEY_space 0x020
#define GDK_KEY_exclam 0x021
#define GDK_KEY_quotedbl 0x022
#define GDK_KEY_numbersign 0x023
#define GDK_KEY_dollar 0x024
#define GDK_KEY_percent 0x025
#define GDK_KEY_ampersand 0x026
#define GDK_KEY_apostrophe 0x027
#define GDK_KEY_quoteright 0x027
#define GDK_KEY_parenleft 0x028
#define GDK_KEY_parenright 0x029
#define GDK_KEY_asterisk 0x02a
#define GDK_KEY_plus 0x02b
#define GDK_KEY_comma 0x02c
#define GDK_KEY_minus 0x02d
#define GDK_KEY_period 0x02e
#define GDK_KEY_slash 0x02f
#define GDK_KEY_0 0x030
#define GDK_KEY_1 0x031
#define GDK_KEY_2 0x032
#define GDK_KEY_3 0x033
#define GDK_KEY_4 0x034
#define GDK_KEY_5 0x035
#define GDK_KEY_6 0x036
#define GDK_KEY_7 0x037
#define GDK_KEY_8 0x038
#define GDK_KEY_9 0x039
#define GDK_KEY_colon 0x03a
#define GDK_KEY_semicolon 0x03b
#define GDK_KEY_less 0x03c
#define GDK_KEY_equal 0x03d
#define GDK_KEY_greater 0x03e
#define GDK_KEY_question 0x03f
#define GDK_KEY_at 0x040
#define GDK_KEY_A 0x041
#define GDK_KEY_B 0x042
#define GDK_KEY_C 0x043
#define GDK_KEY_D 0x044
#define GDK_KEY_E 0x045
#define GDK_KEY_F 0x046
#define GDK_KEY_G 0x047
#define GDK_KEY_H 0x048
#define GDK_KEY_I 0x049
#define GDK_KEY_J 0x04a
#define GDK_KEY_K 0x04b
#define GDK_KEY_L 0x04c
#define GDK_KEY_M 0x04d
#define GDK_KEY_N 0x04e
#define GDK_KEY_O 0x04f
#define GDK_KEY_P 0x050
#define GDK_KEY_Q 0x051
#define GDK_KEY_R 0x052
#define GDK_KEY_S 0x053
#define GDK_KEY_T 0x054
#define GDK_KEY_U 0x055
#define GDK_KEY_V 0x056
#define GDK_KEY_W 0x057
#define GDK_KEY_X 0x058
#define GDK_KEY_Y 0x059
#define GDK_KEY_Z 0x05a
#define GDK_KEY_bracketleft 0x05b
#define GDK_KEY_backslash 0x05c
#define GDK_KEY_bracketright 0x05d
#define GDK_KEY_asciicircum 0x05e
#define GDK_KEY_underscore 0x05f
#define GDK_KEY_grave 0x060
#define GDK_KEY_quoteleft 0x060
#define GDK_KEY_a 0x061
#define GDK_KEY_b 0x062
#define GDK_KEY_c 0x063
#define GDK_KEY_d 0x064
#define GDK_KEY_e 0x065
#define GDK_KEY_f 0x066
#define GDK_KEY_g 0x067
#define GDK_KEY_h 0x068
#define GDK_KEY_i 0x069
#define GDK_KEY_j 0x06a
#define GDK_KEY_k 0x06b
#define GDK_KEY_l 0x06c
#define GDK_KEY_m 0x06d
#define GDK_KEY_n 0x06e
#define GDK_KEY_o 0x06f
#define GDK_KEY_p 0x070
#define GDK_KEY_q 0x071
#define GDK_KEY_r 0x072
#define GDK_KEY_s 0x073
#define GDK_KEY_t 0x074
#define GDK_KEY_u 0x075
#define GDK_KEY_v 0x076
#define GDK_KEY_w 0x077
#define GDK_KEY_x 0x078
#define GDK_KEY_y 0x079
#define GDK_KEY_z 0x07a
#define GDK_KEY_braceleft 0x07b
#define GDK_KEY_bar 0x07c
#define GDK_KEY_braceright 0x07d
#define GDK_KEY_asciitilde 0x07e
#define GDK_KEY_nobreakspace 0x0a0
#define GDK_KEY_exclamdown 0x0a1
#define GDK_KEY_cr 0x9e4

*/