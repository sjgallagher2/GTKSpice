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
    }
    return ret;
}

std::shared_ptr<Action> ActionFactory::make_action(ActionType action, PointParameters pp)
{
    std::shared_ptr<Action> ret = nullptr;
    switch(action)
    {
    case DRAW_POINT:
        ret = std::make_shared<DrawPointAction>(_objecttree,pp);
        break;
    }
    return ret;
}

std::shared_ptr<Action> ActionFactory::make_action(ActionType action, LineParameters lp,std::vector<Vertex> vs)
{
    std::shared_ptr<Action> ret = nullptr;
    switch(action)
    {
    case ADD_LINE:
        ret = std::make_shared<AddLineAction>(_objecttree,lp,vs);
        break;
    }

    return ret;
}

std::shared_ptr<Action> ActionFactory::make_action(ActionType action, LineParameters lp)
{
    std::shared_ptr<Action> ret = nullptr;
    switch(action)
    {
    case APPEND_LINE:
        ret = std::make_shared<AppendLineAction>(_objecttree,lp);
        break;
    }

    return ret;
}

std::shared_ptr<Action> ActionFactory::make_action(ActionType action, int index)
{
    std::shared_ptr<Action> ret = nullptr;
    switch(action)
    {
    case REMOVE_LINE:
        ret = std::make_shared<RemoveLineAction>(_objecttree, index);
        break;
    }

    return ret;
}

std::shared_ptr<Action> ActionFactory::make_action(ActionType action, int index, std::vector<int> vertexindices)
{
    std::shared_ptr<Action> ret = nullptr;
    switch(action)
    {
    case MOVE_LINE_VERTICES:
        ret = std::make_shared<MoveLineVerticesAction>(_objecttree, index,vertexindices);
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

