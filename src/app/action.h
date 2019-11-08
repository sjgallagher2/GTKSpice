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

#include <memory>
#include <gtkmm.h>
#include <app/object_tree.h>
#include <app/component_params.h>
#include <app/gtkspice_state.h>
#include <app/action_stack.h>
#include <app/canvas.h>
#include <app/object_tree.h>
#include <app/schematic.h>
#include <app/spice_data.h>
#include <tools/tool_manager.h>

class Canvas;
class ActionStack;

enum ActionType 
{
    NO_ACTION,
    UNDO,
    REDO,
    SET_TOOL_DRAW_WIRE,
    SET_TOOL_DRAW_LINE,
    SET_TOOL_POINTER,
    SET_TOOL_DELETE,
    SET_TOOL_DRAG,
    SET_TOOL_FLIPLR,
    SET_TOOL_FLIPUD,
    SET_TOOL_ROTCCW,
    SET_TOOL_ROTCW,
    SET_TOOL_TEXT_ADD,
    SET_TOOL_TEXT_MODIFY,
    DRAW_POINT,
    ADD_LINE,
    APPEND_LINE,
    REMOVE_LINE,
    MOVE_LINE,
    MOVE_LINE_VERTICES
};

class Action
{
public:
    Action() {}
    virtual ~Action() {}

    virtual void execute() = 0;
    virtual void unexecute() = 0;
    virtual bool stackable() const {return _stackable;}
protected:
    bool _stackable = false;
};

class ActionFactory
{
public:
    ActionFactory() {}

    ~ActionFactory() {}

    void update(std::shared_ptr<ObjectTree> ot, 
        std::shared_ptr<Schematic> sch, 
        std::shared_ptr<Canvas> canv,
        std::shared_ptr<ActionStack> as,
        std::shared_ptr<GtkSpiceState> state,
        std::shared_ptr<ToolManager> tlmgr)
    {
        _objecttree = ot;
        _schematic = sch;
        _canvas = canv;
        _actionstack = as;
        _state = state;
        _toolmgr = tlmgr;
    }

    std::shared_ptr<Action> make_action(ActionType action);
    std::shared_ptr<Action> make_action(ActionType action, PointParameters pp);
    std::shared_ptr<Action> make_action(ActionType action, LineParameters lp);
    std::shared_ptr<Action> make_action(ActionType action, LineParameters lp,std::vector<Vertex> vs);
    std::shared_ptr<Action> make_action(ActionType action, int index);
    std::shared_ptr<Action> make_action(ActionType action, int index, std::vector<int> vertexindices);

private:
    std::shared_ptr<ObjectTree> _objecttree;
    std::shared_ptr<Schematic> _schematic;
    std::shared_ptr<Canvas> _canvas;
    std::shared_ptr<ActionStack> _actionstack;
    std::shared_ptr<ToolManager> _toolmgr;
    std::shared_ptr<GtkSpiceState> _state;
};

class UndoAction : public Action
{
public:
    UndoAction(std::shared_ptr<ActionStack> as) : _actionstack(as) {_stackable = false;}
    ~UndoAction() {}
    virtual void execute();
    virtual void unexecute() {}
protected:
    std::shared_ptr<ActionStack> _actionstack;
};

class RedoAction : public Action
{
public:
    RedoAction(std::shared_ptr<ActionStack> as) : _actionstack(as) {_stackable = false;}
    ~RedoAction() {}
    virtual void execute();
    virtual void unexecute() {}
protected:
    std::shared_ptr<ActionStack> _actionstack;
};

class DrawPointAction : public Action
{
public:
    DrawPointAction(std::shared_ptr<ObjectTree> ot, PointParameters pp) : 
        _objecttree(ot), _pp(pp)
    {_stackable = true;}
    virtual ~DrawPointAction() {}

    void execute();
    void unexecute();
protected:
    std::shared_ptr<ObjectTree> _objecttree;
    PointParameters _pp;
};

/* AppendLine Action is used to push a new line onto the stack, typically 
 * with only two vertices. The key difference is the line remains active
 * after it is added.
 */
class AppendLineAction : public Action
{
public:
    AppendLineAction(std::shared_ptr<ObjectTree> ot, LineParameters lp) : 
        _objecttree(ot), _lp(lp) 
    {_stackable = true;}
    virtual ~AppendLineAction() {}

    void execute();
    void reexecute();
    void unexecute();
protected:
    std::shared_ptr<ObjectTree> _objecttree;
    LineParameters _lp;
    bool _stay_active = true;
};

/* AddLineAction is used to add a completed line, which will not remain
 * active after it is added
 */
class AddLineAction : public Action
{
public:
    AddLineAction(std::shared_ptr<ObjectTree> ot,
        LineParameters lp,
        std::vector<Vertex> vertices) : 
        _objecttree(ot), _lp(lp)
    {_stackable=true;}
    virtual ~AddLineAction() {}

    void execute();
    void unexecute();
protected:
    std::shared_ptr<ObjectTree> _objecttree;
    LineParameters _lp;
};
class RemoveLineAction : public Action
{
public:
    RemoveLineAction(std::shared_ptr<ObjectTree> ot, int index) : 
        _objecttree(ot),_index(index){_stackable=true;}
    virtual ~RemoveLineAction() {}

    void execute();
    void unexecute();
protected:
    std::shared_ptr<ObjectTree> _objecttree;
    LineParameters _lp;
    int _index;
};
class MoveLineAction : public Action
{
public:
    MoveLineAction(std::shared_ptr<ObjectTree> ot,int index) : 
        _objecttree(ot), _index(index)
    {_stackable=true;}
    virtual ~MoveLineAction() {}

    void execute();
    void unexecute();
protected:
    std::shared_ptr<ObjectTree> _objecttree;
    LineParameters _lp;
    int _index;
};
class MoveLineVerticesAction : public Action
{
public:
    MoveLineVerticesAction(std::shared_ptr<ObjectTree> ot,
        int lineindex,
        std::vector<int> vertexindices) : 
        _objecttree(ot),_lineindex(lineindex),_vertexindices(vertexindices)
    {_stackable=true;}
    virtual ~MoveLineVerticesAction() {}

    void execute();
    void unexecute();
protected:
    std::shared_ptr<ObjectTree> _objecttree;
    int _lineindex;
    std::vector<int> _vertexindices;
};
class AddRectAction : public Action
{
public:
    AddRectAction();
    ~AddRectAction();

    void execute();
    void unexecute();
private:
    std::shared_ptr<ObjectTree> _objecttree;
    RectParameters _rp;
    bool _stay_active = true;
};


#endif /* ACTION_H */
