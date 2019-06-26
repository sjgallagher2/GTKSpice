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
/*
#include <gtkmm.h>
#include <app/coordinate.h>
#include <app/screen_coordinate.h>
#include <app/app_coordinate.h>
#include <app/coordinate_system.h>
#include <iostream>
#include <cmath>

class CoordinateSystem
{
public:
    CoordinateSystem() : _ppg(10), _zoom(1), _center(0,0) {};
    virtual ~CoordinateSystem() {};

    void pixels_per_grid(const double ppg) {_ppg = ppg; }
    void zoom(const double z) { _zoom = z; }
    void zoom_in();
    void zoom_in(Coordinate mouse_pos);
    void zoom_out();
    void zoom_out(Coordinate mouse_pos);
    void center(Coordinate c);

    double pixels_per_grid() const { return _ppg; }
    double zoom() const { return _zoom; }
    Coordinate center() const { return _center; }
    double center_x_app() const { return _center.x(); }
    double center_y_app() const { return _center.y(); }
    double center_x_screen();
    double center_y_screen();

    void snap_to_grid(Coordinate & c);
    void coordinate_to_screen(Coordinate & c);
    Coordinate get_coordinate_to_screen(Coordinate c);
    void screen_to_coordinate(Coordinate & c);
    Coordinate get_screen_to_coordinate(Coordinate c);

protected:
    double _ppg; // Pixels per grid space
    double _zoom; // Zoom factor
    Coordinate _center; // Center of view in app coordinates
};

void CoordinateSystem::zoom_in()
{
    if(_zoom < 0)
        _zoom = 0;
    else if(_zoom == 0)
        _zoom = 1;
    else if(_zoom >= 1)
        _zoom++;
    else if(_zoom < 1)
        _zoom*=2;
}
void CoordinateSystem::zoom_in(Coordinate mouse_pos)
{
    if(_zoom < 0)
        _zoom = 1;
    else if(_zoom == 0)
        _zoom = 1;
    else if(_zoom >= 1)
        _zoom++;
    else if(_zoom < 1)
        _zoom*=2;
}

void CoordinateSystem::zoom_out()
{
    if(_zoom <= 0)
        _zoom = 0;
    else if(_zoom > 1)
        _zoom--;
    else if(_zoom <= 1)
        _zoom/=2;
}

void CoordinateSystem::zoom_out(Coordinate mouse_pos)
{
    if(_zoom <= 0)
        _zoom = 0;
    else if(_zoom > 1)
        _zoom--;
    else if(_zoom <= 1)
        _zoom/=2;
}

void CoordinateSystem::center(Coordinate c)
{
    _center = c;
}


void CoordinateSystem::snap_to_grid(Coordinate & c)
{
    // Get fractional and integral parts of x/sep and y/sep
    double xsepint, xsepfrac, ysepint, ysepfrac, mx, my;

    // Get negatives    
    if(c.x()<0) mx = -1;
    else mx = 1;
    
    if(c.y() < 0) my = -1;
    else my = 1;

    // Get fractional and integral parts
    xsepfrac = std::modf(c.x(), &xsepint);
    ysepfrac = std::modf(c.y(), &ysepint);

    // Convert everything to absolute values for rounding
    xsepint = std::abs(xsepint);
    ysepint = std::abs(ysepint);
    xsepfrac = std::abs(xsepfrac);
    ysepfrac = std::abs(ysepfrac);

    // Fix the x coordinate offset
    if(xsepfrac < 0.5)
        c.x( c.x() - xsepfrac*mx );
    else
        c.x( c.x() + (1 - xsepfrac)*mx );
    
    // Fix the y coordinate offset
    if(ysepfrac < 0.5)
        c.y( c.y() - ysepfrac*my );
    else
        c.y( c.y() + (1 - ysepfrac)*my );

}

double CoordinateSystem::center_x_screen()
{
    Coordinate c(0,0);
    coordinate_to_screen(c);
    return c.x();
}

double CoordinateSystem::center_y_screen()
{
    Coordinate c(0,0);
    coordinate_to_screen(c);
    return c.y();
}

void CoordinateSystem::coordinate_to_screen(Coordinate& c)
{
    // Uses grid spacing (ppg), zoom, and center to convert
    // grid coordinates to screen coordinates
    //std::cout << "Converting coordinate: (" << c.x() << ", " << c.y() << ") to screen position.\n";
    //std::cout << "Center: (" << _center.x() << ", " << _center.y() << ") \n";
    c.x( c.x()*_ppg*_zoom + _center.x()*_ppg*_zoom );
    c.y( -(c.y()*_ppg*_zoom + _center.y()*_ppg*_zoom) );
}

Coordinate CoordinateSystem::get_coordinate_to_screen(const Coordinate& c)
{
    Coordinate out(c.x(), c.y());
    out.x( out.x()*_ppg*_zoom + _center.x()*_ppg*_zoom );
    out.y( -(out.y()*_ppg*_zoom + _center.y()*_ppg*_zoom) );

    return out;
}

void CoordinateSystem::screen_to_coordinate(Coordinate& c)
{
    // Uses grid spacing (ppg), zoom, and center to convert
    // screen coordinates to grid coordinates
    c.x( (c.x()-center_x_screen())/_ppg/_zoom );
    c.y( -(c.y()-center_y_screen())/_ppg/_zoom ); // Flip y sign

}

Coordinate CoordinateSystem::get_screen_to_coordinate(const Coordinate& c)
{
    Coordinate out(c.x(),c.y());
    out.x( (out.x()-center_x_screen())/_ppg/_zoom );
    out.y( -(out.y()-center_y_screen())/_ppg/_zoom ); // Flip y sign

    return out;
}
*/
