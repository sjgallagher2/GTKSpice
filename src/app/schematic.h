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

#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include <memory>
#include <app/element_map.h>

class Schematic
{
public:
    Schematic(std::shared_ptr<GtkSpiceElementMap> em);
    ~Schematic();
private:
    std::shared_ptr<GtkSpiceElementMap> _elementmap;
};

#endif /* SCHEMATIC_H */
