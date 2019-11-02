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

#include <gtkmm/drawingarea.h>
#include <app/coordinate.h>
#include <app/coordinate_system.h>
#include <app/transforms.h>
#include <iostream>

// TODO Not sure why you need to multiply by scale, set_to_device_distance
// should do it for us in tool_move_handler in PointerTool, but it doesn't.

CoordinateSystem::CoordinateSystem() : _center(0,0),_scale(6.0)
{
}

Cairo::Matrix CoordinateSystem::get_view_matrix(int width, int height)
{
    // Return a transformation matrix for a view
    double x0 = _center.x()+width/2 + _scale*_pan_delta.x();
    double y0 = _center.y()+height/2 + _scale*_pan_delta.y();
    Cairo::Matrix vmat(_scale,0,0,_scale,x0,y0);
//    vmat.translate(dx,dy);
//    vmat.scale(_scale,_scale);

    return vmat;
}
void CoordinateSystem::translate(Coordinate delta)
{
    _center.x(_center.x() + delta.x());
    _center.y(_center.y() + delta.y());
}
void CoordinateSystem::finish_pan()
{
    _center.x(_center.x() + _scale*_pan_delta.x());
    _center.y(_center.y() + _scale*_pan_delta.y());
    _pan_delta.x(0);
    _pan_delta.y(0);
}

void CoordinateSystem::zoom_in(Coordinate anchor)
{
    if(_scale*_scale_factor < MAX_ZOOM_IN)
    {
        _scale = _scale*_scale_factor;
    }
    Coordinate disp = Transforms::anchored_scale_displacement(anchor,_scale,_scale_factor,true);
    translate(disp);
}

void CoordinateSystem::zoom_out(Coordinate anchor)
{
    if(_scale/_scale_factor > MAX_ZOOM_OUT)
    {
        _scale = _scale/_scale_factor;
    }
    Coordinate disp = Transforms::anchored_scale_displacement(anchor,_scale,_scale_factor,false);
    translate(disp);
}

