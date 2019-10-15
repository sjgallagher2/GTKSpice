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
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
/* This is a state machine with the intention of tracking clicking and mouse
 * behavior in the DrawingArea.
 * 
 */

#include <gtkmm.h>
#include <gui/drawingeventbox.h>
#include <app/coordinate.h>
#include <app/component_params.h>

class StateMachine
{
public:
    static void init(DrawingEventBox* drawevents);
    ~StateMachine();

    enum DrawStates {POINTER,PAN,TOOL};
    enum ToolTypes {NO_TOOL,
        DRAW_WIRE,DRAW_POINT,
        DRAG_COMPONENT,ROTATE_COMPONENT,FLIP_COMPONENT_LR,FLIP_COMPONENT_UD, DROP_COMPONENT,
        ROTATE_COMPONENT_CW,ROTATE_COMPONENT_CCW,
        ADD_TEXT,MODIFY_TEXT,
        DELETE};
    enum MouseClicks {LEFT_PRESS=1,LEFT_RELEASE, 
        MIDDLE_PRESS, MIDDLE_RELEASE,
        RIGHT_PRESS, RIGHT_RELEASE, 
        DOUBLE_LEFT, DOUBLE_RIGHT};
    enum KeyModifiers {NO_MOD,SHIFT,CTRL,ALT};

    static void change_state(DrawStates ds) {_prevstate=_state; _state = ds;}
    static DrawStates get_state() {return _state;}

    static void change_tool(ToolTypes tt) {_tool = tt;}
    static ToolTypes get_tool() {return _tool;}

    static Glib::ustring get_cursor_name();

    static void click_handler(Coordinate mousepos,int button,int modifier,int cselect);
    static void move_handler(Coordinate mousepos);
    static void key_handler(int key,int modifier);
    static void tool_click_handler(Coordinate mousepos,int button,int modifier,int cselect);
    static void tool_move_handler(Coordinate mousepos);
    static void tool_key_handler(int key,int modifier);

private:
    StateMachine(DrawingEventBox* drawevents);
    static StateMachine* _sm;

    static Glib::ustring get_tool_cursor_name();

    static DrawingEventBox* _drawevents;
    static DrawStates _state;
    static DrawStates _prevstate;
    static ToolTypes _tool;

};

#endif /* STATE_MACHINE_H */
