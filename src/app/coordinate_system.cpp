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
