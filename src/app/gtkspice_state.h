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

#include <gtkmm.h>
#include <memory>
#include <gui/drawingeventbox.h>
#include <app/coordinate.h>

class Tool;

/* 
 * Object to track the program state
 */
class GtkSpiceState
{
public:
    GtkSpiceState();
    ~GtkSpiceState();

    Glib::ustring get_cursor_name();
    void active_tool(std::shared_ptr<Tool> at) {_active_tool = at;}
    std::shared_ptr<Tool> active_tool() const {return _active_tool;}

private:
    Glib::ustring get_tool_cursor_name();
    std::shared_ptr<Tool> _active_tool;
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

#endif /* GTKSPICE_STATE_H */
