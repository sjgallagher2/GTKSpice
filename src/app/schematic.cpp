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

Glib::ustring SchematicSheet::get_spice_lines()
{
    return _elementlist->get_spice_lines();
}

std::shared_ptr<GtkSpiceElement> SchematicSheet::get_element_under_cursor(Coordinate pos)
{
    return _elementlist->get_element_under_cursor(pos);
}
std::shared_ptr<GtkSpiceWire> SchematicSheet::get_wire_under_cursor(Coordinate pos)
{
    return _wirelist->get_wire_under_cursor(pos);
}
std::shared_ptr<GtkSpicePort> SchematicSheet::get_port_under_cursor(Coordinate pos)
{
    return _portlist->get_port_under_cursor(pos);
}
std::vector<std::shared_ptr<GtkSpiceElement>> SchematicSheet::get_elements_in_selection(const Coordinate& start, const Coordinate& end)
{
    return _elementlist->get_elements_in_selection(start,end);
}
std::vector<std::shared_ptr<GtkSpiceWire>> SchematicSheet::get_wires_in_selection(const Coordinate& start, const Coordinate& end)
{
    return _wirelist->get_wires_in_selection(start,end);
}
std::vector<std::shared_ptr<GtkSpicePort>> SchematicSheet::get_ports_in_selection(const Coordinate& start, const Coordinate& end)
{
    return _portlist->get_ports_in_selection(start,end);
}
Glib::ustring SchematicSheet::add_element(const Glib::ustring& sym_file, Coordinate pos, bool floating)
{
    // This should add the element at location 'pos'
    // Check the pins for overlaps with wires and other pins
    // Make necessary connections by first adding a wire (of zero length)
    // and then making a node or selecting the node from a port on the net

    Glib::ustring elemname = _elementlist->add_element(sym_file,pos);
    if(floating)
        _elementlist->set_element_floating(elemname);
    
    auto pins = _elementlist->get_element_pins(elemname);
    
    for(auto itr = pins->begin(); itr != pins->end(); ++itr)
    {
        // Iterating over pins
        Coordinate pin_pos = (*itr)->pin_location() + _elementlist->find_element(elemname)->get_position();
        
        // Check other elements
        std::pair<std::shared_ptr<GtkSpiceElement>,int> under_e;
        under_e = _elementlist->get_pin_under(pin_pos);
        if(under_e.first != nullptr && under_e.second != -1 && under_e.first->get_inst_name() != elemname)
        {
            // This element's pin is under our pin
            // Connect them with a wire of length zero
            if(under_e.first->pin_has_node(under_e.second))
            {
                // Other element already has a node/wire
                Glib::ustring node = under_e.first->get_pin_node(under_e.second);
                std::shared_ptr<GtkSpiceWire> newwire = _wirelist->add_wire(node,pin_pos,pin_pos);
                _nodemanager->connect_node(node,_elementlist->find_element(elemname),std::stoi((*itr)->get_attribute_value("SPICE_ORDER")));
                _nodemanager->connect_node(node,newwire);
                _nodemanager->find_node(node)->add_priority(2);
            }
            else
            {
                // Neither element has a node/wire
                Glib::ustring newnode = _nodemanager->add_auto_node();
                std::shared_ptr<GtkSpiceWire> newwire = _wirelist->add_wire(newnode,pin_pos,pin_pos);
                _nodemanager->connect_node(newnode,_elementlist->find_element(elemname),std::stoi((*itr)->get_attribute_value("SPICE_ORDER")));
                _nodemanager->connect_node(newnode,under_e.first,under_e.second);
                _nodemanager->connect_node(newnode,newwire);
                _nodemanager->find_node(newnode)->add_priority(2);
            }
        }

        // Check wires
        std::pair<std::shared_ptr<GtkSpiceWire>,int> under_w;
        under_w = _wirelist->get_wire_pin_under(pin_pos);
        if(under_w.first != nullptr && under_w.second != -1)
        {
            // This wire's pin is under our pin
            // Connect them
            _nodemanager->connect_node(under_w.first->get_node_name() ,_elementlist->find_element(elemname),std::stoi((*itr)->get_attribute_value("SPICE_ORDER")));
            _nodemanager->find_node(under_w.first->get_node_name())->add_priority(2);
        }
        else
        {
            if(std::shared_ptr<GtkSpiceWire> wire = _wirelist->get_wire_under_cursor(pin_pos))
            {
                // This wire is under our pin as a junction
                // Connect them with a wire of length zero, add intersection?
                std::shared_ptr<GtkSpiceWire> newwire = _wirelist->add_wire(wire->get_node_name(),pin_pos,pin_pos);
                _nodemanager->connect_node(wire->get_node_name(),_elementlist->find_element(elemname),std::stoi((*itr)->get_attribute_value("SPICE_ORDER")));
                _nodemanager->connect_node(wire->get_node_name(),newwire);
                _nodemanager->find_node(wire->get_node_name())->add_priority(2);
            }
        }

        // Check ports
        std::shared_ptr<GtkSpicePort> port = _portlist->get_port_pin_under(pin_pos);
        if(port)
        {
            // This port's pin is under our pin
            // Connect them with a wire of length zero
            std::shared_ptr<GtkSpiceWire> wire = _wirelist->add_wire(port->get_node_name(),pin_pos,pin_pos);
            _nodemanager->connect_node(port->get_node_name(),_elementlist->find_element(elemname),std::stoi((*itr)->get_attribute_value("SPICE_ORDER")));
            _nodemanager->connect_node(port->get_node_name(),wire);
            _nodemanager->find_node(port->get_node_name())->add_priority(3);
        }
        

    }
    return elemname;
}
void SchematicSheet::add_wire(Coordinate start, Coordinate end, bool active)
{
    // This should add a wire from 'start' to 'end', checking endpoints
    // for new connections. If no new connection (no pins or wires under
    // endpoints) make a new node. Otherwise, connect to node of new
    // connection. TODO: Should also check for any pins now connecting
    // to the wire, or have all components update on new wire addition

    std::shared_ptr<GtkSpiceNode> node = nullptr; // Final node
    std::shared_ptr<GtkSpiceNode> node1 = nullptr; // Start position node
    std::shared_ptr<GtkSpiceNode> node2 = nullptr; // End position node
    // We'll check later if these are the same, and we'll merge if needed
    
    // Check start position for other wires
    std::pair<std::shared_ptr<GtkSpiceWire>,int> under_w;
    under_w = _wirelist->get_wire_pin_under(start);
    if(under_w.first != nullptr && under_w.second != -1)
    {
        // This wire's pin is under our wire's pin, connect them
        if(!node1)
        {
            node1 = _nodemanager->find_node(under_w.first->get_node_name());
            node1->set_priority(1);
        }
        else if(_nodemanager->find_node(under_w.first->get_node_name()) != node1)
        {
            Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),under_w.first->get_node_name());
            node1 = _nodemanager->find_node(mergenodename);
        }
    }
    else if(std::shared_ptr<GtkSpiceWire> wire = _wirelist->get_wire_under_cursor(start))
    {
        // This wire is under our pin as a junction, connect them (intersection?)
        if(!node1)
            node1 = _nodemanager->find_node(wire->get_node_name());
        else if(_nodemanager->find_node(wire->get_node_name()) != node1)
        {
            Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),wire->get_node_name());
            node1 = _nodemanager->find_node(mergenodename);
        }
        
    }
    // Check end position for other wires
    under_w = _wirelist->get_wire_pin_under(end);
    if(under_w.first != nullptr && under_w.second != -1)
    {
        // This wire's pin is under our wire's pin, connect them
        if(!node2)
            node2 = _nodemanager->find_node(under_w.first->get_node_name());
        else if(_nodemanager->find_node(under_w.first->get_node_name()) != node1)
        {
            Glib::ustring mergenodename = _nodemanager->merge_nodes(node2->get_name(),under_w.first->get_node_name());
            node2 = _nodemanager->find_node(mergenodename);
        }
    }
    else if(std::shared_ptr<GtkSpiceWire> wire = _wirelist->get_wire_under_cursor(end))
    {
        // This wire is under our pin as a junction, connect them (intersection?)
        if(!node2)
            node2 = _nodemanager->find_node(wire->get_node_name());
        else if(_nodemanager->find_node(wire->get_node_name()) != node1)
        {
            Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),wire->get_node_name());
            node2 = _nodemanager->find_node(mergenodename);
        }
    }

    // Check start position for elements
    std::pair<std::shared_ptr<GtkSpiceElement>,int> under_e;
    under_e = _elementlist->get_pin_under(start);
    if(under_e.first != nullptr && under_e.second != -1)
    {
        // This element's pin is under our wire
        // Connect them with a wire of length zero
        if(under_e.first->pin_has_node(under_e.second))
        {
            // Other element already has a node/wire
            if(!node1)
                node1 = _nodemanager->find_node(under_e.first->get_pin_node(under_e.second));
            else 
            {
                Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),under_e.first->get_pin_node(under_e.second));
                node1 = _nodemanager->find_node(mergenodename);
            }
        }
        else
        {
            // Other element does not have a node/wire
            if(node1)
            {
                _nodemanager->connect_node(node1->get_name(),under_e.first,under_e.second);
            }
            else if(!node1 && !node2)
            { 
                Glib::ustring newnodename = _nodemanager->add_auto_node(); // Make new node
                node1 = _nodemanager->find_node(newnodename); // Set this wire's node to the new node
                _nodemanager->connect_node(newnodename,under_e.first,under_e.second); // Connect element pin to wire node
            }
            else if(!node1 && node2)
            {
                node1 = node2;
                _nodemanager->connect_node(node1->get_name(),under_e.first,under_e.second);
            }
            else
            {
                Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),under_e.first->get_pin_node(under_e.second));
                node1 = _nodemanager->find_node(mergenodename);
            }
        }
    }
    // Check end position for elements
    under_e = _elementlist->get_pin_under(end);
    if(under_e.first != nullptr && under_e.second != -1)
    {
        // This element's pin is under our wire
        // Connect them with a wire of length zero
        if(under_e.first->pin_has_node(under_e.second))
        {
            // Other element already has a node/wire
            if(!node2)
                node2 = _nodemanager->find_node(under_e.first->get_pin_node(under_e.second));
            else
            {
                Glib::ustring mergenodename = _nodemanager->merge_nodes(node2->get_name(),under_e.first->get_pin_node(under_e.second));
                node2 = _nodemanager->find_node(mergenodename);
            }
        }
        else
        {
            // Other element does not have a node/wire
            if(node2)
            {
                _nodemanager->connect_node(node1->get_name(),under_e.first,under_e.second);
            }
            else if(!node1 && !node2)
            { 
                Glib::ustring newnodename = _nodemanager->add_auto_node(); // Make new node
                node2 = _nodemanager->find_node(newnodename); // Set this wire's node to the new node
                _nodemanager->connect_node(newnodename,under_e.first,under_e.second); // Connect element pin to wire node
            }
            else if(node1 && !node2)
            {
                node2 = node1;
                _nodemanager->connect_node(node2->get_name(),under_e.first,under_e.second);
            }
            else 
            {
                Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),node2->get_name());
                node2 = _nodemanager->find_node(mergenodename);
            }
        }
    }

    // Check start position for ports
    std::shared_ptr<GtkSpicePort> port = _portlist->get_port_pin_under(start);
    if(port)
    {
        // This port's pin is under our pin, connect them
        if(!node1)
        {
            node1 = _nodemanager->find_node(port->get_node_name());
            node1->add_priority(3);
        }
        else if(_nodemanager->find_node(port->get_node_name()) != node1)
        {
            Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),_nodemanager->find_node(port->get_node_name())->get_name());
            node1 = _nodemanager->find_node(mergenodename);
        }
    }
    // Check end position for ports
    port = _portlist->get_port_pin_under(end);
    if(port)
    {
        // This port's pin is under our pin, connect them
        if(!node2)
        {
            node2 = _nodemanager->find_node(port->get_node_name());
            node2->add_priority(3);
        }
        else if(_nodemanager->find_node(port->get_node_name()) != node1)
        {
            Glib::ustring mergenodename = _nodemanager->merge_nodes(node2->get_name(),_nodemanager->find_node(port->get_node_name())->get_name());
            node2 = _nodemanager->find_node(mergenodename);
        }
    }

    if(!node1 && !node2)
    {
        // Make a new node
        node = _nodemanager->find_node(_nodemanager->add_auto_node());
    }
    else if(node1 && !node2)
    {
        // Use node1
        node = node1;
    }
    else if(!node1 && node2)
    {
        // Use node2
        node = node2;
    }
    else if(node1 != node2)
    {
        Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),node2->get_name());
        node = _nodemanager->find_node(mergenodename);
    }
    else
    {
        node = node1;
    }
    
    if(node)
    {
        _wirelist->add_wire(node->get_name(),start,end);
    }

}
void SchematicSheet::add_gnd_port(Coordinate pos, bool floating)
{
    // This should add a ground port, check if the pin is connected, and if
    // it is, it should rename whatever node it is attached to, to "0".
    // If deleted, all renamed nodes should be autonamed again, using
    // ports to set node names and otherwise using automatic names
}

std::shared_ptr<GtkSpiceElement> SchematicSheet::find_element(const Glib::ustring& inst_name)
{
    return _elementlist->find_element(inst_name);
}

void SchematicSheet::move_active_objects(Coordinate delta) // TODO how to handle this?
{
    // All active objects (elements, ports, wires) should store the position
    // they were in when they were first made active. When this is called, 
    // the 'delta' (change in position) of all elements is updated. When 
    // the elements are unset being active, the delta is combined with the
    // position to get the new position; keep in mind this needs to be 
    // undoable, so starting positions of all active objects need to be 
    // stored by the Action, thus it needs access to all active objects.
}
void SchematicSheet::remove_active_objects()
{
    // This should delete all active objects (elements, ports, wires). If
    // ports are deleted, their nodes should be autonamed. If wires are
    // deleted, the nodes on either side of the wire must be autonamed,
    // unless the other side of the wire was also active and is being
    // deleted, in which case nothing happens. If elements are deleted,
    // their pin nodes should be autonamed(?).
}
void SchematicSheet::unset_active_objects()
{
    // This should set all 'active' properties to false
}
void SchematicSheet::move_floating_objects(Coordinate delta) // TODO
{
    // A floating object is one which is not instantiated yet
    // This is for dropping objects (like ports and elements)
    // onto the schematic
}
void SchematicSheet::rotate_floating_object() 
{
    // Valid when only one object is floating
}
void SchematicSheet::remove_floating_objects()
{
    // This should remove all floating objects, resulting in no
    // changes (this is for e.g. pressing escape while moving
    // new or pasted objects on the schematic)
}
void SchematicSheet::drop_floating_objects()
{
    // This should add all the floating objects (elements, ports, and
    // wires) to the SchematicSheet
}

std::vector<std::shared_ptr<GtkSpiceElement>> SchematicSheet::get_active_elements()
{
    // Return a vector of the active elements
}
std::vector<std::shared_ptr<GtkSpicePort>> SchematicSheet::get_active_ports()
{
    // Return a vector of the active ports
}

void SchematicSheet::remove_element(std::shared_ptr<GtkSpiceElement> elem)
{
    // This should remove an element, no nodes need to be autonamed
}
void SchematicSheet::remove_element(const Glib::ustring& inst_name)
{
    // This should remove an element, no nodes need to be autonamed
}
void SchematicSheet::remove_wire(std::shared_ptr<GtkSpiceWire> wire)
{
    // This should remove a wire; the nodes on either side of the wire
    // should be autonamed
}
void SchematicSheet::remove_port(std::shared_ptr<GtkSpicePort> port)
{
    // This should remove a port; the wire connected to the pin should
    // be autonamed
}

void SchematicSheet::_update_intersections()
{
    // This should be called after a wire or element is dropped or
    // moved. The list of intersection points is used to draw
    // junction squares
}

Glib::ustring GtkSpiceSchematic::get_spice_lines()
{
    Glib::ustring lines = "";
    for(auto& itr : _sheets)
    {
        lines.append(itr->get_spice_lines());
    }
    return lines;
}

void GtkSpiceSchematic::remove_sheet(int index) 
{
    if(index < _sheets.size())
    {
        if(_sheets.at(index) != _active_sheet)
            _sheets.erase(_sheets.begin() + index);
        else
        {
            _sheets.erase(_sheets.begin()+index);
            _active_sheet = _sheets.at(index-1); // Set prev sheet to active
        }
    }
}

int GtkSpiceSchematic::get_active_sheet_index() 
{
    auto itr = std::find(_sheets.begin(),_sheets.end(),_active_sheet);
    int index = std::distance(_sheets.begin(),itr);
    return index;
}

