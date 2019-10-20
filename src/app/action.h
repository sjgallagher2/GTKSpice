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

/* Base class for actions. This helps with undo/redo. 
 * 
 * An action must have an execute() function and an unexecute() function.
 * The action must be able to communicate with objects such as components,
 * files, the GUI, etc. and tell them to perform the required action. Then
 * its job is done.
 * 
 * The object to-be-acted-upon should be passed to the constructor. For drawing
 * objects this is the ObjectTree.
 * 
 */

#ifndef ACTION_H
#define ACTION_H

#include <gtkmm.h>
#include <app/object_tree.h>
#include <app/component_params.h>

class Action
{
public:
    Action() {}
    virtual ~Action() {}

    virtual void execute() = 0;
    virtual void unexecute() = 0;
};

class DrawPointAction : public Action
{
public:
    DrawPointAction(PointParameters pp) : _pp(pp) {}
    virtual ~DrawPointAction() {}

    void execute();
    void unexecute();
protected:
    PointParameters _pp;
};

class AppendLineAction : public Action
{
public:
    AppendLineAction(LineParameters lp) : _lp(lp) {}
    virtual ~AppendLineAction() {}

    void execute();
    void reexecute();
    void unexecute();
protected:
    LineParameters _lp;
    bool _stay_active = true;
};
class AddLineAction : public Action
{
public:
    AddLineAction(LineParameters lp,std::vector<Vertex> vertices) : _lp(lp) {}
    virtual ~AddLineAction() {}

    void execute();
    void unexecute();
protected:
    LineParameters _lp;
};
class RemoveLineAction : public Action
{
public:
    RemoveLineAction(int index) : _index(index){}
    virtual ~RemoveLineAction() {}

    void execute();
    void unexecute();
protected:
    LineParameters _lp;
    int _index;
};
class MoveLineAction : public Action
{
    MoveLineAction(int index) : _index(index){}
    virtual ~MoveLineAction() {}

    void execute();
    void unexecute();
protected:
    LineParameters _lp;
    int _index;
};
class MoveLineVerticesAction : public Action
{
    MoveLineVerticesAction(int lineindex,std::vector<int> vertexindices) : _lineindex(lineindex),
        _vertexindices(vertexindices){}
    virtual ~MoveLineVerticesAction() {}

    void execute();
    void unexecute();
protected:
    int _lineindex;
    std::vector<int> _vertexindices;
};


// TODO Add actions for moving lines, moving line vertices



#endif /* ACTION_H */
