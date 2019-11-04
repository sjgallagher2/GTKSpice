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
#include <app/component_params.h>
#include <app/coordinate.h>
#include <app/key_accel_map.h>
#include <iostream>


Canvas::Canvas(std::shared_ptr<Window> toplevel, 
    std::shared_ptr<ObjectTree> ot, 
    std::shared_ptr<ActionFactory> af,
    std::shared_ptr<KeyAccelMap> keymap) : 
    _toplevel(toplevel),
    _objecttree(ot),
    _state( std::make_shared<GtkSpiceState>() ),
    _actionfactory(af),
    _cs( std::make_shared<CoordinateSystem>() )
{
    send_test = true;
    _ebox = std::make_shared<DrawingEventBox>(_cs);
    _toolmgr = std::make_shared<ToolManager>(_cs);
    // Set the object tree for the view
    _ebox->set_object_tree(_objecttree);
    // Add the event box for this canvas to the toplevel window and show
    _toplevel->add(*_ebox);
    _ebox->show();

    // Create default Pointer tool
    _state->active_tool(_toolmgr->get_tool(POINTER));

    // Initialize key accelerators
    _keymap = keymap;

    // Events
    _ebox->button_click().connect(sigc::mem_fun(*this,&Canvas::send_test_action));
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
    _state->active_tool()->tool_click_handler(mousepos,button,modifier,cselect);
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
    // CANVAS KEY ACCELERATORS
    ActionType action = _keymap->get_action((KeyModifiers)(modifier),key);
    if(action != NO_ACTION)
    {
        _new_action.emit(_actionfactory->make_action(action));
    }
    
    _state->active_tool()->tool_key_handler(key,modifier);
    _ebox->force_redraw();
}


void Canvas::send_test_action(Coordinate x,int y,int z,int t)
{
    if(send_test)
    {
        LineParameters lp;
        lp.cp.editable = false;
        lp.cp.filled = false;
        lp.cp.selectable = false;
        lp.cp.label = "Test line";
        lp.cp.stroke_thickness = 1;
        VertexList verts;
        verts.push_back(std::make_shared<Vertex>(0,0));
        verts.push_back(std::make_shared<Vertex>(10,10));
        lp.vertices = verts;
        std::cout << "Emitting new test action command for append line...\n";
        _new_action.emit(_actionfactory->make_action(APPEND_LINE,lp));
        send_test = false;
    }
}
