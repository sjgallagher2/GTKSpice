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
#ifndef GTKSPICE_STATE_H
#define GTKSPICE_STATE_H
/* This is a state machine with the intention of tracking clicking and mouse
 * behavior in the DrawingArea.
 * 
 */

#include <gtkmm.h>
#include <gui/drawingeventbox.h>
#include <app/coordinate.h>
#include <app/component_params.h>
#include <tools/tool.h>

enum MouseClicks {LEFT_PRESS=1,LEFT_RELEASE, 
    MIDDLE_PRESS, MIDDLE_RELEASE,
    RIGHT_PRESS, RIGHT_RELEASE, 
    DOUBLE_LEFT, DOUBLE_RIGHT};
enum KeyModifiers {NO_MOD,SHIFT,CTRL,ALT};


class GtkSpiceState
{
public:
    GtkSpiceState(DrawingEventBox* drawevents);
    ~GtkSpiceState();

    void change_state(DrawStates ds) {_prevstate=_state; _state = ds;}
    DrawStates get_state() {return _state;}

    Glib::ustring get_cursor_name();

    void click_handler(Coordinate mousepos,int button,int modifier,int cselect);
    void move_handler(Coordinate mousepos);
    void key_handler(int key,int modifier);

private:
    Glib::ustring get_tool_cursor_name();

    DrawingEventBox* _drawevents;
    Tool* _active_tool;

};

#endif /* GTKSPICE_STATE_H */
