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
#include <tools/tool_draw_line.h>
#include <gui/drawingeventbox.h>

std::shared_ptr<Action> DrawLineTool::tool_click_handler(Coordinate mousepos,int button,int modifier,int cselect)
{
    // TODO Adding vertices, removing vertices should be actions, not handled directly
    std::shared_ptr<Action> ret = nullptr;
    // Check for active wires (wires being drawn)
    // If double-click, set no line active
    // If right-click, remove line vertex
    mousepos.set_to_snapped();
    if(_objecttree->has_active_line())
    {
        if(button == LEFT_RELEASE)
            _objecttree->add_line_vertex(mousepos);
        else if(button == DOUBLE_LEFT)
            _objecttree->set_no_line_active();
        else if(button == RIGHT_RELEASE)
            _objecttree->remove_line_vertex(mousepos);
    }
    else
    {
        if(button == LEFT_RELEASE)
        {
            LineParameters lp;
            lp.vertices.push_back(std::make_shared<Vertex>(mousepos));
            lp.vertices.push_back(std::make_shared<Vertex>(mousepos));
            ret = _actionfactory->make_action(APPEND_LINE,lp);
        }
    }
    return ret;
}
std::shared_ptr<Action> DrawLineTool::tool_move_handler(Coordinate mousepos)
{
    mousepos.set_to_snapped();
    std::shared_ptr<Action> ret = nullptr;
    _objecttree->move_line_vertex(mousepos);
    return ret;

}
std::shared_ptr<Action> DrawLineTool::tool_key_handler(int key,int modifier)
{
    std::shared_ptr<Action> ret = nullptr;
    return ret;
}

