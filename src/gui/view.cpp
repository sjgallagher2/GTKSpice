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
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <app/coordinate_system.h>
#include <gui/view.h>
#include <gui/grid.h>
#include <app/transforms.h>
//#include <testing/debug_text.h>
#include <testing/testdraw.h>
#include <app/object_tree.h>
#include <app/gtkspice_state.h>

/* THE CAIRO COORDINATE SYSTEM
 * When the screen is drawn, on_draw() is called, which provides a context object. It
 * seems like you need to use this context instead of keeping one, which means all changes
 * need to occur in on_draw(). 
 * 
 * There are two coordinate systems: the device coordinate system, and the user coordinate
 * system. The device coordinate system is constant, and reflects the position in the window.
 * The user coordinate systme reflects the position in 'Cairo' space, and by extension
 * in the coordinate system of the drawing environment from a programming perspective. The
 * user coordinates define where drawings are placed.
 * 
 * Device coordinates: (0,0) in upper left
 * Device units: Constant, in pixels
 * User coordinate: (0,0) at origin
 * User units: Depends on scale factor
 * 
 * To create a navigable environment with pan and zoom, we have to keep track of the "deltas",
 * how much the user drags for a pan, as well as the zoom factor. In addition, we want to
 * zoom to the mouse position, not to the screen center. 
 * 
 * At the beginning of on_draw(), the device coordinates and user coordinates are the same. 
 * Nothing has been drawn yet. The first transformation moves (0,0) to the center of the window,
 * and adds the total delta (universal offset) and the current pan delta (for current mouse click).
 * (When the mouse is released, the current pan delta is merged with the total delta.) All deltas
 * are stored as device coordinates, which can be converted to user coordinates when necessary.
 * In this case, the device scale and user scale are the same, so it doesn't matter. When scaling/
 * zooming comes in, it will matter.
 * 
 * So the first transformation positions the user origin in the right position, without scale. 
 * There is only one scale operation in Cairo, which scales everything from the user origin. To
 * zoom into a particular coordinate, you first translate to center that coordinate, then scale,
 * then translate back. Each time scale() is called adds to the overall scale. A universal scale
 * is stored in _scale. Each zoom in increases the scale 1.5 times, and zoom out decreases by 1.5.
 * 
 * Scaling is triggered by a callback, and is handled by on_draw(). Normally, no new scaling has
 * occurred, so the scale is simply set to _scale. Because this is the first scaling operation, the
 * device and user factors are equivalent. 
 * 
 */

// TODO This needs to be rewritten for new structure

View::View() :
    _delta_d(0,0), _pan_anchor_d(0,0), _scale_anchor_d(0,0), _pan_delta_d(0,0),_mouse_pos_d(0,0)
{ 

    add_events(Gdk::SCROLL_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK);

    signal_button_press_event().connect(sigc::mem_fun(*this, &View::on_button_press));
    signal_button_release_event().connect(sigc::mem_fun(*this, &View::on_button_release));
    signal_scroll_event().connect(sigc::mem_fun(*this, &View::on_scroll_event));
    signal_motion_notify_event().connect(sigc::mem_fun(*this, &View::pan));

    _cs.snap_grid(true);
    
}

View::~View()
{ 
}
/*
Glib::RefPtr<Gdk::Cursor> View::get_cursor()
{
    Glib::ustring ctext = GtkSpiceState::get_cursor_name();
    if(ctext.compare("default") == 0)
    {
        if(!_pointer_cursor)
        {
            auto win = get_window();
            auto display = win->get_display();
            _pointer_cursor = Gdk::Cursor::create(display,"default");
        }
        return _pointer_cursor;
    }
    else if(ctext.compare("crosshair")==0)
    {
        if(!_crosshairs_cursor)
        {
            auto win = get_window();
            auto display = win->get_display();
            _crosshairs_cursor = Gdk::Cursor::create(display,"crosshair");
        }
        return _crosshairs_cursor;
    }
    else if(ctext.compare("grabbing")==0)
    {
        if(!_grabbing_cursor)
        {
            auto win = get_window();
            auto display = win->get_display();
            _grabbing_cursor = Gdk::Cursor::create(display,"grabbing");
        }
        return _grabbing_cursor;
    }
    else if(ctext.compare("scissors")==0)
    {
        if(!_scissors_cursor)
        {
            auto win = get_window();
            auto display = win->get_display();
            auto pb = Gdk::Pixbuf::create_from_file("/home/sam/Documents/Devel/Cpp/GTKSpice/data/media/scissor-cursor-32.png");
            _scissors_cursor = Gdk::Cursor::create(display,pb,14,8);
        }
        return _scissors_cursor;
    }
}
*/

void View::force_redraw()
{
    auto win = get_window();
    if (win)
    {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
}

bool View::on_draw(const Cairo::RefPtr<Cairo::Context>& context)
{
    // Cairo context from event, automatically generated
    Gtk::Allocation alloc = get_allocation();
    const int width = alloc.get_width();
    const int height = alloc.get_height();

    double offsetx = width/2 + _delta_d.x() + _pan_delta_d.x();
    double offsety = height/2 + _delta_d.y() + _pan_delta_d.y();

    context->translate(offsetx, offsety);

    if(_zoom_in) // New zoom occurred
    {
        Coordinate dspt = Transforms::anchored_scale(context,_scale_anchor_d,_scale,_scale_factor,true);
        _delta_d.x(_delta_d.x()+dspt.x());
        _delta_d.y(_delta_d.y()+dspt.y());
        _zoom_in = false;
    }
    else if(_zoom_out)
    {
        Coordinate dspt = Transforms::anchored_scale(context,_scale_anchor_d,_scale,_scale_factor,false);
        _delta_d.x(_delta_d.x()+dspt.x());
        _delta_d.y(_delta_d.y()+dspt.y());
        _zoom_out = false;
    }
    else
    {
        context->scale(_scale,_scale);
    }
    context->set_source_rgba(0,0,0,1.0);
    
    // Update coordinate system transformation
    _cs.set_tmatrix(context->get_matrix());

    /* Draw origin */
    _grid.draw_origin(context, _scale);
    
    /* Draw grid */
    // First get view area in user coordinates
    Coordinate org(0,0);
    Coordinate viewarea(width,height);
    org.set_to_user_coordinate(context);
    viewarea.set_to_user_coordinate(context);
    // Now use those to set grid bounds
    _grid.draw_grid(context, _scale,org.x(),viewarea.x(),org.y(),viewarea.y());

    // Draw all objects in the object tree
    if(_objecttree)
    {
        _objecttree->redraw(context);
    }

    // All done
    return true;

}


bool View::on_new_size(GdkEventConfigure* event_configure)
{
}

bool View::on_scroll_event(GdkEventScroll* scroll_event)
{
    Gtk::Allocation alloc = get_allocation();
    const int width = alloc.get_width();
    const int height = alloc.get_height();
    
    // Handler for scrolling (zooming)
    if(scroll_event->direction == GDK_SCROLL_UP)
    {
        if(_scale*_scale_factor < MAX_ZOOM_IN)
        {
            _scale = _scale*_scale_factor;
            _zoom_in = true;
        }
    }
    else if(scroll_event->direction == GDK_SCROLL_DOWN)
    {
        if(_scale/_scale_factor > MAX_ZOOM_OUT)
        {
            _scale = _scale/_scale_factor;
            _zoom_out = true;
        }
    }

    //Use screen (device) position to set scale anchor
    _scale_anchor_d.set_coordinate(scroll_event->x, scroll_event->y);
    
    // Force redraw
    force_redraw();
    return false;
}

bool View::on_button_press(GdkEventButton* button_event)
{
/*
    if(button_event->button == 1 && GtkSpiceState::get_state() != GtkSpiceState::TOOL)
    {
        // Use screen (device) location to set pan anchor
        _pan_anchor_d.set_coordinate(button_event->x, button_event->y);
    }

    //_mouse_pos_d.set_coordinate(button_event->x,button_event->y);
    //_send_mouse_click = true;
*/
    // Force redraw
    force_redraw();
    return false;
}

bool View::on_button_release(GdkEventButton* button_event)
{
    if(button_event->button == 1)
    {
        _delta_d.x(_delta_d.x() + _pan_delta_d.x());
        _delta_d.y(_delta_d.y() + _pan_delta_d.y());
        _pan_delta_d.set_coordinate(0,0);
        //std::cout << "Reset\n";
    }

    //std::cout << "Release\n";

    // Force redraw
    force_redraw();
    return false;
}

bool View::pan(GdkEventMotion* movement_event)
{
    // TODO All instances of state handling must be updated
    _mouse_pos_d.set_coordinate(movement_event->x,movement_event->y);
/*
    if(GtkSpiceState::get_state() == GtkSpiceState::PAN)
    {
        Gtk::Allocation alloc = get_allocation();
        const int width = alloc.get_width();
        const int height = alloc.get_height();

        // Use screen (device) position to set pan distance for x and y
        double dx,dy;
        dx = movement_event->x - _pan_anchor_d.x();
        dy = movement_event->y - _pan_anchor_d.y();
        _pan_delta_d.set_coordinate(dx, dy);

        // Force redraw
        auto win = get_window();
        if (win)
        {
            Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                    get_allocation().get_height());
            win->invalidate_rect(r, false);
        }
        return false;
    }
*/
    force_redraw();
    return false;
}
