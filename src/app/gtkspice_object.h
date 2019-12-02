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
#include <vector>
#include <app/coordinate.h>
#include <app/object_symbol.h>

/* Object representing circuit elements and on-screen SPICE directives
 * 
 * TODO Add node management, SpiceDeck, get_spice_line() function
 */

class GtkSpiceElement 
{
public:
    GtkSpiceElement() {}
    GtkSpiceElement(const Glib::ustring& symbol_file);
    virtual ~GtkSpiceElement() {}

    bool set_symbol_file(const Glib::ustring& symbol_file);
    void set_symbol(std::shared_ptr<ObjectSymbol> sym) {_symbol = sym;}
    void set_symbol_visibility(bool vis) {if(_symbol) _symbol->visible(vis);}
    std::shared_ptr<ObjectSymbol> get_symbol() const {return _symbol;}

    void draw(Cairo::RefPtr<Cairo::Context> context);

    void set_name(Glib::ustring name);
    Glib::ustring get_inst_name() const {return _inst_name;}
    void set_position(Coordinate pos) {_position = pos; if(_symbol) _symbol->position(_position);}
    Coordinate get_position() const {return _position;}

    bool near(const Coordinate& pos)
    {
        return _symbol->near(pos);
    }
    bool under(const Coordinate& pos)
    {
        return _symbol->under(pos);
    }
    bool within(const Coordinate& start, const Coordinate& end)
    {
        return _symbol->within(start,end);
    }
    
    void rotate90() {_symbol->rotate90();}
    void hflip() {_symbol->hflip();}
    void vflip() {_symbol->vflip();}

    bool connect_pin_to_node(const Glib::ustring& pin_name, int node);
    Glib::ustring get_spice_line(); // TODO

protected:
    std::shared_ptr<ObjectSymbol> _symbol;
    Coordinate _position;
    bool _active;
    Glib::ustring _inst_name; // Prefix + name
    std::vector<int> _nodes; // Nodes associated with pins, in SPICE_ORDER of pins

};

class Wire
{};

#endif /* GTKSPICE_OBJECT_H */
