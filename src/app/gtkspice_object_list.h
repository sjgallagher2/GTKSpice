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

#ifndef GTKSPICE_OBJECT_LIST_H
#define GTKSPICE_OBJECT_LIST_H

#include <gtkmm.h>
#include <memory>
#include <map>
#include <set>
#include <app/gtkspice_object.h>

// TODO Fix "active" element/port/wire stuff to support multiple
// active elements, and generally improve the system to support
// (1) moving elements/ports/wires, (2) dropping elements and ports,
// and (3) moving one end of a wire vs the whole wire vs drawing

class GtkSpiceElementList
{
public:
    GtkSpiceElementList();
    virtual ~GtkSpiceElementList();

    virtual bool empty() const {return _element_list.empty();}
    virtual int size() const {return _element_list.size();}

    virtual void draw(const Cairo::RefPtr<Cairo::Context>& context);
    Glib::ustring get_spice_lines();

    std::shared_ptr<ObjectPins> get_element_pins(const Glib::ustring& inst_name ) {return find_element(inst_name)->get_pins();}
    int get_pin_under(const Glib::ustring inst_name, const Coordinate& pos);
    std::pair<std::shared_ptr<GtkSpiceElement>,int> get_pin_under(const Coordinate& pos);
    typedef std::pair<Glib::ustring,std::shared_ptr<SymbolPin>> ElementPinPair;
    std::vector<ElementPinPair> find_pins_on_wire(std::shared_ptr<GtkSpiceWire> wire, bool exclude_endpoints = false);

    Glib::ustring add_element(const Glib::ustring& sym_file, Coordinate pos);
    bool remove_element(const Glib::ustring& inst_name);
    std::shared_ptr<GtkSpiceElement> find_element(const Glib::ustring& inst_name);
    bool set_element_active(const Glib::ustring& inst_name);
    bool set_element_active(const Coordinate& pos);
    bool set_element_active(std::shared_ptr<GtkSpiceElement> elem);
    bool set_element_floating(const Glib::ustring& inst_name);
    bool set_element_floating(const Coordinate& pos);
    bool set_element_floating(std::shared_ptr<GtkSpiceElement> elem);
    void move_active_elements(const Coordinate& delta); // TODO
    std::vector<std::shared_ptr<GtkSpiceElement>> get_active_elements(); 
    std::shared_ptr<GtkSpiceElement> get_element_under_cursor(const Coordinate& mousepos);
    std::vector<std::shared_ptr<GtkSpiceElement>> get_elements_in_selection(const Coordinate& start, const Coordinate& end);
    void move_floating_elements(const Coordinate& delta); // TODO
    void rotate_floating_element(); // TODO, and only valid for one floating element
    std::vector<std::shared_ptr<GtkSpiceElement>> get_floating_elements(); 

protected:
    virtual void _auto_name(GtkSpiceElement& element);
private:
    typedef std::map<Glib::ustring, std::shared_ptr<GtkSpiceElement>> ElementList;
    typedef std::pair<Glib::ustring, std::shared_ptr<GtkSpiceElement>> ElementPair;
    ElementList _element_list;

};

class GtkSpicePortList
{
public:
    GtkSpicePortList() {}
    virtual ~GtkSpicePortList() {}

    virtual bool empty() const {return _port_list.empty();}
    virtual int size() const {return _port_list.size();}

    virtual void draw(const Cairo::RefPtr<Cairo::Context>& context);
    
    void add_in_port(Glib::ustring node_name, Coordinate pos);
    void add_out_port(Glib::ustring node_name, Coordinate pos);
    void add_inout_port(Glib::ustring node_name, Coordinate pos);
    std::shared_ptr<GtkSpicePort> add_gnd_port(Coordinate pos);
    void add_global_port(Coordinate pos);

    bool remove_port(std::shared_ptr<GtkSpicePort> port);
    //std::shared_ptr<GtkSpicePort> find_port(const Glib::ustring& node_name);
<<<<<<< HEAD
    bool set_port_active(const Coordinate& pos, bool active);
    bool set_port_active(std::shared_ptr<GtkSpicePort> port, bool active);
=======
    bool set_port_active(const Coordinate& pos, bool active = true);
    bool set_port_active(std::shared_ptr<GtkSpicePort> port, bool active = true);
>>>>>>> master
    void move_active_port(const Coordinate& pos);
    std::vector<std::shared_ptr<GtkSpicePort>> get_active_ports(); // Return port being moved/dropped
    std::shared_ptr<GtkSpicePort> get_port_under_cursor(const Coordinate& mousepos);
    std::vector<std::shared_ptr<GtkSpicePort>> get_ports_in_selection(const Coordinate& start, const Coordinate& end);
    std::shared_ptr<GtkSpicePort> get_port_pin_under(const Coordinate& pos);

private:
    typedef std::multimap<Glib::ustring, std::shared_ptr<GtkSpicePort>> PortList;
    typedef std::pair<Glib::ustring, std::shared_ptr<GtkSpicePort>> PortPair;
    PortList _port_list;

};


class GtkSpiceWireList
{
public:
    GtkSpiceWireList();
    virtual ~GtkSpiceWireList();

    virtual bool empty() const {return _wire_list.empty();}
    virtual int size() const {return _wire_list.size();}

    virtual void draw(const Cairo::RefPtr<Cairo::Context>& context);
    std::shared_ptr<GtkSpiceWire> add_wire(Glib::ustring node, Coordinate start, Coordinate end);
    bool remove_wire(std::shared_ptr<GtkSpiceWire> wire);
    std::shared_ptr<GtkSpiceWire> find_wire(int wire_index);
    void set_active_wire(std::shared_ptr<GtkSpiceWire> wire, int grabpoint = 1) // grabpoint = 0 (start) or 1 (end)
    {
        _active_wire = wire;
        _active_wire_grabpoint = grabpoint;
    }
    bool set_active_wire(const Coordinate& pos); // Find the grabpoint under pos, use that to set active wire
    void unset_active_wire() { _active_wire = nullptr; }
    bool has_active_wire() {return (_active_wire != nullptr);}
    // NOTE: Don't use this to move wire endpoints
    std::shared_ptr<GtkSpiceWire> get_active_wire() {return _active_wire;}
    void move_active_wire_grabpoint(const Coordinate& pos)
    {
        if(_active_wire)
        {
            if(_active_wire_grabpoint == 0)
                _active_wire->start(pos);
            else
                _active_wire->end(pos);
        }
    }
    std::shared_ptr<GtkSpiceWire> get_wire_under_cursor(const Coordinate& mousepos);
    std::vector<std::shared_ptr<GtkSpiceWire>> get_wires_in_selection(const Coordinate& start, const Coordinate& end);
    std::vector<std::shared_ptr<GtkSpiceWire>> get_wires_by_node(const Glib::ustring& node_name);

    // -1 for none, 0 for start, 1 for end
    std::vector<std::pair<std::shared_ptr<GtkSpiceWire>,int>> get_wire_pins_under(const Coordinate& pos);

    // Returns a vector of Coordinate representing Wire intersections (only when sharing node)
    std::vector<Coordinate> get_intersections();
private:
    typedef std::multimap<Glib::ustring,std::shared_ptr<GtkSpiceWire>> WireMap;
    typedef std::pair<Glib::ustring,std::shared_ptr<GtkSpiceWire>> WireMapPair;
    WireMap _wire_list; // <node name, wire>
    std::shared_ptr<GtkSpiceWire> _active_wire = nullptr; // Active wire
    int _active_wire_grabpoint = 1; // 0 = start, 1 = end, 2 = both; determines which end of wire is being moved

    std::set<Glib::ustring> _node_names; // List of all node names TODO Why??
};

#endif /* GTKSPICE_OBJECT_LIST_H */