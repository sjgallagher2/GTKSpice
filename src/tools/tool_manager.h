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
#ifndef TOOL_MANAGER_H
#define TOOL_MANAGER_H

#include <memory>
#include <map>
#include <app/coordinate_system.h>
#include <tools/tool.h>
#include <tools/tool_component_drag.h>
#include <tools/tool_component_fliplr.h>
#include <tools/tool_component_flipud.h>
#include <tools/tool_component_rotateccw.h>
#include <tools/tool_component_rotatecw.h>
#include <tools/tool_delete.h>
#include <tools/tool_draw_wire.h>
#include <tools/tool_text_add.h>
#include <tools/tool_text_modify.h>
#include <tools/tool_view_pan.h>
#include <tools/tool_pointer.h>

class PointerTool;

class ToolManager
{
public:
    ToolManager(std::shared_ptr<CoordinateSystem> cs)
    {
        _coord_sys = cs;
        _pointer = std::make_shared<PointerTool>(cs);
        _tool_map.insert(std::pair<ToolTypes,std::shared_ptr<PointerTool>>(POINTER,_pointer));
    }
    ~ToolManager() {}

    std::shared_ptr<Tool> get_tool(ToolTypes tool)
    {
        std::shared_ptr<Tool> ret = nullptr;
        if(_tool_map.find(tool) != _tool_map.end())
        {
            ret = _tool_map.find(tool)->second;
        }
        else
        {
            ret = _pointer; // Default tool
        }
        return ret;
    }

private:
    std::map<ToolTypes,std::shared_ptr<PointerTool>> _tool_map;
    std::shared_ptr<CoordinateSystem> _coord_sys;
    std::shared_ptr<PointerTool> _pointer; // TODO Add the other tools
};

#endif /* TOOL_MANAGER_H */
