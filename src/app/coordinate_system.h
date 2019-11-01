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


#ifndef COORDINATE_SYSTEM_H
#define COORDINATE_SYSTEM_H

#include <cairomm/matrix.h>
#include <app/coordinate.h>

// Class for storing the in-app coordinate system
/* USAGE: 
 * This class is the interface between the device coordinates and the
 * user/developer coordinates. The transformations are (in order):
 *  1. Translation
 *      1a. Translation for allocation
 *      1b. Translation for offset due to panning and zooming
 *      1c. Translation for offset due to active panning
 *  2. Scaling
 *      2a. Scaling for zooming
 * 
 * Before events for mouse presses and movements are emitted, they
 * are converted by this coordinate system in the DrawingEventBox
 * into user space. 
 */
class CoordinateSystem
{
public:
    CoordinateSystem();
    virtual ~CoordinateSystem() {};

    Cairo::Matrix get_view_matrix(int width, int height);

    //void set_to_user_coordinates(Coordinate& c);
    //void set_to_device_coordinates(Coordinate& c);
    //void set_to_user_distance(Coordinate& c);
    //void set_to_device_distance(Coordinate& c);

    void snap_grid(bool s) {_snap_grid = s;}
    bool snap_grid() const {return _snap_grid;}

    float scale() const {return _scale;}

    void pan(Coordinate delta); // Translate by delta in x,y
    void zoom_in(Coordinate anchor);  // Zoom in with anchor fixed
    void zoom_out(Coordinate anchor);  // Zoom out with anchor fixed
protected:
    bool _snap_grid = false;

    
    Coordinate _center;
    float _scale = 4;
    float _scale_factor = 1.5; // Multiplication factor for zooming in and out
    const float MAX_ZOOM_IN = 25;
    const float MAX_ZOOM_OUT = 0.6;
};
//Cairo::Matrix _tmat;
//Cairo::Matrix _tmatinv;
//    bool _pan = false;
//    bool _zoom_in = false;
//    bool _zoom_out = false;
//    Coordinate _pan_anchor; // Anchor for pan 
//    Coordinate _delta; // Net change in position
//    Coordinate _pan_delta; // Change in position for current pan 

#endif /* COORDINATE_SYSTEM_H */