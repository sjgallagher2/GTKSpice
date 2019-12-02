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

#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <app/coordinate_system.h>
#include <gui/view.h>
#include <gui/grid.h>
#include <app/transforms.h>
#include <testing/testdraw.h>
#include <app/gtkspice_state.h>

#include <iostream>


// FOR TESTING
#include <app/draw_primitives.h>
#include <app/object_symbol.h>
#include <app/gtkspice_object.h>

/* 
 * When the screen is drawn, on_draw() is called, which provides a context object. 
 * You need to use this context instead of keeping one, which means all changes
 * need to occur in on_draw(). 
 * 
 */


View::View(std::shared_ptr<CoordinateSystem> cs) :
    _cs(cs)
{
    add_events(Gdk::SCROLL_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK);

    signal_button_press_event().connect(sigc::mem_fun(*this, &View::on_button_press));
    signal_button_release_event().connect(sigc::mem_fun(*this, &View::on_button_release));
    signal_scroll_event().connect(sigc::mem_fun(*this, &View::on_scroll_event));

    _cs->snap_grid(true);
    
}

View::~View()
{ 
}

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
    Gtk::Allocation alloc = get_allocation();
    const int width = alloc.get_width();
    const int height = alloc.get_height();

    _tmat = _cs->get_view_matrix(width,height);
    context->set_matrix(_tmat);
    context->set_source_rgba(0,0,0,1.0);
    
    /* Draw origin */
    _grid.draw_origin(context, _cs->scale());
    
    /* Draw grid */
    // First get view area in user coordinates
    Coordinate org(0,0);
    Coordinate viewarea(width,height);
    org.set_to_user_coordinate(context);
    viewarea.set_to_user_coordinate(context);
    // Now use those to set grid bounds
    _grid.draw_grid(context, _cs->scale(),org.x(),viewarea.x(),org.y(),viewarea.y());

    // Draw ViewFeatures objects
    if(_vfeatures)
        _vfeatures->draw_features(context);

    // Draw all objects in the object tree
    if(_elementmap)
        _elementmap->redraw(context);

    
    /******** TESTING ********/

    DrawSettings ds; // Use most defaults
    ds.font_size = 5;
    
    Coordinate o_pos(30,10);
    GtkSpiceElement obj("/home/sam/Documents/Electronics/SPICE/lib/sym/ind.asy");
    obj.set_name("123");
    obj.set_position(o_pos);
    obj.rotate90();
    obj.rotate90();
    obj.hflip();
    obj.vflip();
    obj.draw(context);
    /*************************/

    // All done
    return true;

}


bool View::on_new_size(GdkEventConfigure* event_configure)
{
    return false;
}

bool View::on_scroll_event(GdkEventScroll* scroll_event)
{
    return false;
}

bool View::on_button_press(GdkEventButton* button_event)
{
    return false;
}

bool View::on_button_release(GdkEventButton* button_event)
{
    return false;
}
