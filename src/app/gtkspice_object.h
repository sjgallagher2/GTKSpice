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

#ifndef GTKSPICE_OBJECT_H
#define GTKSPICE_OBJECT_H

#include <gtkmm.h>
#include <memory>
#include <app/coordinate.h>
#include <app/object_symbol.h>

class GtkSpiceObject 
{
public:
    GtkSpiceObject() {}
    GtkSpiceObject(const Glib::ustring& symbol_file);
    virtual ~GtkSpiceObject() {}

    bool set_symbol_file(const Glib::ustring& symbol_file);
    void set_symbol(std::shared_ptr<ObjectSymbol> sym) {_symbol = sym;}
    void set_symbol_visibility(bool vis) {if(_symbol) _symbol->visible(vis);}
    std::shared_ptr<ObjectSymbol> get_symbol() const {return _symbol;}

    void draw(Cairo::RefPtr<Cairo::Context> context);

    void set_name(Glib::ustring name);
    Glib::ustring get_inst_name() const {return _inst_name;}
    void set_position(Coordinate pos) {_position = pos; if(_symbol) _symbol->position(_position);}
    Coordinate get_position() const {return _position;}

    bool near(const Coordinate& pos); // Return true if Coordinate is in the BoundingBox
    bool under(const Coordinate& pos); // Return true if Coordinate is on the Symbol
protected:
    std::shared_ptr<ObjectSymbol> _symbol;
    Coordinate _position;
    bool _active;
    Glib::ustring _inst_name; // Prefix + name
    
};

class Wire
{};

class ObjectFactory 
{};

#endif /* GTKSPICE_OBJECT_H */
