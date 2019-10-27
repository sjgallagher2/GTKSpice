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
#ifndef TOOL_VIEW_PAN_H
#define TOOL_VIEW_PAN_H

#include <tools/tool.h>
#include <tools/tool_view.h>
#include <app/coordinate.h>
#include <app/gtkspice_state.h>

class PanViewTool : public ViewTool
{
public:
    PanViewTool() {}
    virtual ~PanViewTool() {}

    virtual void tool_click_handler(Coordinate mousepos,int button,int modifier,int cselect);
    virtual void tool_move_handler(Coordinate mousepos);
    virtual void tool_key_handler(int key,int modifier);
    virtual Glib::ustring get_tool_cursor_name() {return "grabbing";}
};

#endif /* TOOL_VIEW_PAN_H */