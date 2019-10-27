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

#ifndef VIEW_H
#define VIEW_H

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <app/coordinate_system.h>
#include <app/coordinate.h>
#include <gui/grid.h>
#include <testing/debug_text.h>
#include <app/action_stack.h>

class View : public Gtk::DrawingArea
{
public:
    View();
    virtual ~View();

    int visible_units_x();
    int visible_units_y();

    CoordinateSystem* get_coordinate_system() {return &_cs;}

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
    bool pan(GdkEventMotion* movement_event);

    Cairo::RefPtr<Cairo::Context> _context;

    CoordinateSystem _cs;
    Grid _grid;
    DebugText _dt;
    bool _pan = false;
    bool _zoom_in = false;
    bool _zoom_out = false;
    Coordinate _pan_anchor_d; // Anchor for pan (device coordinates)
    Coordinate _delta_d; // Net change in position (device coordinates)
    Coordinate _pan_delta_d; // Change in position for current pan (device coorindates)
    float _scale = 4;
    float _scale_factor = 1.5; // Multiplication factor for zooming in and out
    const float MAX_ZOOM_IN = 25;
    const float MAX_ZOOM_OUT = 0.6;
    Coordinate _scale_anchor_d; // Scale anchor (device coordinates)
    Coordinate _mouse_pos_d;
    Glib::RefPtr<Gdk::Cursor> _crosshairs_cursor;
    Glib::RefPtr<Gdk::Cursor> _pointer_cursor;
    Glib::RefPtr<Gdk::Cursor> _grabbing_cursor;
    Glib::RefPtr<Gdk::Cursor> _scissors_cursor;
};

#endif /* VIEW_H */