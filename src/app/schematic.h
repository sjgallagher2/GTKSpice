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
#include <algorithm>
#include <app/gtkspice_object_list.h>
#include <app/gtkspice_object.h>

class SchematicSheet
{
public:
    SchematicSheet();
    ~SchematicSheet();

    void draw(Cairo::RefPtr<Cairo::Context> context)
    {
        _elementlist->draw(context);
        _wirelist->draw(context);
        _portlist->draw(context);
    }

    std::shared_ptr<GtkSpiceElementList> get_element_list() const {return _elementlist;}
    std::shared_ptr<GtkSpiceWireList> get_wire_list() const {return _wirelist;}
    std::shared_ptr<GtkSpicePortList> get_port_list() const {return _portlist;}
    std::shared_ptr<NodeManager> get_node_manager() const {return _nodemanager;}
    std::vector<Glib::ustring> get_node_list() const; // TODO

    Glib::ustring get_spice_lines();

    std::shared_ptr<GtkSpiceElement> get_element_under_cursor(Coordinate pos);
    std::shared_ptr<GtkSpiceWire> get_wire_under_cursor(Coordinate pos);
    std::shared_ptr<GtkSpicePort> get_port_under_cursor(Coordinate pos);
    std::vector<std::shared_ptr<GtkSpiceElement>> get_elements_in_selection(const Coordinate& start, const Coordinate& end);
    std::vector<std::shared_ptr<GtkSpiceWire>> get_wires_in_selection(const Coordinate& start, const Coordinate& end);
    std::vector<std::shared_ptr<GtkSpicePort>> get_ports_in_selection(const Coordinate& start, const Coordinate& end);

    Glib::ustring add_element(const Glib::ustring& sym_file, Coordinate pos, bool floating = false);
    void add_wire(Coordinate start, Coordinate end, bool active = false);
    void add_gnd_port(Coordinate pos, bool floating = false); 

    std::shared_ptr<GtkSpiceElement> find_element(const Glib::ustring& inst_name);
    
    void move_active_objects(Coordinate delta); // TODO how to handle this?
    void remove_active_objects();
    void unset_active_objects();
    void move_floating_objects(Coordinate delta); // TODO
    void rotate_floating_object(); // Valid when only one object is floating
    void remove_floating_objects();
    void drop_floating_objects();

    std::vector<std::shared_ptr<GtkSpiceElement>> get_active_elements();
    std::vector<std::shared_ptr<GtkSpicePort>> get_active_ports();

    void remove_element(std::shared_ptr<GtkSpiceElement> elem);
    void remove_element(const Glib::ustring& inst_name);
    void remove_wire(std::shared_ptr<GtkSpiceWire> wire);
    void remove_port(std::shared_ptr<GtkSpicePort> port);

private:
    void _update_pin_element_connections(Glib::ustring elemname, std::shared_ptr<SymbolPin> pin);
    void _update_pin_wire_connections(Glib::ustring elemname, std::shared_ptr<SymbolPin> pin);
    void _update_pin_port_connections(Glib::ustring elemname, std::shared_ptr<SymbolPin> pin);
    
    void _update_port_connections(std::shared_ptr<GtkSpicePort> port);
    void _update_element_connections(Glib::ustring elemname);

    void _update_intersections();

    std::shared_ptr<GtkSpiceElementList> _elementlist;
    std::shared_ptr<GtkSpiceWireList> _wirelist;
    std::shared_ptr<GtkSpicePortList> _portlist;
    std::shared_ptr<NodeManager> _nodemanager;
    // TODO Add method of storing drawing intersection connections
};

class GtkSpiceSchematic
{
public:
    GtkSpiceSchematic() 
    {
        _sheets.push_back(std::make_shared<SchematicSheet>());
        _active_sheet = _sheets.at(0);
    }
    ~GtkSpiceSchematic() {}

    Glib::ustring get_spice_lines();

    int add_sheet() {_sheets.push_back(std::make_shared<SchematicSheet>());return _sheets.size()-1;}
    void remove_sheet(int index);
    std::shared_ptr<SchematicSheet> get_sheet(int index) {if(index < _sheets.size()) return _sheets.at(index);}

    void set_active_sheet(int index) {if(index < _sheets.size()) _active_sheet = _sheets.at(index);}
    std::shared_ptr<SchematicSheet> get_active_sheet() {return _active_sheet;}
    int get_active_sheet_index();

    void redraw(Cairo::RefPtr<Cairo::Context> context)
    {
        if(_active_sheet != nullptr)
            _active_sheet->draw(context);
    }

private:
    std::vector<std::shared_ptr<SchematicSheet>> _sheets;
    std::shared_ptr<SchematicSheet> _active_sheet;

};

#endif /* SCHEMATIC_H */
