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
#include <app/action.h>

void DrawPointAction::execute()
{
    //ObjectTree::add_component(_pp);
}
void DrawPointAction::unexecute()
{
    //ObjectTree::remove_point(_index);
}
void AppendLineAction::execute()
{
    //int index = ObjectTree::add_component(_lp);
    //ObjectTree::set_line_active(index,_stay_active);
    //_lp.cp.index = index;
}
void AppendLineAction::unexecute()
{
    //_lp = ObjectTree::get_line_parameters(_lp.cp.index);
    //_stay_active = false; // Ensure that if we re-add the line, it won't be active
    //ObjectTree::remove_component("line",_lp.cp.index);
}
void RemoveLineAction::execute()
{
    //_lp = ObjectTree::get_line_parameters(_index); // Store line params
    //ObjectTree::remove_component("line",_index); // Pop
}
void RemoveLineAction::unexecute()
{
    //ObjectTree::add_component(_lp);
}
void MoveLineAction::execute()
{
}
void MoveLineAction::unexecute()
{
}
void MoveLineVerticesAction::execute()
{
}
void MoveLineVerticesAction::unexecute()
{
}

