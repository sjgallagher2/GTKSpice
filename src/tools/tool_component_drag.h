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
#ifndef TOOL_COMPONENT_DRAG_H
#define TOOL_COMPONENT_DRAG_H

#include <memory>
#include <app/action.h>
#include <tools/tool.h>
#include <tools/tool_component.h>

class DragComponentTool : public ComponentTool 
{
public:
    DragComponentTool(std::shared_ptr<ActionFactory> af) : ComponentTool(af) {}
    virtual ~DragComponentTool() {}

    virtual std::shared_ptr<Action> tool_click_handler(Coordinate mousepos,int button,int modifier,int cselect);
    virtual std::shared_ptr<Action> tool_move_handler(Coordinate mousepos);
    virtual std::shared_ptr<Action> tool_key_handler(int key,int modifier);
    virtual Glib::ustring get_tool_cursor_name() {return "grabbing";}
};

#endif /* TOOL_COMPONENT_DRAG_H */
