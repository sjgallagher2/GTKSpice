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

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <app/coordinate.h>
#include <cmath>
#include <app/vertex_list.h>

namespace Geometry
{
    inline float distance_from_line(Coordinate P,Coordinate LineStart,Coordinate LineEnd)
    {
        // Calculate the distance from a point to a line
        // First calculate point projection onto the line
        // If it projects onto the line, then calculate orthogonal distance
        // Otherwise calculate distance to endpoints
        float x0,x1,x2,y0,y1,y2,dx,dy;
        x1 = LineStart.x();
        x2 = LineEnd.x();
        dx = x2-x1;
        y1 = LineStart.y();
        y2 = LineEnd.y();
        dy = y2-y1;
        x0 = P.x();
        y0 = P.y();

        float ip = (x0-x1)*dx+(y0-y1)*dy;
        bool proj = (0 <= ip && ip <= dx*dx+dy*dy);
        // Thanks Daniel Fischer on SE

        float d;

        if(proj)
        {
            // Orthogonal distance
            d = std::abs(dy*x0-dx*y0+x2*y1-y2*x1)/std::sqrt(dy*dy+dx*dx);
        }
        else
        {
            // Endpoint distances
            float d1 = std::sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
            float d2 = std::sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0));
            if(d1 < d2)
                d = d1;
            else
                d = d2;
        }
        return d;
    }
    inline float distance_from_line(Coordinate P,std::shared_ptr<Vertex> LineStart,
        std::shared_ptr<Vertex> LineEnd)
    {
        float x0,x1,x2,y0,y1,y2,dx,dy;
        x1 = (float)LineStart->x();
        x2 = (float)LineEnd->x();
        dx = x2-x1;
        y1 = (float)LineStart->y();
        y2 = (float)LineEnd->y();
        dy = y2-y1;
        x0 = (float)P.x();
        y0 = (float)P.y();

        float ip = (x0-x1)*dx+(y0-y1)*dy;
        bool proj = (0 <= ip && ip <= dx*dx+dy*dy);
        // Thanks Daniel Fischer on SE

        float d;
        if(proj)
        {
            // Orthogonal distance
            d = std::abs(dy*x0-dx*y0+x2*y1-y2*x1)/std::sqrt(dy*dy+dx*dx);
        }
        else
        {
            // Endpoint distances
            float d1 = std::sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
            float d2 = std::sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0));
            if(d1 < d2)
                d = d1;
            else
                d = d2;
        }
        return d;
    }
}

#endif /* GEOMETRY_H */
