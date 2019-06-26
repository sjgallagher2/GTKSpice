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

#ifndef COORDINATE_H
#define COORDINATE_H

// A coordinate class used for storing screen and application coordinate points
// Coordinates are always stored as doubles
class Coordinate
{
public:
    Coordinate() {};
    Coordinate(double X, double Y) : _x(X), _y(Y) {};
    virtual ~Coordinate() {};

    void set_to_user_coordinate(const Cairo::RefPtr<Cairo::Context>& context);
    void set_to_user_distance(const Cairo::RefPtr<Cairo::Context>& context);
    void set_to_device_coordinate(const Cairo::RefPtr<Cairo::Context>& context);
    void set_to_device_distance(const Cairo::RefPtr<Cairo::Context>& context);
    void set_coordinate(double X, double Y) { _x = X; _y = Y; };

    /* Sets and Gets */
    void x(const double X) { _x = X; }
    void y(const double Y) { _y = Y; }

    double x()  const { return _x; }
    double y() const { return _y; }

protected:
    double _x;
    double _y;

};

#endif /* COORDINATE_H */