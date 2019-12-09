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

#include <app/schematic.h>

SchematicSheet::SchematicSheet() : 
    _elementlist(std::make_shared<GtkSpiceElementList>()),
    _wirelist(std::make_shared<GtkSpiceWireList>()),
    _portlist(std::make_shared<GtkSpicePortList>()),
    _nodemanager(std::make_shared<NodeManager>())
{}

SchematicSheet::~SchematicSheet() 
{}


