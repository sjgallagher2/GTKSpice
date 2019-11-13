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

#ifndef OBJECT_SYMBOL_H
#define OBJECT_SYMBOL_H

#include <memory>
#include <vector>
#include <app/coordinate.h>
#include <app/draw_primitives.h>

struct SymbolAttribute 
{
    Glib::ustring name;
    Glib::ustring value;
    bool required; // Can this attribute have an empty value?
    bool show_on_schematic; // Should this attribute be shown on the schematic?
    bool editable; // Can the user change this attribute?
    bool removable; // Can the user remove this attribute from the symbol?
};

typedef std::vector<std::shared_ptr<Primitive>> ObjectGeometry;

class ObjectSymbol 
{
public:
    ObjectSymbol() {init_attributes();}
    bool under(Coordinate pos); // Return true if visible and selectable parts of Symbol are (almost) under pos

protected:
    virtual void init_attributes(); // Create default attributes: FILE, NAME, VALUE, and DESCRIPTION
    DrawSettings _drawsettings;
    bool _visible;
    ObjectGeometry _geometry;
    BoundingBox _boundingbox;
    std::vector<SymbolPin> _pins;
    std::vector<SymbolAttribute> _attrs;

};

#endif /* OBJECT_SYMBOL_H */
