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

#include <iostream>
#include <tools/tool_delete.h>
#include <app/component_params.h>


std::shared_ptr<Action> DeleteTool::tool_click_handler(Coordinate mousepos,int button,int modifier,int cselect)
{
    std::shared_ptr<Action> ret = nullptr;
    // Get component under mouse and delete it
    int undermouse_index = _objecttree->get_line_under_cursor(mousepos);
    if(undermouse_index != -1)
        ret = _actionfactory->make_action(REMOVE_LINE,undermouse_index);
    else
    {
        if(button == LEFT_PRESS)
        {
            _vfeatures->StartTempRectangle(mousepos);
            _select_start = mousepos;
        }
        else if(button == LEFT_RELEASE)
        {
            _vfeatures->FinishTempRectangle();
            _select_end = mousepos;
            // Get selected lines, delete them
            std::vector<int> lines = _objecttree->get_lines_in_selection(_select_start,_select_end);
            if(!lines.empty())
                ret = _actionfactory->make_action(REMOVE_LINES,lines);
        }
    }
    return ret;
}
std::shared_ptr<Action> DeleteTool::tool_move_handler(Coordinate mousepos)
{
    std::shared_ptr<Action> ret = nullptr;
    _vfeatures->UpdateTempRectangle(mousepos);
    return ret;
}
std::shared_ptr<Action> DeleteTool::tool_key_handler(int key,int modifier)
{
    std::shared_ptr<Action> ret = nullptr;
    return ret;
}
