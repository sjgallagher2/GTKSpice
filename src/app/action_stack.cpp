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
#include <app/action_stack.h>

ActionStack* ActionStack::_as = nullptr;
ActionLifo ActionStack::_stk(ActionStack::MAX_LEN);
ActionLifo ActionStack::_rstk(ActionStack::MAX_LEN);

void ActionLifo::push(Action* i)
{
	// Push to stack
	if(_stk.size() < MAX_LEN)
		_stk.push_front(i);
	else if(_stk.size() == MAX_LEN)
	{
		std::rotate(_stk.begin(),_stk.begin()+_stk.size()-1,_stk.end());
		_stk.front() = i;
	}
}
Action* ActionLifo::pop()
{
	// Pop from stack, return value
	Action* ret;
	if(_stk.size() > 0)
	{
		ret = _stk.front();
		_stk.pop_front();
	}
	else
	{
		ret = nullptr; 
	}
	return ret;
}


ActionStack* ActionStack::Instance()
{
    if(!ActionStack::_as)
        ActionStack::_as = new ActionStack;

    return ActionStack::_as;
}

ActionStack::~ActionStack()
{
    delete ActionStack::_as;
}

void ActionStack::push(Action* c)
{
    c->execute();
    _stk.push(c);
    if(!_rstk.empty())
        _rstk.clear();
}

void ActionStack::undo()
{
    Action* undone = _stk.pop();
	if(undone)
	{
    	undone->unexecute();
    	_rstk.push(undone);
	}
}
void ActionStack::redo()
{
    Action* redone = _rstk.pop();
	if(redone)
	{
    	redone->execute();
    	_stk.push(redone);
	}
}
