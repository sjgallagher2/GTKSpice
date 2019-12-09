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
    Glib::ustring get_name() const {return _name_no_prefix;}
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

    void connect_pin(int pin_order, std::string node)
    {
        if(pin_order < _pin_nodes.size())
        {
            _pin_nodes.at(pin_order) = node;
        }
    }
    std::string get_pin_name(int pin_order)
    {
        return "";
    }
    bool pin_has_node(int pin_order)
    {
        if (pin_order < _pin_nodes.size())
            return (_pin_nodes.at(pin_order) > "");
        else return false;
    }
    
    Glib::ustring get_spice_line(); // TODO

protected:
    std::shared_ptr<ObjectSymbol> _symbol;
    Coordinate _position;
    bool _active;
    Glib::ustring _inst_name; // Prefix + name
    Glib::ustring _name_no_prefix;

    std::vector<int> _pin_highlights; // Vector of pins which are not connected, indexed by SPICE_ORDER
    std::vector<std::string> _pin_nodes; // node of pins, indexed by SPICE_ORDER
};

class GtkSpiceNode
{
public:
    GtkSpiceNode(Glib::ustring name) : _name(name) {}
    ~GtkSpiceNode() {}
    void rename(Glib::ustring name) 
    {
        _name = name;
        for(auto& itr : _connections)
        {
            itr.first->connect_pin(itr.second,_name);
        }
    }
    std::string get_name() const {return _name;}
    
    void add_connection(std::shared_ptr<GtkSpiceElement> elem, int pin)
    {
        _connections.insert(std::pair<std::shared_ptr<GtkSpiceElement>, int>(elem, pin));
        elem->connect_pin(pin,_name);
    }
    void remove_connection(std::shared_ptr<GtkSpiceElement> elem, int pin)
    {
        if(_connections.find(elem) != _connections.end())
        {
            auto itrs = _connections.equal_range(elem);
            for(auto itr = itrs.first; itr != itrs.second; ++itr)
            {
                if(itr->second == pin)
                {
                    _connections.erase(itr);
                    elem->connect_pin(pin,"");
                }
            }
        }
    }
    
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
    
    void add_node(Glib::ustring node_name)
        { _node_map.insert(NodeKeyPair(node_name,std::make_shared<GtkSpiceNode>(node_name))); }
    void combine_nodes(Glib::ustring node1, Glib::ustring node2)
        {} // TODO
    
    void rename_node(Glib::ustring node_name, Glib::ustring new_name)
    {
        if( _node_map.find(node_name) != _node_map.end())
        {
            _node_map.find(node_name)->second->rename(new_name);
            std::shared_ptr<GtkSpiceNode> tnode = _node_map.find(node_name)->second;
            _node_map.erase(node_name);
            _node_map.insert(NodeKeyPair(new_name,tnode));
        }
    }
    
    void connect_node(Glib::ustring node_name,std::shared_ptr<GtkSpiceElement> elem,int pin_order)
    {
        if( _node_map.find(node_name) != _node_map.end())
        {
            if( !elem->pin_has_node(pin_order) )
                _node_map.find(node_name)->second->add_connection(elem, pin_order);
            else
                std::cerr << "WARNING: Element pin already has a node. \n";
        }
    }
    
    void break_connection(Glib::ustring node_name, std::shared_ptr<GtkSpiceElement> elem, int pin_order)
    {
        if( _node_map.find(node_name) != _node_map.end())
            if( elem->pin_has_node(pin_order) )
                _node_map.find(node_name)->second->remove_connection(elem, pin_order);
    }
    
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

private:
    std::shared_ptr<GtkSpiceNode> _node;
    Coordinate _start,_end;
    double _height,_width;
};

#endif /* GTKSPICE_OBJECT_H */
