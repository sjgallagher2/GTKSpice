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
#include <app/component_params.h>

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

#endif /* GTKSPICE_STATE_H */
