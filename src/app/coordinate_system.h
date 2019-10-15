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

// Class for storing the in-app coordinate system

#ifndef COORDINATE_SYSTEM_H
#define COORDINATE_SYSTEM_H

#include <cairomm/matrix.h>
#include <app/coordinate.h>

class CoordinateSystem
{
public:
    CoordinateSystem() {};
    virtual ~CoordinateSystem() {};

    void set_tmatrix(const Cairo::Matrix m) {_tmat = m;_tmatinv=_tmat;_tmatinv.invert();}

    void set_to_user_coordinates(Coordinate& c);
    void set_to_device_coordinates(Coordinate& c);
    void set_to_user_distance(Coordinate& c);
    void set_to_device_distance(Coordinate& c);

    void snap_grid(bool s) {_snap_grid = s;}
    bool snap_grid() const {return _snap_grid;}
protected:
    bool _snap_grid = false;

    Cairo::Matrix _tmat;
    Cairo::Matrix _tmatinv;
};

#endif /* COORDINATE_SYSTEM_H */