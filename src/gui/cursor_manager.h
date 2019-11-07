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

#ifndef CURSOR_MANAGER_H
#define CURSOR_MANAGER_H

#include <gtkmm.h>
#include <map>

class CursorManager
{
public:
    CursorManager(){}
    ~CursorManager() {}

    void cm_init(Glib::RefPtr<Gdk::Display> disp)
    {
        _pointer_cursor = Gdk::Cursor::create(disp,"default");
        _crosshair_cursor = Gdk::Cursor::create(disp,"crosshair");
        _grabbing_cursor = Gdk::Cursor::create(disp,"grabbing");
        // Scissor cursor is custom
        // TODO This resource is an absolute path
        auto cpb1 = Gdk::Pixbuf::create_from_file("/home/sam/Documents/Devel/Cpp/GTKSpice/data/media/scissor-cursor-32.png");
        _scissor_cursor = Gdk::Cursor::create(disp,cpb1,14,8);

        // Create map
        _cursor_map.insert(std::pair<Glib::ustring,Glib::RefPtr<Gdk::Cursor>>("default",_pointer_cursor));
        _cursor_map.insert(std::pair<Glib::ustring,Glib::RefPtr<Gdk::Cursor>>("crosshair",_crosshair_cursor));
        _cursor_map.insert(std::pair<Glib::ustring,Glib::RefPtr<Gdk::Cursor>>("grabbing",_grabbing_cursor));
        _cursor_map.insert(std::pair<Glib::ustring,Glib::RefPtr<Gdk::Cursor>>("scissor",_scissor_cursor));
    }

    Glib::RefPtr<Gdk::Cursor> get_cursor_by_name(Glib::ustring name)
    {
        Glib::RefPtr<Gdk::Cursor> ret;

        if(_cursor_map.find(name) != _cursor_map.end())
            ret = _cursor_map.find(name)->second;
        else
            ret = _pointer_cursor;
        return ret;
    }

private:
    typedef std::map<Glib::ustring,Glib::RefPtr<Gdk::Cursor>> Cursormap;
    Glib::RefPtr<Gdk::Cursor> _crosshair_cursor;
    Glib::RefPtr<Gdk::Cursor> _pointer_cursor;
    Glib::RefPtr<Gdk::Cursor> _grabbing_cursor;
    Glib::RefPtr<Gdk::Cursor> _scissor_cursor;

    Cursormap _cursor_map;

};

#endif /* CURSOR_MANAGER_H */