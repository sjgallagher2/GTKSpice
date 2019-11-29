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
    inline float rad2deg(float angle_rad)
    {
        return angle_rad*180/M_PI;
    }
    inline float deg2rad(float angle_deg)
    {
        return angle_deg*M_PI/180;
    }
    inline float magnitude(Coordinate p)
    {
        return std::sqrt( p.x()*p.x() + p.y()*p.y() );
    }
    inline float arg(Coordinate p)
    {
        return std::atan2(p.y(),p.x());
    }
    inline float arg_d(Coordinate p)
    {
        return arg(p)*180/M_PI;
    }
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
            float d1 = magnitude(Coordinate(x1-x0,y1-y0));
            float d2 = magnitude(Coordinate(x2-x0,y2-y0));
            if(d1 < d2)
                d = d1;
            else
                d = d2;
        }
        return d;
    }
    inline float distance_from_rect(Coordinate p, Coordinate anchor, double width, double height)
    {
        float left_dist = distance_from_line(p,anchor,Coordinate(anchor.x(),anchor.y() + height));
        float top_dist = distance_from_line(p,Coordinate(anchor.x(),anchor.y()+height),Coordinate(anchor.x()+width,anchor.y() + height));
        float right_dist = distance_from_line(p,Coordinate(anchor.x()+width,anchor.y()+height),Coordinate(anchor.x()+width,anchor.y()));
        float bottom_dist = distance_from_line(p,Coordinate(anchor.x(),anchor.y()),Coordinate(anchor.x(),anchor.y()));
        return std::min(std::min(left_dist,top_dist),std::min(right_dist,bottom_dist));
    }

    // TODO Adapt for ellipses
    inline float distance_from_circle(Coordinate p, Coordinate center, float radius)
    {
        float dist = magnitude(p-center);
        return radius - dist;
    }
    // TODO Elliptical arcs
    inline float distance_from_arc(Coordinate p, Coordinate center, float radius, float start_angle_d, float end_angle_d)
    {
        float distance;
        if(start_angle_d != end_angle_d)
        {
            // Normalize and get endpoints
            if(start_angle_d > end_angle_d) // Make sure start is lower angle
            {
                float temp_ang = start_angle_d;
                start_angle_d = end_angle_d;
                end_angle_d = temp_ang;
            }

            float start_angle = start_angle_d*M_PI/180;
            float end_angle = end_angle_d*M_PI/180;
            if(std::abs(end_angle - start_angle) < 2*M_PI) // If the arc is not a full circle
            {
                // Start and end points on arc
                Coordinate startpoint(center.x() + radius*std::cos(start_angle),center.y() + radius*std::sin(start_angle));
                Coordinate endpoint(center.x() + radius*std::cos(end_angle),center.y() + radius*std::sin(end_angle));

                // Get angle from arc center to point
                float p_angle = std::atan2(p.y()-center.y(),p.x()-center.x());
                if(p_angle < 0) // Normalize to 0 to 2pi
                    p_angle = p_angle + 2*M_PI;
                
                // Check for angle inclusion
                bool incl=false,cwside=false,ccwside=false;
                if(p_angle > start_angle && p_angle < end_angle)
                    incl = true; // Angle of p included
                else if(p_angle < start_angle)
                    cwside = true; // Angle of p is clockwise from arc
                else if(p_angle > end_angle)
                    ccwside = true; // Angle of p is counterclockwise from arc
                
                // Compute distance
                if(incl)
                    distance = std::abs(radius - magnitude(Coordinate(p.x()-center.x(),p.y()-center.y())));
                else if(cwside)
                    distance = magnitude(Coordinate(p.x()-startpoint.x(),p.y()-startpoint.y()));
                else if(ccwside)
                    distance = magnitude(Coordinate(p.x()-endpoint.x(),p.y()-endpoint.y()));
                else
                    distance = -1; // Error
            }
            else // Arc is circle, use circle distance
                distance = std::abs(radius - magnitude(Coordinate(p.x()-center.x(),p.y()-center.y())));
        }
        else // Angles are the same, only one point
        {
            if(start_angle_d > end_angle_d) // Make sure start is lower angle
            {
                float temp_ang = start_angle_d;
                start_angle_d = end_angle_d;
                end_angle_d = temp_ang;
            }

            float start_angle = start_angle_d*M_PI/180;
            float end_angle = end_angle_d*M_PI/180;
            Coordinate anglepoint(center.x() + radius*std::cos(end_angle),center.y() + radius*std::sin(end_angle));
            distance = magnitude(Coordinate(p.x()-anglepoint.x(),p.y()-anglepoint.y()));
        }
        return distance;
    }
}

#endif /* GEOMETRY_H */
