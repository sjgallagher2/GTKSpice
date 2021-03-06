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

    _update_element_connections(elemname); // This updates all connections for this element (expensive)
    
    return elemname;
}

void SchematicSheet::add_wire(Coordinate start, Coordinate end, bool active)
{
    // This should add a wire from 'start' to 'end', checking endpoints
    // for new connections. If no new connection (no pins or wires under
    // endpoints) make a new node. Otherwise, connect to node of new
    // connection. 
    // Each coordinate (start, end, the space in between) can have one or
    // more elements, one or more wires, and one or more ports under it. 
    // We need to go through all of them.
    // If one wire has a pin over top of another's middle, a junction is created.
    // If two wires and an element are all connected at pins, a junction is created
    // representing the newest wire attaching to the older wire
    
    // Two temporary nodes are used to find a node for this wire, one for the
    // wire start and one for the wire end. A wire pin will try to set its node 
    // to the same as whatever is under it (another wire or a port) or else it 
    // uses the other pin's node or creates a new node. To add a connection to an
    // existing node, call NodeManager::connect_node(). This tells the NodeManager
    // to have the desired Node add the wire to its connections. This automatically
    // updates the wire's own node name. This should be the last thing done, after
    // any node conflicts are resolved. 

    // For elements we're connecting to, we can add connections on-the-fly, because
    // they'll be sorted out during the node merge. 

    // One more thing, the wires store linked lists of their graphical connections
    // to each other, and these need to be updated so we can traverse wires.

    // Make a wire object with a node name "-1"
    std::shared_ptr<GtkSpiceWire> new_wire = _wirelist->add_wire("",start,end);

    // Make temporary nodes
    std::shared_ptr<GtkSpiceNode> node = nullptr; // Final node
    std::shared_ptr<GtkSpiceNode> node1 = nullptr; // Start position node
    std::shared_ptr<GtkSpiceNode> node2 = nullptr; // End position node
    // We'll check later if these are the same, and we'll merge where needed

    /* START POSITION CONNECTIONS */
    // Check start position for other wires' pins or middles
    std::vector<std::pair<std::shared_ptr<GtkSpiceWire>,int>> under_w_start = _wirelist->get_wire_pins_under(start);
    std::shared_ptr<GtkSpiceWire> middle_w_start = _wirelist->get_wire_under_cursor(start);

    // Remove the new wire from the list
    for(auto it =under_w_start.begin(); it != under_w_start.end(); ++it)
    {
        if(it->first == new_wire)
        {
            under_w_start.erase(it); 
            break;
        }
    }
    if(middle_w_start == new_wire)
        middle_w_start = nullptr;


    for(auto& under_w : under_w_start) // First the wire pins under our wire's start pin
    {
        if(under_w.first != nullptr && under_w.second != -1)
        { // This wire's pin is under our wire's pin, connect them
            new_wire->add_start_wire_connection(under_w.first);
            if(under_w.second == 0)
                under_w.first->add_start_wire_connection(new_wire);
            else if(under_w.second == 1)
                under_w.first->add_end_wire_connection(new_wire);

            // Update our start node
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

            // Make sure the wire isn't used as a middle wire
            if(under_w.first == middle_w_start)
                middle_w_start = nullptr;
        }
    }
    // Next the wire middles under our wire's start pin
    if(middle_w_start != nullptr)
    {
        // Wire middle is under our start pin, connect them, add a junction
        new_wire->add_start_wire_connection(middle_w_start);
        middle_w_start->add_junc_wire_connection(new_wire);
        new_wire->set_start_junction(true);

        // Update our start node
        if(!node1)
            node1 = _nodemanager->find_node(middle_w_start->get_node_name());
        else if(_nodemanager->find_node(middle_w_start->get_node_name()) != node1)
        {
            Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),middle_w_start->get_node_name());
            node1 = _nodemanager->find_node(mergenodename);
        }

    }
    

    // Check start position for element pins
    // Note: We only need to check for one pin, because two overlapping elements are 
    // connected by a wire of length 0 which will have already been added
    // Also, if a wire was found at a position, then any element pin at the same 
    // position is already connected to that wire, so we don't need to check
    if(under_w_start.empty() && middle_w_start == nullptr)
    { // If no wire was found
        // Check for an element pin
        std::pair<std::shared_ptr<GtkSpiceElement>,int> under_e_start = _elementlist->get_pin_under(start);
        if(under_e_start.first != nullptr && under_e_start.second != -1)
        {// This element's pin is under our wire
            if(under_e_start.first->pin_has_node(under_e_start.second))
            {// Other element already has a node
                if(!node1)
                    node1 = _nodemanager->find_node(under_e_start.first->get_pin_node(under_e_start.second));
                else
                {
                    Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),under_e_start.first->get_pin_node(under_e_start.second));
                    node1 = _nodemanager->find_node(mergenodename);
                }
            }
            else
            {// Other element does not have a node/wire
                
                if(node1)
                {
                    _nodemanager->connect_node(node1->get_name(),under_e_start.first,under_e_start.second);
                }
                else if(!node1 && !node2)
                { 
                    Glib::ustring newnodename = _nodemanager->add_auto_node(); // Make new node
                    node1 = _nodemanager->find_node(newnodename); // Set this wire's node to the new node
                    _nodemanager->connect_node(newnodename,under_e_start.first,under_e_start.second); // Connect element pin to wire node
                }
                else if(!node1 && node2)
                {
                    node1 = node2;
                    _nodemanager->connect_node(node1->get_name(),under_e_start.first,under_e_start.second);
                }
                else
                {
                    Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),under_e_start.first->get_pin_node(under_e_start.second));
                    node1 = _nodemanager->find_node(mergenodename);
                }
            }
        }
    }

    // Check start position for port pins
    std::shared_ptr<GtkSpicePort> port_start = _portlist->get_port_pin_under(start);
    if(port_start)
    {
        // This port's pin is under our pin, connect them
        if(!node1)
        {
            node1 = _nodemanager->find_node(port_start->get_node_name());
            node1->add_priority(3);
        }
        else if(_nodemanager->find_node(port_start->get_node_name()) != node1)
        {
            Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),_nodemanager->find_node(port_start->get_node_name())->get_name());
            node1 = _nodemanager->find_node(mergenodename);
        }
    }

    /* END POSITION CONNECTIONS*/
    // Check end position for other wires' pins or middles
    std::vector<std::pair<std::shared_ptr<GtkSpiceWire>,int>> under_w_end = _wirelist->get_wire_pins_under(end);
    std::shared_ptr<GtkSpiceWire> middle_w_end = _wirelist->get_wire_under_cursor(end);

    // Remove the new wire from the lists
    for(auto it =under_w_end.begin(); it != under_w_end.end(); ++it)
    {
        if(it->first == new_wire)
        {
            under_w_end.erase(it); 
            break;
        }
    }
    if(middle_w_end == new_wire)
        middle_w_end = nullptr;
    
    for(auto& under_w : under_w_end) // First the wire pins under our wire's start pin
    {
        if(under_w.first != nullptr && under_w.second != -1)
        { // This wire's pin is under our wire's pin, connect them
            new_wire->add_end_wire_connection(under_w.first);
            if(under_w.second == 0)
                under_w.first->add_start_wire_connection(new_wire);
            else if(under_w.second == 1)
                under_w.first->add_end_wire_connection(new_wire);

            // Update our end node
            if(!node2)
            {
                node2 = _nodemanager->find_node(under_w.first->get_node_name());
                node2->set_priority(1);
            }
            else if(_nodemanager->find_node(under_w.first->get_node_name()) != node2)
            {
                Glib::ustring mergenodename = _nodemanager->merge_nodes(node2->get_name(),under_w.first->get_node_name());
                node2 = _nodemanager->find_node(mergenodename);
            }
            // Make sure the wire isn't used as a middle wire
            if(under_w.first == middle_w_end)
                middle_w_end = nullptr;
        }
    }
    // Next the wire middles under our wire's end pin
    if(middle_w_end != nullptr)
    {
        // Wire middle is under our start pin, connect them, add a junction
        new_wire->add_end_wire_connection(middle_w_end);
        middle_w_end->add_junc_wire_connection(new_wire);
        new_wire->set_end_junction(true);

        // Update our start node
        if(!node2)
            node2 = _nodemanager->find_node(middle_w_end->get_node_name());
        else if(_nodemanager->find_node(middle_w_end->get_node_name()) != node2)
        {
            Glib::ustring mergenodename = _nodemanager->merge_nodes(node2->get_name(),middle_w_end->get_node_name());
            node2 = _nodemanager->find_node(mergenodename);
        }

    }


    // Check end position for element pins
    if(under_w_end.empty() && middle_w_end == nullptr)
    { // If no wire was found
        // Check for an element pin
        std::pair<std::shared_ptr<GtkSpiceElement>,int> under_e_end = _elementlist->get_pin_under(end);
        if(under_e_end.first != nullptr && under_e_end.second != -1)
        {// This element's pin is under our wire
            if(under_e_end.first->pin_has_node(under_e_end.second))
            {// Other element already has a node
                if(!node2)
                    node2 = _nodemanager->find_node(under_e_end.first->get_pin_node(under_e_end.second));
                else
                {
                    Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),under_e_end.first->get_pin_node(under_e_end.second));
                    node2 = _nodemanager->find_node(mergenodename);
                }
            }
            else
            {// Other element does not have a node/wire
                
                if(node1)
                {
                    _nodemanager->connect_node(node1->get_name(),under_e_end.first,under_e_end.second);
                }
                else if(!node1 && !node2)
                { 
                    Glib::ustring newnodename = _nodemanager->add_auto_node(); // Make new node
                    node2 = _nodemanager->find_node(newnodename); // Set this wire's node to the new node
                    _nodemanager->connect_node(newnodename,under_e_end.first,under_e_end.second); // Connect element pin to wire node
                }
                else if(!node1 && node2)
                {
                    node2 = node1;
                    _nodemanager->connect_node(node1->get_name(),under_e_end.first,under_e_end.second);
                }
                else
                {
                    Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),under_e_end.first->get_pin_node(under_e_end.second));
                    node2 = _nodemanager->find_node(mergenodename);
                }
            }
        }
    }

    // Check end position for port pins
    std::shared_ptr<GtkSpicePort> port_end = _portlist->get_port_pin_under(end);
    if(port_end)
    {
        // This port's pin is under our pin, connect them
        if(!node1)
        {
            node1 = _nodemanager->find_node(port_end->get_node_name());
            node1->add_priority(3);
        }
        else if(_nodemanager->find_node(port_end->get_node_name()) != node1)
        {
            Glib::ustring mergenodename = _nodemanager->merge_nodes(node1->get_name(),_nodemanager->find_node(port_end->get_node_name())->get_name());
            node1 = _nodemanager->find_node(mergenodename);
        }
    }

    /* NODE UPDATE */
    // Find node name
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

    /* WIRE UPDATE */
    // Update the final wire
    if(node)
    {
        _nodemanager->connect_node(node->get_name(),new_wire);

        auto elem_pin_pair_vec = _elementlist->find_pins_on_wire(new_wire,true);
        for(auto& itr : elem_pin_pair_vec)
        {
            // Check element pins to see if their connections have changed
            _update_pin_wire_connections(itr.first,itr.second);
        }
    }
}


void SchematicSheet::add_gnd_port(Coordinate pos, bool floating)
{
    // This should add a ground port, check if the pin is connected, and if
    // it is, it should rename whatever node it is attached to, to "0".
    // If deleted, all renamed nodes should be autonamed again, using
    // ports to set node names and otherwise using automatic names
    auto port = _portlist->add_gnd_port(pos);
    //std::cout << "Node manager has node 0? " << (_nodemanager->find_node("0") == nullptr) << "\n";
    _nodemanager->add_node(port->get_node_name());
    _nodemanager->find_node(port->get_node_name())->set_priority(3);
    _update_port_connections(port);
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
    _elementlist->remove_element(elem->get_inst_name());
}
void SchematicSheet::remove_element(const Glib::ustring& inst_name)
{
    // This should remove an element, no nodes need to be autonamed
    _elementlist->remove_element(inst_name);
}
void SchematicSheet::remove_wire(std::shared_ptr<GtkSpiceWire> wire)
{
    // This should remove a wire; the nodes on either side of the wire
    // should be autonamed
    Glib::ustring nodename = wire->get_node_name();
    auto wirenode = _nodemanager->find_node(nodename);

    // Remove wire
    _wirelist->remove_wire(wire);

    // Update wire and element connections
}
void SchematicSheet::remove_port(std::shared_ptr<GtkSpicePort> port)
{
    // This should remove a port; the wire connected to the pin should
    // be renamed (autonamed or adapted to other port if, for some reason,
    // two ports are connected together)
    
    // Get port node
    Glib::ustring nodename = port->get_node_name();

    // Remove port
    _portlist->remove_port(port);

    // Rename the node
    _nodemanager->rename_node(nodename);
}

void SchematicSheet::_update_pin_element_connections(Glib::ustring elemname, std::shared_ptr<SymbolPin> pin)
{
    Coordinate pin_pos = pin->pin_location() + _elementlist->find_element(elemname)->get_position();
    
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
            newwire->set_start_junction(true);
            _nodemanager->connect_node(node,_elementlist->find_element(elemname),std::stoi(pin->get_attribute_value("SPICE_ORDER")));
            _nodemanager->connect_node(node,newwire);
            _nodemanager->find_node(node)->add_priority(2);
        }
        else
        {
            // Neither element has a node/wire
            Glib::ustring newnode = _nodemanager->add_auto_node();
            std::shared_ptr<GtkSpiceWire> newwire = _wirelist->add_wire(newnode,pin_pos,pin_pos);
            _nodemanager->connect_node(newnode,_elementlist->find_element(elemname),std::stoi(pin->get_attribute_value("SPICE_ORDER")));
            _nodemanager->connect_node(newnode,under_e.first,under_e.second);
            _nodemanager->connect_node(newnode,newwire);
            _nodemanager->find_node(newnode)->add_priority(2);
        }
    }
}
void SchematicSheet::_update_pin_wire_connections(Glib::ustring elemname, std::shared_ptr<SymbolPin> pin)
{
    Coordinate pin_pos = pin->pin_location() + _elementlist->find_element(elemname)->get_position();

    std::vector<std::pair<std::shared_ptr<GtkSpiceWire>,int>> under_w_vec;
    under_w_vec = _wirelist->get_wire_pins_under(pin_pos);
    for(auto& under_w : under_w_vec)
    {
        if(under_w.first != nullptr && under_w.second != -1)
        {
            // This wire's pin is under our pin
            // Connect them
            _nodemanager->connect_node(under_w.first->get_node_name() ,_elementlist->find_element(elemname),std::stoi(pin->get_attribute_value("SPICE_ORDER")));
            _nodemanager->find_node(under_w.first->get_node_name())->add_priority(2);
            break;
        }
        else
        {
            if(std::shared_ptr<GtkSpiceWire> wire = _wirelist->get_wire_under_cursor(pin_pos))
            {
                // This wire is under our pin as a junction
                // Connect them with a wire of length zero, add intersection
                std::shared_ptr<GtkSpiceWire> newwire = _wirelist->add_wire(wire->get_node_name(),pin_pos,pin_pos);
                if(under_w.second == 0)
                    newwire->set_start_junction(true);
                else if(under_w.second == 1)
                    newwire->set_end_junction(true);
                _nodemanager->connect_node(wire->get_node_name(),_elementlist->find_element(elemname),std::stoi(pin->get_attribute_value("SPICE_ORDER")));
                _nodemanager->connect_node(wire->get_node_name(),newwire);
                _nodemanager->find_node(wire->get_node_name())->add_priority(2);
            }
        }
    }
}
void SchematicSheet::_update_pin_port_connections(Glib::ustring elemname, std::shared_ptr<SymbolPin> pin)
{
    Coordinate pin_pos = pin->pin_location() + _elementlist->find_element(elemname)->get_position();

    std::shared_ptr<GtkSpicePort> port = _portlist->get_port_pin_under(pin_pos);
    if(port)
    {
        // This port's pin is under our pin
        // Connect them with a wire of length zero
        std::shared_ptr<GtkSpiceWire> wire = _wirelist->add_wire(port->get_node_name(),pin_pos,pin_pos);
        _nodemanager->connect_node(port->get_node_name(),_elementlist->find_element(elemname),std::stoi(pin->get_attribute_value("SPICE_ORDER")));
        _nodemanager->connect_node(port->get_node_name(),wire);
        _nodemanager->find_node(port->get_node_name())->add_priority(3);
    }
}

void SchematicSheet::_update_element_connections(Glib::ustring elemname)
{
    auto pins = _elementlist->get_element_pins(elemname);
    
    for(auto itr = pins->begin(); itr != pins->end(); ++itr)
    {
        // Iterating over pins
        // Check other elements
        _update_pin_element_connections(elemname,*itr);

        // Check wires
        _update_pin_wire_connections(elemname,*itr);

        // Check ports
        _update_pin_port_connections(elemname,*itr);
    }
}
void SchematicSheet::_update_port_connections(std::shared_ptr<GtkSpicePort> port)
{    
    Coordinate pin_pos = port->get_pin_position() + port->get_position();
    
    // Check for elements
    std::pair<std::shared_ptr<GtkSpiceElement>,int> under_e;
    under_e = _elementlist->get_pin_under(pin_pos);
    if(under_e.first != nullptr && under_e.second != -1)
    {
        // This element's pin is under our pin
        // Connect them with a wire of length zero
        if(under_e.first->pin_has_node(under_e.second))
        {
            // Other element already has a node/wire
            Glib::ustring other_node = under_e.first->get_pin_node(under_e.second);
            _nodemanager->merge_nodes(port->get_node_name(),other_node); // Always prefers our port node name
            std::shared_ptr<GtkSpiceWire> newwire = _wirelist->add_wire(port->get_node_name(),pin_pos,pin_pos);
            newwire->set_start_junction(true);
            port->set_highlight(false);
        }
        else
        {
            // Other element doesn't have a node
            std::shared_ptr<GtkSpiceWire> newwire = _wirelist->add_wire(port->get_node_name(),pin_pos,pin_pos);
            newwire->set_start_junction(true);
            _nodemanager->connect_node(port->get_node_name(),under_e.first,under_e.second);
            _nodemanager->connect_node(port->get_node_name(),newwire);
            _nodemanager->find_node(port->get_node_name())->add_priority(3);
            port->set_highlight(false);
        }
    }

    // Check for wires
    std::vector<std::pair<std::shared_ptr<GtkSpiceWire>,int>> under_w_vec;
    under_w_vec = _wirelist->get_wire_pins_under(pin_pos);
    for(auto& under_w : under_w_vec)
    {
        if(under_w.first != nullptr && under_w.second != -1)
        {
            // This wire's pin is under our pin
            // Merge nodes, connect wire and port
            Glib::ustring other_node = under_w.first->get_node_name();
            _nodemanager->merge_nodes(port->get_node_name(),other_node); // Always prefers our port node name
            std::shared_ptr<GtkSpiceWire> newwire = _wirelist->add_wire(port->get_node_name(),pin_pos,pin_pos);
            _nodemanager->connect_node(port->get_node_name(),newwire);
            port->set_highlight(false);
        }
        else
        {
            if(std::shared_ptr<GtkSpiceWire> wire = _wirelist->get_wire_under_cursor(pin_pos))
            {
                // This wire is under our pin as a junction
                // Connect them with a wire of length zero, add intersection?
                Glib::ustring other_node = wire->get_node_name();
                _nodemanager->merge_nodes(port->get_node_name(),other_node); // Always prefers our port node name
                std::shared_ptr<GtkSpiceWire> newwire = _wirelist->add_wire(port->get_node_name(),pin_pos,pin_pos);
                if(under_w.second == 0)
                    newwire->set_start_junction(true);
                else if(under_w.second == 1)
                    newwire->set_end_junction(true);
                port->set_highlight(false);
            }
        }
    }


    // Check for other ports (hopefully none)
    std::shared_ptr<GtkSpicePort> other_port = _portlist->get_port_pin_under(pin_pos);
    if(other_port)
    {
        // This port's pin is under our pin
        // Connect them with a wire of length zero
        Glib::ustring other_node = other_port->get_node_name();
        _nodemanager->merge_nodes(port->get_node_name(),other_node); // Always prefers our port node name
        std::shared_ptr<GtkSpiceWire> newwire = _wirelist->add_wire(port->get_node_name(),pin_pos,pin_pos);
        _nodemanager->connect_node(port->get_node_name(),newwire);
        port->set_highlight(false);
    }

    
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

