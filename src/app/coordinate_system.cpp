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

CoordinateSystem::CoordinateSystem() : _center(0,0),_scale(4.0)
{
}

Cairo::Matrix CoordinateSystem::get_view_matrix(int width, int height)
{
    // Return a transformation matrix for a view
    double dx = width/2 + _center.x();
    double dy = height/2 + _center.y();
    Cairo::Matrix vmat(1,0,0,1,0,0);
    vmat.translate(dx,dy);
    vmat.scale(_scale,_scale);
    Cairo::Matrix vmatinv = vmat;
    vmatinv.invert();

    return vmat;
}
/*
void CoordinateSystem::set_to_user_coordinates(Coordinate& c)
{
    double xx,yy;
    xx = c.x();
    yy = c.y();
    _tmatinv.transform_point(xx,yy);
    c.set_coordinate(xx,yy);
}
void CoordinateSystem::set_to_device_coordinates(Coordinate& c)
{
    double xx,yy;
    xx = c.x();
    yy = c.y();
    _tmat.transform_point(xx,yy);
    c.set_coordinate(xx,yy);
}
void CoordinateSystem::set_to_user_distance(Coordinate& c)
{
    double xx,yy;
    xx = c.x();
    yy = c.y();
    _tmatinv.transform_distance(xx,yy);
    c.set_coordinate(xx,yy);
}
void CoordinateSystem::set_to_device_distance(Coordinate& c)
{
    double xx,yy;
    xx = c.x();
    yy = c.y();
    _tmat.transform_distance(xx,yy);
    c.set_coordinate(xx,yy);
}
*/
void CoordinateSystem::pan(Coordinate delta)
{
    _center.x(_center.x() + delta.x());
    _center.y(_center.y() + delta.y());
}

void CoordinateSystem::zoom_in(Coordinate anchor)
{
    if(_scale*_scale_factor < MAX_ZOOM_IN)
    {
        _scale = _scale*_scale_factor;
    }
    Coordinate disp = Transforms::anchored_scale_displacement(anchor,_scale,_scale_factor,true);
    pan(disp);
}

void CoordinateSystem::zoom_out(Coordinate anchor)
{
    if(_scale/_scale_factor > MAX_ZOOM_OUT)
    {
        _scale = _scale/_scale_factor;
    }
    Coordinate disp = Transforms::anchored_scale_displacement(anchor,_scale,_scale_factor,false);
    pan(disp);
}

