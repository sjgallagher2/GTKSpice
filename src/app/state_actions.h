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


#ifndef STATE_ACTIONS_H
#define STATE_ACTIONS_H

#include <memory>
#include <app/action.h>
#include <tools/tool.h>
#include <tools/tool_manager.h>
#include <app/gtkspice_state.h>

class SetToolAction : public Action 
{
public:
    SetToolAction(std::shared_ptr<ToolManager> tm, 
        std::shared_ptr<GtkSpiceState> state) : 
        _state(state),_toolmgr(tm) {_stackable = false;}
    virtual ~SetToolAction() {}

    virtual void execute();
    virtual void unexecute();
protected:
    std::shared_ptr<GtkSpiceState> _state;
    std::shared_ptr<ToolManager> _toolmgr;
};

class SetToolDrawWireAction : public SetToolAction
{
public:
    SetToolDrawWireAction(std::shared_ptr<ToolManager> tm,
        std::shared_ptr<GtkSpiceState> state) : SetToolAction(tm,state) {}
    virtual ~SetToolDrawWireAction() {}
    virtual void execute();
    virtual void unexecute() {}
};
class SetToolPointerAction : public SetToolAction
{
public:
    SetToolPointerAction(std::shared_ptr<ToolManager> tm,
        std::shared_ptr<GtkSpiceState> state) : SetToolAction(tm,state) {}
    virtual ~SetToolPointerAction() {}
    virtual void execute();
    virtual void unexecute() {}
};
class SetToolDeleteAction : public SetToolAction
{
public:
    SetToolDeleteAction(std::shared_ptr<ToolManager> tm,
        std::shared_ptr<GtkSpiceState> state) : SetToolAction(tm,state) {}
    virtual ~SetToolDeleteAction() {}
    virtual void execute();
    virtual void unexecute() {}
};
class SetToolDragAction : public SetToolAction
{
public:
    SetToolDragAction(std::shared_ptr<ToolManager> tm,
        std::shared_ptr<GtkSpiceState> state) : SetToolAction(tm,state) {}
    virtual ~SetToolDragAction() {}
    virtual void execute();
    virtual void unexecute() {}
};
class SetToolFlipLRAction : public SetToolAction
{
public:
    SetToolFlipLRAction(std::shared_ptr<ToolManager> tm,
        std::shared_ptr<GtkSpiceState> state) : SetToolAction(tm,state) {}
    virtual ~SetToolFlipLRAction() {}
    virtual void execute();
    virtual void unexecute() {}
};
class SetToolFlipUDAction : public SetToolAction
{
public:
    SetToolFlipUDAction(std::shared_ptr<ToolManager> tm,
        std::shared_ptr<GtkSpiceState> state) : SetToolAction(tm,state) {}
    virtual ~SetToolFlipUDAction() {}
    virtual void execute();
    virtual void unexecute() {}
};
class SetToolRotateCWAction : public SetToolAction
{
public:
    SetToolRotateCWAction(std::shared_ptr<ToolManager> tm,
        std::shared_ptr<GtkSpiceState> state) : SetToolAction(tm,state) {}
    virtual ~SetToolRotateCWAction() {}
    virtual void execute();
    virtual void unexecute() {}
};
class SetToolRotateCCWAction : public SetToolAction
{
public:
    SetToolRotateCCWAction(std::shared_ptr<ToolManager> tm,
        std::shared_ptr<GtkSpiceState> state) : SetToolAction(tm,state) {}
    virtual ~SetToolRotateCCWAction() {}
    virtual void execute();
    virtual void unexecute() {}
};
class SetToolTextAddAction : public SetToolAction
{
public:
    SetToolTextAddAction(std::shared_ptr<ToolManager> tm,
        std::shared_ptr<GtkSpiceState> state) : SetToolAction(tm,state) {}
    virtual ~SetToolTextAddAction() {}
    virtual void execute();
    virtual void unexecute() {}
};
class SetToolTextModifyAction : public SetToolAction
{
public:
    SetToolTextModifyAction(std::shared_ptr<ToolManager> tm,
        std::shared_ptr<GtkSpiceState> state) : SetToolAction(tm,state) {}
    virtual ~SetToolTextModifyAction() {}
    virtual void execute();
    virtual void unexecute() {}
};

#endif /* STATE_ACTIONS_H */
