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

#include <app/vertex_list.h>
#include <app/canvas.h>
#include <app/component_params.h>
#include <app/coordinate.h>

// NOTE: You can pass a std::shared_ptr<ObjectTree> to this
Canvas::Canvas(std::shared_ptr<const ObjectTree> ot, std::shared_ptr<ActionFactory> af) : 
    _objecttree(ot),
    _state( std::make_shared<GtkSpiceState>() ),
    _actionfactory(af)
{
    // TODO Where to get DrawingEventBox and ViewFeatures?
    LineParameters lp;
    lp.cp.editable = false;
    lp.cp.filled = false;
    lp.cp.selectable = false;
    lp.cp.label = "Test line";
    lp.cp.stroke_thickness = 1;
    VertexList verts;
    verts.push_back(std::make_shared<Vertex>(0,0));
    verts.push_back(std::make_shared<Vertex>(10,10));
    lp.vertices = verts;

    _new_action.emit(_actionfactory->make_action(APPEND_LINE,lp));
}

Canvas::~Canvas()
{
}

