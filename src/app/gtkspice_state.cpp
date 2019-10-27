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

// TODO All instances of state handling must be updated

#include <iostream>
#include <gtkmm.h>
#include <app/gtkspice_state.h>
#include <app/vertex_list.h>

Glib::ustring GtkSpiceState::get_tool_cursor_name()
{
    return _active_tool->get_tool_cursor_name();
}
GtkSpiceState::GtkSpiceState()
{
    _active_tool = nullptr;
    //_drawevents = drawevents;
    //_drawevents->button_click().connect(sigc::ptr_fun(&GtkSpiceState::click_handler));
    //_drawevents->mouse_move().connect(sigc::ptr_fun(&GtkSpiceState::move_handler));
    //_drawevents->key_press().connect(sigc::ptr_fun(&GtkSpiceState::key_handler));
}

GtkSpiceState::~GtkSpiceState()
{
}

void GtkSpiceState::click_handler(Coordinate mousepos, int button, int modifier, int cselect)
{
    _active_tool->tool_click_handler(mousepos,button,modifier,cselect);
}
void GtkSpiceState::move_handler(Coordinate mousepos)
{
    _active_tool->tool_move_handler(mousepos);
}
void GtkSpiceState::key_handler(int key,int modifier)
{
    /* UNIVERSAL KEY HANDLING */
    if(modifier == CTRL)
    {
        switch(key)
        {
        case GDK_KEY_z:
            // Undo
            //ActionStack::undo();
            //_drawevents->force_redraw();
            break;
        case GDK_KEY_r:
            // Redo
            //ActionStack::redo();
            //_drawevents->force_redraw();
            break;
        }
    }
    _active_tool->tool_key_handler(key,modifier);
}
/*
Glib::ustring GtkSpiceState::get_cursor_name()
{
    Glib::ustring cursor_name = "default";
    switch(_state)
    {
    case POINTER:
        cursor_name = "default";
        break;
    case PAN:
        cursor_name = "grabbing";
        break;
    case TOOL:
        cursor_name = get_tool_cursor_name();
        break;
    }
    return cursor_name;
}
*/

/*
{
    Glib::ustring cursor_name = "default";
    switch(_tool)
    {
    case NO_TOOL:
        break;
    case DRAW_WIRE:
        cursor_name = "crosshair";
        break;
    case DRAG_COMPONENT:
        break;
    case ROTATE_COMPONENT:
        break;
    case FLIP_COMPONENT_LR:
        break;
    case FLIP_COMPONENT_UD:
        break;
    case DROP_COMPONENT:
        break;
    case ADD_TEXT:
        break;
    case MODIFY_TEXT:
        break;
    case ROTATE_COMPONENT_CW:
        break;
    case ROTATE_COMPONENT_CCW:
        break;
    case DELETE:
        cursor_name = "scissors";
        break;
    default:
        break;
    }
    return cursor_name;
}
*/

/*
void GtkSpiceState::click_handler(Coordinate mousepos, int button, int modifier, int cselect)
{
    switch(_state)
    {
    case POINTER:
        if(button == LEFT_PRESS)
        {
            // TODO This is temporary
            change_state(PAN);
            // Find object underneath cursor from ObjectStack
        }
        else if(button == RIGHT_RELEASE)
        {
            // Find object, right-click action
        }
        break;
    case TOOL:
        // Find object underneath cursor from ObjectStack, pass to tool_handler
        //tool_click_handler(mousepos,button,modifier,cselect);
        break;
    case PAN:
        if(button == LEFT_RELEASE)
            change_state(_prevstate);
        break;
    default:
        break;

    }
}
void GtkSpiceState::move_handler(Coordinate mousepos)
{
    switch(_state)
    {
    case POINTER:
        break;
    case TOOL:
        // Find object underneath cursor from ObjectStack, pass to tool_handler
        tool_move_handler(mousepos);
        break;
    case PAN:
        break;
    default:
        break;

    }
}
*/
/*
    else
    {
        switch(_state)
        {
        case POINTER:
            if(modifier == NO_MOD)
            {
                switch(key)
                {
                case GDK_KEY_Escape:
                    change_tool(NO_TOOL);
                    change_state(POINTER);
                    break;
                case GDK_KEY_w:
                    change_state(TOOL);
                    change_tool(DRAW_WIRE);
                    break;
                case GDK_KEY_Delete:
                    change_state(TOOL);
                    change_tool(DELETE);
                    break;
                }
                break;
            }
        case PAN:
            switch(key)
            {
            default:
                break;
            }
            break;
        case TOOL:
            tool_key_handler(key,modifier);
            break;
        }
    }
*/
/*
void GtkSpiceState::tool_click_handler(Coordinate mousepos, int button, int modifier, int cselect)
{
    // Send actions to ActionStack for this component
    mousepos.set_to_snapped();
    switch(_tool)
    {
    case NO_TOOL:
        break;
    case DRAW_WIRE:
        if(ObjectTree::has_active_line())
        {
            if(button == LEFT_RELEASE)
                ObjectTree::add_line_vertex(mousepos);
            else if(button == DOUBLE_LEFT)
                ObjectTree::set_no_line_active();
            else if(button == RIGHT_RELEASE)
                ObjectTree::remove_line_vertex(mousepos);
        }
        else
        {
            if(button == LEFT_RELEASE)
            {
                LineParameters lp;
                lp.vertices.push_back(new Vertex(mousepos));
                lp.vertices.push_back(new Vertex(mousepos));
                ActionStack::push(new AppendLineAction(lp)); // Append to a line with only two vertices
            }
        }
        break;
    case DRAG_COMPONENT:
        break;
    case ROTATE_COMPONENT:
        break;
    case FLIP_COMPONENT_LR:
        break;
    case FLIP_COMPONENT_UD:
        break;
    case DROP_COMPONENT:
        break;
    case ADD_TEXT:
        break;
    case MODIFY_TEXT:
        break;
    case ROTATE_COMPONENT_CW:
        break;
    case ROTATE_COMPONENT_CCW:
        break;
    case DELETE:
        if(button==LEFT_RELEASE && cselect != -1)
            ActionStack::push(new RemoveLineAction(cselect));
        else if(button==RIGHT_RELEASE)
        {
            change_tool(NO_TOOL);
            change_state(POINTER);
        }
        break;
    default:
        break;
    }
}

void GtkSpiceState::tool_move_handler(Coordinate mousepos)
{
    mousepos.set_to_snapped();
    // Send actions to ActionStack for this component
    switch(_tool)
    {
    case NO_TOOL:
        break;
    case DRAW_WIRE:
        // Update active endpoint of active line vertex
        ObjectTree::move_line_vertex(mousepos);
        break;
    case DRAG_COMPONENT:
        break;
    case ROTATE_COMPONENT:
        break;
    case FLIP_COMPONENT_LR:
        break;
    case FLIP_COMPONENT_UD:
        break;
    case DROP_COMPONENT:
        break;
    case ADD_TEXT:
        break;
    case MODIFY_TEXT:
        break;
    case ROTATE_COMPONENT_CW:
        break;
    case ROTATE_COMPONENT_CCW:
        break;
    case DELETE:
        break;
    default:
        break;
    }
}

void GtkSpiceState::tool_key_handler(int key, int modifier)
{
    switch(_tool)
    {
    case NO_TOOL:
        break;
    case DRAW_WIRE:
        switch(key)
        {
        case GDK_KEY_Escape:
            while(ObjectTree::has_active_line())
                ObjectTree::remove_line_vertex(Coordinate(0,0));
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_Return:
            ObjectTree::finish_line();
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    case DRAG_COMPONENT:
        switch(key)
        {
        case GDK_KEY_Escape:
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    case ROTATE_COMPONENT:
        switch(key)
        {
        case GDK_KEY_Escape:
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    case FLIP_COMPONENT_LR:
        switch(key)
        {
        case GDK_KEY_Escape:
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    case FLIP_COMPONENT_UD:
        switch(key)
        {
        case GDK_KEY_Escape:
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    case DROP_COMPONENT:
        switch(key)
        {
        case GDK_KEY_Escape:
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    case ADD_TEXT:
        switch(key)
        {
        case GDK_KEY_Escape:
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    case MODIFY_TEXT:
        switch(key)
        {
        case GDK_KEY_Escape:
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    case ROTATE_COMPONENT_CW:
        switch(key)
        {
        case GDK_KEY_Escape:
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    case ROTATE_COMPONENT_CCW:
        switch(key)
        {
        case GDK_KEY_Escape:
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    case DELETE:
        switch(key)
        {
        case GDK_KEY_Escape:
            change_tool(NO_TOOL);
            change_state(POINTER);
            break;
        case GDK_KEY_w:
            change_tool(DRAW_WIRE);
            break;
        case GDK_KEY_Delete:
            change_tool(DELETE);
            break;
        }
        break;
    default:
        break;
    }
}
*/
