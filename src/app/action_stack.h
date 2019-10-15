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

/* Base class for user actions. This helps with undo/redo. 
 * A LIFO stack, holds Action subclasses. When you push a new action,
 * it is performed, and kept on the stack. When you undo, it is popped, 
 * and undone. 
 * 
 * This is a singleton. It's basically the most straightforward implementation
 * of a stack possible.
 */

#ifndef ACTION_STACK_H
#define ACTION_STACK_H

#include <app/action.h>
#include <deque>

class ActionLifo
{
public:
	ActionLifo(int max = 10) : MAX_LEN(max) {}
	~ActionLifo() {}
	void push(Action* i);
	Action* pop();
	void printstack();
	bool empty() const {return _stk.empty();}

	void clear() {_stk.clear();}

private:	
	std::deque<Action*> _stk;
	const int MAX_LEN;

};

class ActionStack
{
public:
    static ActionStack* Instance();
    ~ActionStack();

    static void push(Action* c);
    static void undo();
    static void redo();
private:
    ActionStack() {}
    static ActionStack* _as;
    static ActionLifo _stk; // Undo stack
    static ActionLifo _rstk; // Redo stack
    static const int MAX_LEN = 25;
};

#endif /* ACTION_STACK_H */

