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


class GtkSpiceNode
{
public:
    GtkSpiceNode(Glib::ustring name) : _name(name) {}
    ~GtkSpiceNode() {}
    void rename(Glib::ustring name);
    std::string get_name() const {return _name;}
    
    void add_connection(std::shared_ptr<GtkSpiceElement> elem, int pin);
    void remove_connection(std::shared_ptr<GtkSpiceElement> elem, int pin);
    
    std::multimap<std::shared_ptr<GtkSpiceElement>,int> get_connections() const {return _connections;}
    
private:
    Glib::ustring _name;
    std::multimap<std::shared_ptr<GtkSpiceElement>, int> _connections; // <element,pin> for pin indexed by SPICE_ORDER
};

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
    void merge_nodes(Glib::ustring node1, Glib::ustring node2,int node1_priority = 0,int node2_priority = 0);
    
    void rename_node(Glib::ustring node_name, Glib::ustring new_name);
    void connect_node(Glib::ustring node_name,std::shared_ptr<GtkSpiceElement> elem,int pin_order);
    void break_connection(Glib::ustring node_name, std::shared_ptr<GtkSpiceElement> elem, int pin_order);
    
private:
    typedef std::map<Glib::ustring,std::shared_ptr<GtkSpiceNode>> NodeMap;
    typedef std::pair<Glib::ustring,std::shared_ptr<GtkSpiceNode>> NodeKeyPair;
    
    NodeMap _node_map;
};

/* 
 * 
 */
class GtkSpiceWire
{
public:
    GtkSpiceWire(std::shared_ptr<GtkSpiceNode> node, Coordinate start, Coordinate end) : 
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
    bool under(Coordinate pos, float tol = 1);
    bool within(const Coordinate& begin, const Coordinate& end);

    void assign_node(std::shared_ptr<GtkSpiceNode> node) {_node = node;}
    std::shared_ptr<GtkSpiceNode> get_node() const {return _node;}
    Glib::ustring get_node_name() const {return _node->get_name();}

    void set_active() {_active = true;}
    void set_floating() {_floating = true;}
    void unset_active() {_active = false;}
    void drop() {_floating = false;}
    bool is_active() const {return _active;}
    bool is_floating() const {return _floating;}

    int pin_under(const Coordinate& pos); // Returns 0 (start) or 1 (end) or -1 (none)
    // Defaults to end if start==end

private:
    std::shared_ptr<GtkSpiceNode> _node;
    Coordinate _start,_end;
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
