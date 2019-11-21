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
#include <app/object_tree.h>
#include <app/gtkspice_state.h>


// FOR TESTING
#include <app/draw_primitives.h>
#include <app/object_symbol.h>
#include <app/object.h>
#include <fstream>
#include <app/ltspice_symbol_parser.h>

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
    if(_objecttree)
        _objecttree->redraw(context);

    
    /******** TESTING ********/
    DrawSettings ds; // Use defaults
    ds.font_size = 5;

    /*
    std::shared_ptr<LinePrimitive> line1 = std::make_shared<LinePrimitive>();
    line1->start(Coordinate(-5,-15));
    line1->end(Coordinate(-5,15));
    std::shared_ptr<LinePrimitive> line2 = std::make_shared<LinePrimitive>();
    line2->start(Coordinate(-5,-15));
    line2->end(Coordinate(20,0));
    std::shared_ptr<LinePrimitive> line3 = std::make_shared<LinePrimitive>();
    line3->start(Coordinate(-5,15));
    line3->end(Coordinate(20,0));

    std::shared_ptr<RectPrimitive> rect1 = std::make_shared<RectPrimitive>();
    rect1->anchor(Coordinate(-3,5));
    rect1->height(3);
    rect1->width(3);
    std::shared_ptr<RectPrimitive> rect2 = std::make_shared<RectPrimitive>();
    rect2->anchor(Coordinate(-3,-5));
    rect2->height(-3);
    rect2->width(3);

    std::shared_ptr<ArcPrimitive> arc1 = std::make_shared<ArcPrimitive>();
    arc1->center(Coordinate(5,0));
    arc1->radius(3);
    arc1->set_start_angle_degrees(-90);
    arc1->set_end_angle_degrees(90);

    std::shared_ptr<CirclePrimitive> circ1 = std::make_shared<CirclePrimitive>();
    circ1->center(Coordinate(18,0));
    circ1->radius(3);
    std::shared_ptr<CirclePrimitive> circ2 = std::make_shared<CirclePrimitive>();
    circ2->center(Coordinate(21,0));
    circ2->radius(3);

    std::shared_ptr<TextPrimitive> text1 = std::make_shared<TextPrimitive>();
    text1->text("U1");
    text1->anchor(Coordinate(0,2));

    // Make geometry, pins, set position

    ObjectGeometry sym_geom;
    sym_geom.push_back(line1);
    sym_geom.push_back(line2);
    sym_geom.push_back(line3);
    sym_geom.push_back(rect1);
    sym_geom.push_back(rect2);
    sym_geom.push_back(arc1);
    sym_geom.push_back(circ1);
    sym_geom.push_back(circ2);
    sym_geom.push_back(text1);
    */
    
    Coordinate sym_pos(30,10);


    //GtkSpiceObject opamp;
    //opamp.index = 0;
    //opamp.active = false;
    //opamp.position = sym_pos;
    //opamp.symbol = opamp_sym;
    
    std::ifstream sfilestrm("/home/sam/Documents/Electronics/SPICE/lib/sym/Opamps/LM308.asy",std::fstream::in);
    LTSpiceSymbolParser parser(sfilestrm);
    if(parser.parse() == 0)
    {
        ObjectGeometry sym_geom = parser.get_geometry();
        ObjectPins sym_pins = parser.get_pins();
        std::shared_ptr<ObjectSymbol> test_sym = std::make_shared<ObjectSymbol>(sym_geom,sym_pins,sym_pos);
        test_sym->draw(context);
    }
    else
    {
        std::cout << "Encountered error while reading file.\n";
    }

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
