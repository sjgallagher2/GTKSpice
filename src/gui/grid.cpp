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
#include <gui/grid.h>
#include <app/coordinate.h>

Grid::Grid() : _radius(1.5)
{ 
}

Grid::~Grid()
{
}

void Grid::draw_origin(const Cairo::RefPtr<Cairo::Context>& context, float scale)
{
    context->save();
    float s = 1;
    context->set_line_width(s);
    context->set_line_cap(Cairo::LINE_CAP_BUTT);

    context->move_to(-5,0);
    context->line_to(5,0);
    context->move_to(0,-5);
    context->line_to(0,5);

    context->stroke();
    context->restore();
}

void Grid::draw_grid(const Cairo::RefPtr<Cairo::Context>& context,const float scale,
                    const int left, const int right, const int bottom, const int top)
{
    /* Draw grid dots */
    context->save();
    float sf = 2; // Dot scale factor
    float s = sf/scale;
    float inc = 10; // Dot spacing
    if(scale > 6)
    {
        inc = 1; // Smaller dot spacing for smaller grid
    }
    Coordinate::grid(inc);

    //context->set_line_width(_radius);
    //context->set_line_cap(Cairo::LINE_CAP_ROUND);
    
    // This draws the dots, starting at the origin, and going out
    for(int y = 0; y < top; y+= inc)
    {
        for(int x = 0; x < right; x+=inc)
            if(x != 0 || y != 0)
                context->rectangle(x-s/2, y-s/2,s,s);
        for(int x = 0; x > left; x -= inc)
            if(x != 0 || y != 0)
                context->rectangle(x-s/2, y-s/2,s,s);
    }
    for(int y = 0; y > bottom; y-= inc)
    {
        for(int x = 0; x < right; x+=inc)
            if(x != 0 || y != 0)
                context->rectangle(x-s/2, y-s/2,s,s);
        for(int x = 0; x > left; x -= inc)
            if(x != 0 || y != 0)
                context->rectangle(x-s/2, y-s/2,s,s);
    }
    // Note: You can't assume a symmetric grid, hence the many for loops
    // Note: The if's remove the origin point
    
    float shade = 0.5;
    context->set_source_rgba(shade,shade,shade,1.0);
    context->fill();

    context->restore();

}