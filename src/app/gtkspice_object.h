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
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <app/coordinate.h>
#include <app/object_symbol.h>

/* Object representing circuit elements and on-screen SPICE directives
 * 
 * TODO Add SPICE directive text element
 */

class GtkSpiceWire;

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
    Glib::ustring get_name() const {return _name_no_prefix;}
    void set_position(Coordinate pos) {_position = pos; if(_symbol) _symbol->position(_position);}
    Coordinate get_position() const {return _position;}

    void set_active() {_active = true;}
    void set_floating() {_floating = true;}
    void unset_active() {_active = false;}
    void drop() {_floating = false;}
    bool is_active() const {return _active;}
    bool is_floating() const {return _floating;}

    bool near(const Coordinate& pos) const {return _symbol->near(pos);}
    bool under(const Coordinate& pos) const { return _symbol->under(pos); }
    bool within(const Coordinate& start, const Coordinate& end) const { return _symbol->within(start,end); }
    
    void rotate90() {_symbol->rotate90();}
    void hflip() {_symbol->hflip();}
    void vflip() {_symbol->vflip();}

    void connect_pin(int pin_order, std::string node);
    std::string get_pin_name(int pin_order) { return _symbol->get_pin(pin_order)->get_attribute_value("NAME"); } // TODO
    std::shared_ptr<ObjectPins> get_pins() const {return _symbol->get_pins();}
    std::shared_ptr<SymbolPin> get_pin(Glib::ustring pin_name) const {return _symbol->get_pin(pin_name);}
    std::shared_ptr<SymbolPin> get_pin(int pin_order) const {return _symbol->get_pin(pin_order);}
    int pin_count() const {return _symbol->pin_count();}
    int pin_under(const Coordinate& pos); // Return SPICE_ORDER of pin under pos, or -1 if none
    bool pin_has_node(int pin_order)
    {
        if (pin_order <= _pin_nodes.size())
            return (_pin_nodes.at(pin_order-1) > "");
        else return false;
    }
    std::string get_pin_node(int pin_order)
    {
        if(pin_order <= _pin_nodes.size())
            return _pin_nodes.at(pin_order-1);
        return "";
    }
    
    Glib::ustring get_spice_line(); // TODO

protected:
    std::shared_ptr<ObjectSymbol> _symbol;
    Coordinate _position;
    bool _active;
    bool _floating;
    Glib::ustring _inst_name; // Prefix + name
    Glib::ustring _name_no_prefix;

    std::vector<bool> _pin_highlights; // Vector of pin highlight booleans, indexed as SPICE_ORDER-1
    std::vector<std::string> _pin_nodes; // node of pins, indexed as SPICE_ORDER
};

/* GtkSpiceNode
 * A node stores its name (a string), pointers to the elements associated with it with their pin numbers,
 * pointers to the wires associated with it, and a priority value (0-3) for node merges.
 * 
 * AVOID ACCESSING NODES DIRECTLY. DO NOT USE MODIFY CONNECTIONS WITH THE NODE.
 */
class GtkSpiceNode
{
public:
    GtkSpiceNode(Glib::ustring name) : _name(name) {}
    ~GtkSpiceNode() {}
    void rename(Glib::ustring name);
    std::string get_name() const {return _name;}
    
    void add_connection(std::shared_ptr<GtkSpiceElement> elem, int pin);
    void remove_connection(std::shared_ptr<GtkSpiceElement> elem, int pin);
    void connect_wire(std::shared_ptr<GtkSpiceWire> wire);
    void disconnect_wire(std::shared_ptr<GtkSpiceWire> wire);
    
    std::multimap<std::shared_ptr<GtkSpiceElement>,int> get_connections() const {return _connections;}
    std::vector<std::shared_ptr<GtkSpiceWire>> get_wires() const {return _wires;}
    
    // Set priority forces, add priority either increases the priority or keeps it the same
    void set_priority(int priority) {if(priority >= 0 && priority <= 3) _priority = priority;}
    void add_priority(int priority) {if(priority >= 0 && priority <= 3 && priority > _priority) _priority = priority;}
    int get_priority() const {return _priority;}

private:
    Glib::ustring _name;
    std::multimap<std::shared_ptr<GtkSpiceElement>, int> _connections; // <element,pin> for pin indexed by SPICE_ORDER
    std::vector<std::shared_ptr<GtkSpiceWire>> _wires; // Wires connected with this node
    int _priority = 0; // Priorities for merges (see NodeManager::merge_nodes())
};

/* NodeManager
 * The NodeManager is the interface to the list (map) of nodes in the schematic, which it stores.
 * Nodes can be added or merged. Nodes can be autonamed using add_auto_node(), which is preferred 
 * to the manual add_node() function. 
 * 
 * A Node stores the elements and wires associated with itself. An element or wire, in turn, only 
 * stores the node string(s), and it doesn't manage the node string(s) directly. The Node will 
 * keep this updated as necessary. This makes node merges and renames much easier.
 * 
 * To connect an element pin or wire to a node, use the NodeManager, NOT the Node. Call 
 * connect_node() when a pin or wire does not already have a node. If the element pin or
 * wire already has a node, you must call break_connection() first. 
 * 
 * A node merge occurs when a wire connects two other wires which originally have different nodes.
 * The wires must all share one node, so we need to decide which node to keep and which can be 
 * merged in. This is done with priorities. For instance, a port (like ground) has the highest
 * priority.
 */
class NodeManager
{
public:
    NodeManager() {}
    ~NodeManager() {}

    bool empty() const {return _node_map.empty();}
    int size() const {return _node_map.size();}
    
    std::shared_ptr<GtkSpiceNode> find_node(Glib::ustring node_name)
    {
        if(_node_map.find(node_name) != _node_map.end())
            return _node_map.find(node_name)->second;
    }

    void add_node(Glib::ustring node_name)
        { _node_map.insert(NodeKeyPair(node_name,std::make_shared<GtkSpiceNode>(node_name))); }
    Glib::ustring add_auto_node(); // Adds a new node which is autonamed
    Glib::ustring merge_nodes(Glib::ustring node1, Glib::ustring node2);
    
    void rename_node(Glib::ustring node_name, Glib::ustring new_name);
    void connect_node(Glib::ustring node_name,std::shared_ptr<GtkSpiceElement> elem,int pin_order);
    void connect_node(Glib::ustring node_name,std::shared_ptr<GtkSpiceWire> wire);
    void break_connection(Glib::ustring node_name, std::shared_ptr<GtkSpiceElement> elem, int pin_order);
    void break_connection(Glib::ustring node_name, std::shared_ptr<GtkSpiceWire> wire);
    
private:
    typedef std::map<Glib::ustring,std::shared_ptr<GtkSpiceNode>> NodeMap;
    typedef std::pair<Glib::ustring,std::shared_ptr<GtkSpiceNode>> NodeKeyPair;
    
    NodeMap _node_map;
};

/* A 'wire' is a visual aid showing node connections in a circuit. Wires are
 * linked-list type data structures which represent a single, straight-line 
 * segment on the schematic. Because wires are used to control node connections,
 * it's important to be able to check when wires are connected to each other,
 * when they connect directly to elements or ports, when they have junctions, 
 * and so on. When a wire is deleted, we must check the wires it was connected
 * to, traversing them to find a new node name for those other wires. 
 * 
 * Wires are visual, so the "order matters", we can't simply use node names to
 * organize them, we need to know what wires are actually connected and in 
 * what way. Contrast this with elements and ports, where we only need to say
 * what node name the pins have. 
 */
class GtkSpiceWire
{
public:
    typedef std::vector<std::shared_ptr<GtkSpiceWire>> WireVector;
public:
    GtkSpiceWire(Glib::ustring node, Coordinate start, Coordinate end) : 
        _node(node),_start(start),_end(end) {}
    ~GtkSpiceWire() {};

    void draw(Cairo::RefPtr<Cairo::Context> context);
    void start(Coordinate s) {_start = s;}
    void end(Coordinate e) 
    {
        // Enforce orthogonality of wires
        _end = _start;
        Coordinate delta = e - _start; 
        if(delta.x() > delta.y())
            _end.x(e.x());
        else if(delta.y() > delta.x())
            _end.y(e.y());
        else
            _end.x(e.x()); // Default to horizontal wire
    }
    Coordinate start() const {return _start;}
    Coordinate end() const {return _end;}
    
    bool has_start_junction() {return _start_junction;}
    bool has_end_junction() {return _end_junction;}
    void set_start_junction(bool j) {_start_junction = j;}
    void set_end_junction(bool j) {_end_junction = j;}

    bool has_start_connection() const {return _has_start_wire_connection || _count_start_element_connections || _count_start_port_connections;}
    bool has_end_connection() const {return _has_end_wire_connection || _count_end_element_connections || _count_end_port_connections;}
    bool has_junc_connection() const {return _has_junc_wire_connections;}
    bool has_start_wire_connection() const {return _has_start_wire_connection;}
    bool has_end_wire_connection() const {return _has_end_wire_connection;}
    bool has_start_element_connection() const {return _count_start_element_connections;}
    bool has_end_element_connection() const {return _count_end_element_connections;}
    bool has_start_port_connection() const {return _count_start_port_connections;}
    bool has_end_port_connection() const {return _count_end_port_connections;}

    // To add a wire connection, pass its pointer
    void add_start_wire_connection(std::shared_ptr<GtkSpiceWire> wire);
    void add_end_wire_connection(std::shared_ptr<GtkSpiceWire> wire);
    void add_junc_wire_connection(std::shared_ptr<GtkSpiceWire> wire);

    // To remove a wire connection, pass its pointer, and the list will be
    // searched for that pointer and an element that matches will be removed
    void remove_start_wire_connection(std::shared_ptr<GtkSpiceWire> wire);
    void remove_end_wire_connection(std::shared_ptr<GtkSpiceWire> wire);
    void remove_junc_wire_connection(std::shared_ptr<GtkSpiceWire> wire);

    // These simply increment/decrement the number of element/port connections,
    // no further information about the connections is stored
    void add_start_element_connection() {_count_start_element_connections++;}
    void add_end_element_connection() {_count_end_element_connections++;}
    void add_start_port_connection() {_count_start_port_connections++;}
    void add_end_port_connection() {_count_end_port_connections++;}
    void remove_start_element_connection() {_count_start_element_connections--;}
    void remove_end_element_connection() {_count_end_element_connections--;}
    void remove_start_port_connection() {_count_start_port_connections--;}
    void remove_end_port_connection() {_count_end_port_connections--;}

    WireVector get_start_connections() const {return _start_wire_connections;}
    WireVector get_end_connections() const {return _end_wire_connections;}
    WireVector get_junc_connections() const {return _junc_wire_connections;}

    bool under(Coordinate pos, float tol = 1);
    bool within(const Coordinate& begin, const Coordinate& end);

    void assign_node(Glib::ustring node) {_node = node;}
    Glib::ustring get_node_name() const {return _node;}

    void set_active() {_active = true;}
    void set_floating() {_floating = true;}
    void unset_active() {_active = false;}
    void drop() {_floating = false;}
    bool is_active() const {return _active;}
    bool is_floating() const {return _floating;}

    int pin_under(const Coordinate& pos); // Returns 0 (start) or 1 (end) or -1 (none)
    // Defaults to end if start==end

private:
    // Connections on start and end to other wires, elements, ports
    // This is needed for wire traversals
    WireVector _start_wire_connections;
    WireVector _end_wire_connections;
    WireVector _junc_wire_connections; // Wires that attach to this wire at a junction
    bool _has_start_wire_connection;
    bool _has_end_wire_connection;
    bool _has_junc_wire_connections;
    int _count_start_element_connections = 0;
    int _count_end_element_connections = 0;
    int _count_start_port_connections = 0;
    int _count_end_port_connections = 0;
    
    Glib::ustring _node;
    Coordinate _start,_end;
    bool _start_junction = false; // Show a junction square on start pin
    bool _end_junction = false; // Show a junction square on end pin
    double _height,_width;
    bool _active = false;
    bool _floating = false;
};

class GtkSpicePort
{
public:
    GtkSpicePort() {}
    virtual ~GtkSpicePort() {}

    virtual void draw(Cairo::RefPtr<Cairo::Context> context) { if(_symbol) _symbol->draw(context,{_highlight} ); };
    
    void set_position(Coordinate pos) {_position = pos; if(_symbol) _symbol->position(_position);}
    Coordinate get_position() const {return _position;}
    Glib::ustring get_node_name() const {return _node_name;}
    Coordinate get_pin_position() {return _symbol->get_pins()->at(0)->pin_location();}

    void set_active() {_active = true;}
    void set_floating() {_floating = true;}
    void unset_active() {_active = false;}
    void drop() {_floating = false;}
    bool is_active() const {return _active;}
    bool is_floating() const {return _floating;}

    bool near(const Coordinate& pos) const {return _symbol->near(pos);}
    bool under(const Coordinate& pos) const { return _symbol->under(pos); }
    bool within(const Coordinate& start, const Coordinate& end) const { return _symbol->within(start,end); }
    bool pin_under(const Coordinate& pos);
    
    void rotate90() {_symbol->rotate90();}
    void hflip() {_symbol->hflip();}
    void vflip() {_symbol->vflip();}

    void set_highlight(bool h) {_highlight = h;}

protected:
    Glib::ustring _node_name;
    Coordinate _position;
    std::shared_ptr<ObjectSymbol> _symbol;
    bool _highlight = true;
    bool _active = false;
    bool _floating = false;

};
class GtkSpiceInPort : public GtkSpicePort
{
public:
    GtkSpiceInPort(Glib::ustring node_name);
    virtual ~GtkSpiceInPort() {}
    
protected:
};
class GtkSpiceOutPort : public GtkSpicePort
{
public:
    GtkSpiceOutPort(Glib::ustring node_name);
    virtual ~GtkSpiceOutPort() {}

protected:
};
class GtkSpiceInoutPort : public GtkSpicePort
{
public:
    GtkSpiceInoutPort(Glib::ustring node_name);
    virtual ~GtkSpiceInoutPort() {}

protected:
};
class GtkSpiceGndPort : public GtkSpicePort
{
public:
    GtkSpiceGndPort();
    virtual ~GtkSpiceGndPort() {}
    
protected:
};
class GtkSpiceGlobalPort : public GtkSpicePort
{
public:
    GtkSpiceGlobalPort(Glib::ustring node_name);
    virtual ~GtkSpiceGlobalPort() {}

protected:
};


#endif /* GTKSPICE_OBJECT_H */
