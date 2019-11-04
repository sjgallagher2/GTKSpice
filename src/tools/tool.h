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
#ifndef GTKSPICE_TOOL_H
#define GTKSPICE_TOOL_H

#include <app/coordinate.h>

/* Pure virtual Tool interace */

enum ToolTypes
{
    POINTER,
    DRAW_WIRE,
    COMPONENT_DRAG,
    COMPONENT_FLIPLR,
    COMPONENT_FLIPUD,
    COMPONENT_ROTATECCW,
    COMPONENT_ROTATECW,
    DELETE,
    TEXT_MODIFY,
    TEXT_ADD
};

class Tool
{
public:
    Tool() {}
    virtual ~Tool() {}

    virtual void tool_click_handler(Coordinate mousepos,int button,int modifier,int cselect) = 0;
    virtual void tool_move_handler(Coordinate mousepos) = 0;
    virtual void tool_key_handler(int key,int modifier) = 0;
    virtual Glib::ustring get_tool_cursor_name() = 0;

};
/*
Glib::RefPtr<Gdk::Cursor> View::get_cursor()
{
    Glib::ustring ctext = GtkSpiceState::get_cursor_name();
    if(ctext.compare("default") == 0)
    {
        if(!_pointer_cursor)
        {
            auto win = get_window();
            auto display = win->get_display();
            _pointer_cursor = Gdk::Cursor::create(display,"default");
        }
        return _pointer_cursor;
    }
    else if(ctext.compare("crosshair")==0)
    {
        if(!_crosshairs_cursor)
        {
            auto win = get_window();
            auto display = win->get_display();
            _crosshairs_cursor = Gdk::Cursor::create(display,"crosshair");
        }
        return _crosshairs_cursor;
    }
    else if(ctext.compare("grabbing")==0)
    {
        if(!_grabbing_cursor)
        {
            auto win = get_window();
            auto display = win->get_display();
            _grabbing_cursor = Gdk::Cursor::create(display,"grabbing");
        }
        return _grabbing_cursor;
    }
    else if(ctext.compare("scissors")==0)
    {
        if(!_scissors_cursor)
        {
            auto win = get_window();
            auto display = win->get_display();
            auto pb = Gdk::Pixbuf::create_from_file("/home/sam/Documents/Devel/Cpp/GTKSpice/data/media/scissor-cursor-32.png");
            _scissors_cursor = Gdk::Cursor::create(display,pb,14,8);
        }
        return _scissors_cursor;
    }
}
*/

#endif /* GTKSPICE_TOOL_H */