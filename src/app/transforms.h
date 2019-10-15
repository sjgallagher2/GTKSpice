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
    inline Coordinate anchored_scale (const Cairo::RefPtr<Cairo::Context>& context, Coordinate anchor, float scale, float scale_factor, bool zoom_in)
    {
        if(zoom_in)
        {
            context->scale(scale,scale);

            anchor.set_to_user_coordinate(context);
            // Scaling caused everything to move away, we need to bring it back. This comes
            // from Euclidean displacement after multiplying coordinates by scalefactor.
            // The factor is |1-scalefactor| e.g. 1.5x scale means |1-3/2| = 1/2
            float f = std::abs(1-scale_factor);
            context->translate(-anchor.x()*f,-anchor.y()*f);

            // We need to get the scale anchor back to a device distance
            anchor.set_to_device_distance(context);

            // Then return a displacement coordinate
            Coordinate displacement;
            displacement.x(-anchor.x()*f);
            displacement.y(-anchor.y()*f);
            return displacement;
        }
        else // Zoom out
        {
            context->scale(scale,scale);

            anchor.set_to_user_coordinate(context);
            // For zoom out, the distance is |1-1/scalefactor|
            float f = std::abs(1-1/scale_factor);
            context->translate(anchor.x()*f,anchor.y()*f);
            
            anchor.set_to_device_distance(context);

            Coordinate displacement;
            displacement.x(anchor.x()*f);
            displacement.y(anchor.y()*f);
            return displacement;

        }
    }
}
#endif /* TRANSFORMS_H */