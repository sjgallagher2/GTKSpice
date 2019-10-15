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

#ifndef COMPONENT_PARAMS_H
#define COMPONENT_PARAMS_H

#include <gtkmm.h>
#include <app/vertex_list.h>

struct ComponentParameters
{
    int index = -1;
    Glib::ustring label;
    bool is_active;
    float stroke_thickness;
    bool filled;
    bool editable;
    bool selectable;
};

struct LineParameters
{
    ComponentParameters cp;
    VertexList vertices;
};

struct PointParameters
{
    ComponentParameters cp;
    Coordinate pos; // Position of point
};

#endif /* COMPONENT_PARAMS_H */
