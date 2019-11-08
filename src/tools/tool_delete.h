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
#ifndef TOOL_DELETE_H
#define TOOL_DELETE_H

#include <memory>
#include <app/object_tree.h>
#include <app/action.h>
#include <tools/tool.h>

// TODO Add drag select
class DeleteTool : public Tool
{
public:
    DeleteTool(std::shared_ptr<ActionFactory> af,
        std::shared_ptr<ObjectTree> ot) :
        Tool(af),
        _objecttree(ot) {}
    virtual ~DeleteTool() {}

    virtual std::shared_ptr<Action> tool_click_handler(Coordinate mousepos,int button,int modifier,int cselect);
    virtual std::shared_ptr<Action> tool_move_handler(Coordinate mousepos);
    virtual std::shared_ptr<Action> tool_key_handler(int key,int modifier);
    virtual Glib::ustring get_tool_cursor_name() {return "scissor";}

protected:
    std::shared_ptr<ObjectTree> _objecttree;
};

#endif /* TOOL_DELETE_H */
