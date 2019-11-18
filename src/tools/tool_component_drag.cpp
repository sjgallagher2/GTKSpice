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

#include <tools/tool_component_drag.h>

std::shared_ptr<Action> DragComponentTool::tool_click_handler(Coordinate mousepos,int button,int modifier,int cselect)
{
    std::shared_ptr<Action> ret = nullptr;
    // Check for vertices under mouse
    // If vertices, set to active
    // If no vertices, assume drag-select

    return ret;
}
std::shared_ptr<Action> DragComponentTool::tool_move_handler(Coordinate mousepos)
{
    mousepos.set_to_snapped();
    std::shared_ptr<Action> ret = nullptr;
    // Move active vertices
    return ret;

}
std::shared_ptr<Action> DragComponentTool::tool_key_handler(int key,int modifier)
{
    std::shared_ptr<Action> ret = nullptr;
    if(key == GDK_KEY_Escape && modifier == NO_MOD)
    {
    }
    return ret;
}



