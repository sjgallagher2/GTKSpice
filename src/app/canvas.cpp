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

#include <app/vertex_list.h>
#include <app/canvas.h>
#include <app/coordinate.h>
#include <app/key_accel_map.h>
#include <iostream>


Canvas::Canvas(std::shared_ptr<Window> toplevel, 
    std::shared_ptr<ActionFactory> af,
    std::shared_ptr<KeyAccelMap> keymap) : 
    _toplevel(toplevel),
    _state( std::make_shared<GtkSpiceState>() ),
    _actionfactory(af),
    _cs( std::make_shared<CoordinateSystem>() ),
    _vfeatures( std::make_shared<ViewFeatures>() )
{
    send_test = true;
    _ebox = std::make_shared<DrawingEventBox>(_cs);
    _toolmgr = std::make_shared<ToolManager>(_actionfactory,_cs,_vfeatures);
    // Set the object tree for the view
    _ebox->set_view_features(_vfeatures);
    // Add the event box for this canvas to the toplevel window and show
    _toplevel->add(*_ebox);
    _ebox->show();

    // Create default Pointer tool
    _state->active_tool(_toolmgr->get_tool(POINTER));

    // Initialize key accelerators
    _keymap = keymap;

    // Events
    _ebox->button_click().connect(sigc::mem_fun(*this, &Canvas::click_handler));
    _ebox->mouse_move().connect(sigc::mem_fun(*this, &Canvas::move_handler));
    _ebox->key_press().connect(sigc::mem_fun(*this, &Canvas::key_handler));
    _ebox->scroll_wheel().connect(sigc::mem_fun(*this,&Canvas::scroll_handler));
    _toplevel->signal_key_press_event().connect(sigc::mem_fun(*_ebox,&DrawingEventBox::on_key_press_event));
    _toplevel->signal_button_press_event().connect(sigc::mem_fun(*_ebox, &DrawingEventBox::on_button_press_event));
    _toplevel->signal_button_release_event().connect(sigc::mem_fun(*_ebox, &DrawingEventBox::on_button_release_event));
    _toplevel->signal_motion_notify_event().connect(sigc::mem_fun(*_ebox, &DrawingEventBox::on_mouse_move_event));
    _toplevel->signal_scroll_event().connect(sigc::mem_fun(*_ebox, &DrawingEventBox::on_scroll_wheel_event));
}

Canvas::~Canvas()
{
}

void Canvas::click_handler(Coordinate mousepos, int button, int modifier, int cselect)
{
    std::shared_ptr<Action> click_action = _state->active_tool()->tool_click_handler(mousepos,button,modifier,cselect);
    if(click_action)
        _new_action.emit(click_action);
    update_cursor();
    _ebox->force_redraw();
}
void Canvas::scroll_handler(Coordinate mousepos, int scroll_dir)
{
    if(scroll_dir == SCROLL_UP)
        _cs->zoom_in(mousepos);
    else if(scroll_dir == SCROLL_DOWN)
        _cs->zoom_out(mousepos);
    _ebox->force_redraw();
    
}
void Canvas::move_handler(Coordinate mousepos)
{
    _state->active_tool()->tool_move_handler(mousepos);
    _ebox->force_redraw();
}
void Canvas::key_handler(int key,int modifier)
{
    // TOOL KEY ACCELERATORS
    std::shared_ptr<Action> t_action = 
        _state->active_tool()->tool_key_handler(key,modifier);
    if(t_action)
        _new_action.emit(t_action);
        
    // CANVAS KEY ACCELERATORS
    ActionType e_action = _keymap->get_action((KeyModifiers)(modifier),key);
    if(e_action != NO_ACTION)
    {
        _new_action.emit(_actionfactory->make_action(e_action));
    }
    
    // Redraw
    update_cursor();
    _ebox->force_redraw();
}

void Canvas::update_cursor()
{
    _new_cursor.emit(_state->active_tool()->get_tool_cursor_name());
}
