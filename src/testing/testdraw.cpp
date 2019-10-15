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

#include <gtkmm.h>
#include <iostream>
#include <cairomm/context.h>
#include <testing/testdraw.h>
#include <librsvg-2.0/librsvg/rsvg.h>

TestDraw::TestDraw() 
{ 
}

TestDraw::~TestDraw()
{
}

void TestDraw::draw_something(const Cairo::RefPtr<Cairo::Context>& context)
{
    GError** e = NULL;
    RsvgHandle* svgin = rsvg_handle_new_from_file("../src/testing/Res-test.svg",e);
    if(svgin == NULL)
    {
        std::cout << "Error: File not found.\n";
    }
    else
    {
        std::cout << "File found... \n";
        RsvgDimensionData dim;
        rsvg_handle_get_dimensions(svgin,&dim);
        std::cout << "SVG Size: " << dim.width << " x " << dim.height << "\n";
    }
    // Cairomm wraps the Cairo base class cairo_t, and rsvg can render to a 
    // cairo_t, so we need to move it over

    context->save();
    Cairo::Context::cobject* crtemp = context->cobj();
    rsvg_handle_render_cairo_sub(svgin,crtemp,"#layer1");
    context->restore();
    /*
    if(
    {
        //Cairo::Context context_temp(crtemp);
    }
    else
    {
        std::cout << "Object index not found...\n";
    }*/
    


    context->save();
    context->set_line_width(2);
    context->set_line_cap(Cairo::LINE_CAP_BUTT);

    Coordinate anchor(10,10);
    context->move_to(anchor.x()-5,anchor.y()+0);
    context->line_to(anchor.x()+5,anchor.y()+0);
    context->move_to(anchor.x()+0,anchor.y()-5);
    context->line_to(anchor.x()+0,anchor.y()+5);

    context->stroke();
    context->restore();
    //g_object_unref(svgin);
}
