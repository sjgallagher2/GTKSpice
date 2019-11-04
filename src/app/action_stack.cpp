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

void ActionLifo::push(std::shared_ptr<Action> i)
{
	// Push to stack
	if(_stk.size() < MAX_LEN)
		_stk.push_front(i);
	else if(_stk.size() == MAX_LEN) // If length is max, rotate
	{
		std::rotate(_stk.begin(),_stk.begin()+_stk.size()-1,_stk.end());
		_stk.front() = i;
	}
}
std::shared_ptr<Action> ActionLifo::pop()
{
	// Pop from stack, return value
	std::shared_ptr<Action> ret;
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


ActionStack::~ActionStack()
{
}

void ActionStack::push(std::shared_ptr<Action> c)
{
	// TODO Handle special actions (undo, redo, others?) separately?
	if(c)
	{
		c->execute();
		if(c->stackable())
		{
			_stk.push(c); // Push to undo stack
			if(!_rstk.empty()) // Check redo stack, if a new action is added this stack is always cleared
				_rstk.clear();
		}
	}
}

void ActionStack::undo()
{
    std::shared_ptr<Action> undone = _stk.pop();
	if(undone)
	{
    	undone->unexecute();
    	_rstk.push(undone);
	}
}
void ActionStack::redo()
{
    std::shared_ptr<Action> redone = _rstk.pop();
	if(redone)
	{
    	redone->execute();
    	_stk.push(redone);
	}
}
