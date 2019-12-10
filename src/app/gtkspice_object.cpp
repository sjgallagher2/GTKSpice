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

#include <fstream>
#include <app/gtkspice_object.h>
#include <app/draw_primitives.h>
#include <app/ltspice_symbol_parser.h>
#include <spice/spice_element.h>
#include <app/geometry.h>

GtkSpiceElement::GtkSpiceElement(const Glib::ustring& symbol_file)
{
    std::ifstream sfilestrm(symbol_file,std::fstream::in);
    if(sfilestrm.good())
    {
        LTSpiceSymbolParser parser(sfilestrm);
        if(parser.parse() == 0)
            _symbol = std::make_shared<ObjectSymbol>(parser.get_symbol());
        else
            std::cerr << "Encountered error while reading file: " << symbol_file << "\n";
    }
    else
        std::cerr << "Error: Could not open file: " << symbol_file << "\n";
    
    if(_symbol)
    {
        _pin_nodes.resize(_symbol->pin_count());
        _pin_highlights.resize(_symbol->pin_count());
        for(int i = 0; i < _symbol->pin_count(); i++)
        {
            _pin_nodes.at(i) = ""; // Default to -1 (invalid node)
            _pin_highlights.at(i) = true; 
        }
    }
}

bool GtkSpiceElement::set_symbol_file(const Glib::ustring& symbol_file)
{
    std::ifstream sfilestrm(symbol_file,std::fstream::in);
    if(sfilestrm.good())
    {
        LTSpiceSymbolParser parser(sfilestrm);
        if(parser.parse() == 0)
        {
            _symbol = std::make_shared<ObjectSymbol>(parser.get_symbol());
            _pin_nodes.resize(_symbol->pin_count());
            _pin_highlights.resize(_symbol->pin_count());
            for(int i = 0; i < _symbol->pin_count(); i++)
            {
                _pin_nodes.at(i) = ""; // Default to null
                _pin_highlights.at(i) = true;
            }
            return true;
        }
        else
            std::cerr << "Encountered error while reading file: " << symbol_file << "\n";
    }
    else
        std::cerr << "Error: Could not open file: " << symbol_file << "\n";
    return false;
}

void GtkSpiceElement::set_name(Glib::ustring name)
{
    // Note: It is the Schematic's responsibility to manage names
    if(_symbol)
    {
        _name_no_prefix = name;
        _inst_name = _symbol->get_attribute_value("PREFIX") + name;
        _symbol->set_attribute_value("INSTNAME",_inst_name);
    }
}

void GtkSpiceElement::connect_pin(int pin_order, std::string node)
{
    if(pin_order < _pin_nodes.size())
    {
        _pin_nodes.at(pin_order) = node;
        _pin_highlights.at(pin_order) = false;
    }
}

void GtkSpiceElement::draw(Cairo::RefPtr<Cairo::Context> context)
{
    if(_symbol)
        _symbol->draw(context,_pin_highlights);
}

/* @brief Returns a full line (with newline) representing the element 
 * 
 * The newline is included because nothing is allowed to go after
 * the end of this directive. 
 * 
 * There is a full SPICE component formatter (see spice/spice_element.h) 
 * but this is actually more detail than we need. Most of the parameters
 * there are written in to either value, spicemodel, or spiceline.
 */
Glib::ustring GtkSpiceElement::get_spice_line()
{
    Glib::ustring spice_line = _inst_name;
    Glib::ustring elem_value = _symbol->get_attribute_value("VALUE");
    Glib::ustring elem_spicemodel = _symbol->get_attribute_value("SPICEMODEL");
    Glib::ustring elem_spiceline = _symbol->get_attribute_value("SPICELINE");

    Glib::ustring elem_nodes = "";
    for(auto& itr : _pin_nodes)
        elem_nodes.append(itr+" ");
    // Note elem_nodes ends with a space

    spice_line.append(" "+elem_nodes);
    if(!elem_value.empty())
        spice_line.append(elem_value);
    if(!elem_spicemodel.empty())
        spice_line.append(" "+elem_spicemodel);
    if(!elem_spiceline.empty())
        spice_line.append(" "+elem_spiceline);
    spice_line.append("\n");

    return spice_line.uppercase();
}

void GtkSpiceWire::draw(Cairo::RefPtr<Cairo::Context> context)
{
    context->save();
    context->set_source_rgb(0.2,0.2,0.8);
    double lw = 2;
    context->device_to_user_distance(lw,lw);
    context->set_line_width(lw);
    context->set_line_cap(Cairo::LINE_CAP_BUTT);
    context->set_line_join(Cairo::LINE_JOIN_BEVEL);
    context->set_antialias(Cairo::ANTIALIAS_NONE);

    context->move_to(_start.x(),_start.y());
    context->line_to(_end.x(),_end.y());
    context->stroke();

    context->restore();
}

void GtkSpiceNode::rename(Glib::ustring name)
{
    _name = name;
    for(auto& itr : _connections)
    {
        itr.first->connect_pin(itr.second,_name);
    }
}

void GtkSpiceNode::add_connection(std::shared_ptr<GtkSpiceElement> elem, int pin)
{
    _connections.insert(std::pair<std::shared_ptr<GtkSpiceElement>, int>(elem, pin));
    elem->connect_pin(pin,_name);
}
void GtkSpiceNode::remove_connection(std::shared_ptr<GtkSpiceElement> elem, int pin)
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

void NodeManager::rename_node(Glib::ustring node_name, Glib::ustring new_name)
{
    if( _node_map.find(node_name) != _node_map.end())
    {
        _node_map.find(node_name)->second->rename(new_name);
        std::shared_ptr<GtkSpiceNode> tnode = _node_map.find(node_name)->second;
        _node_map.erase(node_name);
        _node_map.insert(NodeKeyPair(new_name,tnode));
    }
}

void NodeManager::connect_node(Glib::ustring node_name,std::shared_ptr<GtkSpiceElement> elem,int pin_order)
{
    if( _node_map.find(node_name) != _node_map.end())
    {
        if( !elem->pin_has_node(pin_order) )
            _node_map.find(node_name)->second->add_connection(elem, pin_order);
        else
            std::cerr << "WARNING: Element pin already has a node. \n";
    }
}

void NodeManager::break_connection(Glib::ustring node_name, std::shared_ptr<GtkSpiceElement> elem, int pin_order)
{
    if( _node_map.find(node_name) != _node_map.end())
        if( elem->pin_has_node(pin_order) )
            _node_map.find(node_name)->second->remove_connection(elem, pin_order);
}


bool GtkSpiceWire::under(Coordinate pos, float tol)
{
    float dist = Geometry::distance_from_line(pos,_start,_end);
    return dist < tol;
}

bool GtkSpiceWire::within(const Coordinate& begin, const Coordinate& end)
{
    BoundingBox bb;
    bb.anchor = begin;
    bb.width = end.x()-begin.x();
    bb.height = end.y()-begin.y();
    bool ret = bb.contains(_start) | bb.contains(_end);
    return ret;
}

GtkSpiceInPort::GtkSpiceInPort(Glib::ustring node_name)
{
    _node_name = node_name;
}
GtkSpiceOutPort::GtkSpiceOutPort(Glib::ustring node_name)
{
    _node_name = node_name;
}
GtkSpiceInoutPort::GtkSpiceInoutPort(Glib::ustring node_name)
{
    _node_name = node_name;
}
GtkSpiceGlobalPort::GtkSpiceGlobalPort(Glib::ustring node_name)
{
    _node_name = node_name;
}

GtkSpiceGndPort::GtkSpiceGndPort()
{
    // Create geometry
    ObjectGeometry geom;
    ObjectPins pins;
    Coordinate pos = Coordinate(0,0);

    std::shared_ptr<LinePrimitive> line1,line2,line3,line4;
    line1 = std::make_shared<LinePrimitive>();
    line2 = std::make_shared<LinePrimitive>();
    line3 = std::make_shared<LinePrimitive>();
    line4 = std::make_shared<LinePrimitive>();
    line1->start(Coordinate(0,16));
    line1->end(Coordinate(0,24));
    line2->start(Coordinate(-8,24));
    line2->end(Coordinate(8,24));
    line3->start(Coordinate(-8,24));
    line3->end(Coordinate(0,32));
    line4->start(Coordinate(0,32));
    line4->end(Coordinate(8,24));
    geom.push_back(line1);
    geom.push_back(line2);
    geom.push_back(line3);
    geom.push_back(line4);

    std::shared_ptr<SymbolPin> pin1 = std::make_shared<SymbolPin>();
    pin1->pin_location(Coordinate(0,16));
    pin1->set_attribute_value("SPICE_ORDER","1"); // This doesn't actually have any meaning for SPICE
    pins.push_back(pin1);

    _symbol = std::make_shared<ObjectSymbol>(geom,pins,pos,true);
}

