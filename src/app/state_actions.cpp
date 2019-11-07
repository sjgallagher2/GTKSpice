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

#include <app/state_actions.h>

void SetToolAction::execute()
{
}
void SetToolAction::unexecute()
{
}

void SetToolDrawWireAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(DRAW_WIRE));
}
void SetToolDrawLineAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(DRAW_LINE));
}
void SetToolPointerAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(POINTER));
}
void SetToolDeleteAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(DELETE));
}
void SetToolDragAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(COMPONENT_DRAG));
}
void SetToolFlipLRAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(COMPONENT_FLIPLR));
}
void SetToolFlipUDAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(COMPONENT_FLIPUD));
}
void SetToolRotateCWAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(COMPONENT_ROTATECW));
}
void SetToolRotateCCWAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(COMPONENT_ROTATECCW));
}
void SetToolTextAddAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(TEXT_ADD));
}
void SetToolTextModifyAction::execute()
{
    _state->active_tool(_toolmgr->get_tool(TEXT_MODIFY));
}

