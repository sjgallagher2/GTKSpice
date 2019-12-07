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

// A class which presents a virtual view of the coordinate space

/* The View class inherits from the GTK DrawingArea class, so it can
 * be used in the same way as a DrawingArea. It uses Cairo to present
 * a view to the user of the layout of their schematic. 
 * 
 * The view class has on_draw() which is the main drawing function so
 * it needs to pass the context to the object_tree and those objects
 * all have their draw() function called in order. 
 * 
 */

#ifndef GTKSPICE_VIEW_H
#define GTKSPICE_VIEW_H

#include <memory>
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gui/view_features.h>
#include <app/gtkspice_object_list.h>
#include <app/coordinate_system.h>
#include <app/coordinate.h>
#include <gui/grid.h>
#include <testing/debug_text.h>
#include <app/action_stack.h>

class DrawingEventBox;

class View : public Gtk::DrawingArea
{
public:
    View(std::shared_ptr<CoordinateSystem> cs);
    virtual ~View();

    int visible_units_x();
    int visible_units_y();

    //CoordinateSystem get_coordinate_system() {return &_cs;}
    Cairo::Matrix tmatrix() {return _tmat;}
    void set_view_features(std::shared_ptr<ViewFeatures> os) {_vfeatures = os;}

    //Glib::RefPtr<Gdk::Cursor> get_cursor();

    // These calculate the left-, right-, top-, and bottom-most coordinates visible
    int get_left();
    int get_right();
    int get_top();
    int get_bottom();

    void force_redraw();

private:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& context);
    bool on_new_size(GdkEventConfigure* event_configure);
    bool on_scroll_event(GdkEventScroll* scroll_event);
    bool on_button_press(GdkEventButton* button_event);
    bool on_button_release(GdkEventButton* button_event);
//    bool pan(GdkEventMotion* movement_event);


    Cairo::RefPtr<Cairo::Context> _context;
    std::shared_ptr<ViewFeatures> _vfeatures;
    Cairo::Matrix _tmat;

    std::shared_ptr<CoordinateSystem> _cs;
    Grid _grid;
    DebugText _dt;
    Coordinate _mouse_pos_d;
};

#endif /* GTKSPICE_VIEW_H */