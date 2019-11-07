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
#include <tools/tool_pointer.h>

// NOTE: How can we introduce a higher degree of customizability?
// Tool Customizations:
//  Left-click action
//  Right-click action
//  Double left-click action
//  Middle-click action
//  Shift-left-click action
//  Ctrl-left-click action
//  Alt-left-click action
//  Scroll action

PointerTool::PointerTool(std::shared_ptr<ActionFactory> af, 
    std::shared_ptr<CoordinateSystem> cs) :
    Tool(af),
    _cs(cs)
{}

PointerTool::~PointerTool()
{}

std::shared_ptr<Action> PointerTool::tool_click_handler(Coordinate mousepos,int button,int modifier,int cselect)
{
    std::shared_ptr<Action> ret = nullptr;
    if(modifier == NO_MOD)
    {
        if(button == LEFT_PRESS)
        {
            // Opt: Set the rectangle select anchor
            // Opt: Select object under mouse, change tool to move
            // Opt: Change tool to pan, or do pan mode
            _panning = true;
            _pan_anchor_d = mousepos;
            _pan_anchor_d.set_to_device_coordinate();
            _pan_delta_d = Coordinate(0,0);

        }
        else if(button == LEFT_RELEASE)
        {
            _panning = false;
            _cs->finish_pan();
            // Find any objects under mouse or selection rectangle, select and highlight
        }
        else if(button == RIGHT_RELEASE)
        {
            // Find any objects under mouse, select, highlight, and context-menu
        }
    }
    return ret;
}

std::shared_ptr<Action> PointerTool::tool_move_handler(Coordinate mousepos)
{
    // Previous method had bad rounding issues or something, so now we convert
    // to device coordinates (although this somewhat defeats the purpose of
    // the CoordinateSystem interface).
    std::shared_ptr<Action> ret = nullptr;
    if(_panning)
    {
        Coordinate delta;
        mousepos.set_to_device_coordinate();
        delta = _pan_anchor_d - mousepos;
        delta.set_view_matrix(mousepos.get_view_matrix());
        delta.set_to_user_distance();
        _cs->set_pan_delta(delta);
    }
    // Check if left mouse button is down
    // Start a drag-select rectangle
    return ret;
}

std::shared_ptr<Action> PointerTool::tool_key_handler(int key,int modifier)
{
    std::shared_ptr<Action> ret = nullptr;
    return ret;
}
