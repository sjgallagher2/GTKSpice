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
#include <app/state_actions.h>

std::shared_ptr<Action> ActionFactory::make_action(ActionType action)
{
    std::shared_ptr<Action> ret = nullptr;
    switch(action)
    {
    case UNDO:
        ret = std::make_shared<UndoAction>(_actionstack);
        break;
    case REDO:
        ret = std::make_shared<RedoAction>(_actionstack);
        break;
    case SET_TOOL_DRAW_WIRE:
        ret = std::make_shared<SetToolDrawWireAction>(_toolmgr,_state);
        break;
    case SET_TOOL_DRAW_LINE:
        ret = std::make_shared<SetToolDrawLineAction>(_toolmgr,_state);
        break;
    case SET_TOOL_POINTER:
        ret = std::make_shared<SetToolPointerAction>(_toolmgr,_state);
        break;
    case SET_TOOL_DELETE:
        ret = std::make_shared<SetToolDeleteAction>(_toolmgr,_state);
        break;
    case SET_TOOL_DRAG:
        ret = std::make_shared<SetToolDragAction>(_toolmgr,_state);
        break;
    case SET_TOOL_FLIPLR:
        ret = std::make_shared<SetToolFlipLRAction>(_toolmgr,_state);
        break;
    case SET_TOOL_FLIPUD:
        ret = std::make_shared<SetToolFlipUDAction>(_toolmgr,_state);
        break;
    case SET_TOOL_ROTCCW:
        ret = std::make_shared<SetToolRotateCWAction>(_toolmgr,_state);
        break;
    case SET_TOOL_ROTCW:
        ret = std::make_shared<SetToolRotateCCWAction>(_toolmgr,_state);
        break;
    case SET_TOOL_TEXT_ADD:
        ret = std::make_shared<SetToolTextAddAction>(_toolmgr,_state);
        break;
    case SET_TOOL_TEXT_MODIFY:
        ret = std::make_shared<SetToolTextModifyAction>(_toolmgr,_state);
        break;
    }
    return ret;
}

void UndoAction::execute()
{
    _actionstack->undo();
}
void RedoAction::execute()
{
    _actionstack->redo();
}
/*
void DrawPointAction::execute()
{
    _objecttree->add_component(_pp);
}
void DrawPointAction::unexecute()
{
    _objecttree->remove_component("point",_pp.cp.index);
}
void AddLineAction::execute()
{
    // Add line
}
void AddLineAction::unexecute()
{
    // Un-add line
}
void AppendLineAction::execute()
{
    int index = _objecttree->add_component(_lp);
    _objecttree->set_line_active(index,_stay_active);
    _lp.cp.index = index;
}
void AppendLineAction::unexecute()
{
    _lp = _objecttree->get_line_parameters(_lp.cp.index);
    _stay_active = false; // Ensure that if we re-add the line, it won't be active
    _objecttree->remove_component("line",_lp.cp.index);
}
void RemoveLineAction::execute()
{
    _lp = _objecttree->get_line_parameters(_index); // Store line params
    _objecttree->remove_component("line",_index); // Pop
}
void RemoveLineAction::unexecute()
{
    _objecttree->add_component(_lp);
}
void RemoveLinesAction::execute()
{
    _lp.resize(_indices.size());
    for(int i = 0; i < _indices.size(); i++)
    {
        _lp.at(i) = _objecttree->get_line_parameters(_indices.at(i)); // Store line params
        _objecttree->remove_component("line",_indices.at(i)); // Pop
    }
}
void RemoveLinesAction::unexecute()
{
    for(int i = 0; i < _lp.size(); i++)
        _objecttree->add_component(_lp.at(i));
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
*/

