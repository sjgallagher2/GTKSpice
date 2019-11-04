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

#ifndef KEY_ACCEL_MAP_H
#define KEY_ACCEL_MAP_H

#include <map>
#include <app/action.h>

class KeyAccelMap
{
public:
    KeyAccelMap() {}
    ~KeyAccelMap() {}
    void add_pair(KeyModifiers mod,int keyval, ActionType action,bool overwrite);
    ActionType get_action(KeyModifiers mod, int key);
    void set_defaults();

private:
    std::map<std::pair<KeyModifiers,int>,ActionType> key_map; // Keyval, ActionType

};

/*
enum Keys {
    KEY_BackSpace, KEY_Tab, KEY_Enter, KEY_Escape,
    KEY_Delete, KEY_Home, KEY_End, KEY_Left,
    KEY_Up, KEY_Right, KEY_Down, KEY_F1,
    KEY_F2, KEY_F3, KEY_F4, KEY_F5,
    KEY_F6, KEY_F7, KEY_F8, KEY_F9,
    KEY_F10, KEY_F11, KEY_F12, KEY_a,
    KEY_b, KEY_c, KEY_d, KEY_e,
    KEY_f, KEY_g, KEY_h, KEY_i,
    KEY_j, KEY_k, KEY_l, KEY_m,
    KEY_n, KEY_o, KEY_p, KEY_q,
    KEY_r, KEY_s, KEY_t, KEY_u,
    KEY_v, KEY_w, KEY_x, KEY_y,
    KEY_z, KEY_0, KEY_1, KEY_2,
    KEY_3, KEY_4, KEY_5, KEY_6,
    KEY_7, KEY_8, KEY_9 
    };
*/
#endif /* KEY_ACCEL_MAP_H */
