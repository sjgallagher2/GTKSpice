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

#include <app/canvas.h>

// NOTE: You can pass a std::shared_ptr<ObjectTree> to this
Canvas::Canvas(std::shared_ptr<const ObjectTree> ot) : 
    _objecttree(ot),
    _state( std::make_shared<GtkSpiceState>() )
{
    // TODO Where to get DrawingEventBox and ViewFeatures?
}

Canvas::~Canvas()
{
}

