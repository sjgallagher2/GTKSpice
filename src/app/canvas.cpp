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
#include <iostream>


// NOTE: You can pass a std::shared_ptr<ObjectTree> to this
Canvas::Canvas(std::shared_ptr<Window> toplevel, std::shared_ptr<ObjectTree> ot, std::shared_ptr<ActionFactory> af) : 
    _toplevel(toplevel),
    _objecttree(ot),
    _state( std::make_shared<GtkSpiceState>() ),
    _actionfactory(af),
    _ebox( std::make_shared<DrawingEventBox>())
{
    send_test = true;
    _ebox->set_object_tree(_objecttree);
    _toplevel->add(*_ebox);
    _ebox->show();
    // TODO Where to get DrawingEventBox and ViewFeatures?
    _ebox->button_click().connect(sigc::mem_fun(*this,&Canvas::send_test_action));
    _toplevel->signal_key_press_event().connect(sigc::mem_fun(*_ebox,&DrawingEventBox::on_key_press_event));
    _toplevel->signal_button_press_event().connect(sigc::mem_fun(*_ebox, &DrawingEventBox::on_button_press_event));
    _toplevel->signal_button_release_event().connect(sigc::mem_fun(*_ebox, &DrawingEventBox::on_button_release_event));
    _toplevel->signal_motion_notify_event().connect(sigc::mem_fun(*_ebox, &DrawingEventBox::on_mouse_move_event));
}

Canvas::~Canvas()
{
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
        std::cout << "Emitting new action command for append line...\n";
        _new_action.emit(_actionfactory->make_action(APPEND_LINE,lp));
        send_test = false;
    }
}