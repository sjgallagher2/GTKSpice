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

#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include <cairomm/context.h>
#include <app/coordinate.h>

namespace Transforms
{
    // Performs an anchored scaling transformation on a Cairo context
    // Params
    //  anchor          Location to keep anchored
    //  scale           Absolute scale value
    //  scale_factor    Scale factor, multiplication factor for zoom in, division factor for zoom out,
    //                  should be >1
    //  zoom_in         true/false, false indicates zoom out

    // Returns: 
    //  displacement    A Coordinate for the displacement

    inline Coordinate anchored_scale_displacement (Coordinate anchor, float scale, float scale_factor, bool zoom_in)
    {
        if(zoom_in)
        {
            // Scaling caused everything to move away, we need to bring it back. This comes
            // from Euclidean displacement after multiplying coordinates by scalefactor.
            // The factor is |1-scalefactor| e.g. 1.5x scale means |1-3/2| = 1/2
            float f = scale/scale_factor*std::abs(1-scale_factor);

            // Return a displacement coordinate
            Coordinate displacement;
            displacement.x(-anchor.x()*f);
            displacement.y(-anchor.y()*f);
            return displacement;
        }
        else // Zoom out
        {
            // For zoom out, the distance is |1-1/scalefactor|
            float f = scale*scale_factor*std::abs(1-1/scale_factor);
            
            Coordinate displacement;
            displacement.x(anchor.x()*f);
            displacement.y(anchor.y()*f);
            return displacement;

        }
    }
}
#endif /* TRANSFORMS_H */