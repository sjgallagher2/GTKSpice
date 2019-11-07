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

#include <memory>
#include <app/coordinate.h>
#include <app/action.h>

class ActionFactory;
class Action;

/* Pure virtual Tool interace */

enum ToolTypes
{
    POINTER,
    DRAW_WIRE,
    DRAW_LINE,
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
    Tool(std::shared_ptr<ActionFactory> af) {_actionfactory = af;}
    virtual ~Tool() {}

    virtual std::shared_ptr<Action> tool_click_handler(Coordinate mousepos,int button,int modifier,int cselect) = 0;
    virtual std::shared_ptr<Action> tool_move_handler(Coordinate mousepos) = 0;
    virtual std::shared_ptr<Action> tool_key_handler(int key,int modifier) = 0;
    virtual Glib::ustring get_tool_cursor_name() = 0;

protected:
    std::shared_ptr<ActionFactory> _actionfactory;
};

#endif /* GTKSPICE_TOOL_H */