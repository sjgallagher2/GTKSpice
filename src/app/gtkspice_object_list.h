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

class GtkSpiceElementList
{
public:
    GtkSpiceElementList();
    virtual ~GtkSpiceElementList();

    virtual bool empty() const {return _element_list.empty();}
    virtual int size() const {return _element_list.size();}

    virtual void draw(const Cairo::RefPtr<Cairo::Context>& context);
    Glib::ustring add_element(const Glib::ustring& sym_file, Coordinate pos);
    bool remove_element(const Glib::ustring& inst_name);
    std::shared_ptr<GtkSpiceElement> find_element(const Glib::ustring& inst_name);
    bool set_active_element(const Glib::ustring& inst_name);
    bool set_active_element(const Coordinate& pos);
    bool set_active_element(std::shared_ptr<GtkSpiceElement> elem);
    bool has_active_element() { return (_active_element == nullptr);}
    void move_active_element(const Coordinate& pos)
    {
        if(_active_element)
            _active_element->set_position(pos);
    }
    std::shared_ptr<GtkSpiceElement> get_active_element(); // Return element being moved/dropped
    std::shared_ptr<GtkSpiceElement> get_element_under_cursor(const Coordinate& mousepos);
    std::vector<std::shared_ptr<GtkSpiceElement>> get_elements_in_selection(const Coordinate& start, const Coordinate& end);

protected:
    virtual void _auto_name(GtkSpiceElement& element);
private:
    typedef std::map<Glib::ustring, std::shared_ptr<GtkSpiceElement>> ElementList;
    typedef std::pair<Glib::ustring, std::shared_ptr<GtkSpiceElement>> ElementPair;
    ElementList _element_list;

    std::shared_ptr<GtkSpiceElement> _active_element = nullptr; // Active element
    std::shared_ptr<GtkSpiceElement> _floating_element = nullptr; // Element which is "floating" (being dropped)
};

class GtkSpiceWireList
{
public:
    GtkSpiceWireList();
    virtual ~GtkSpiceWireList();

    virtual bool empty() const {return _wire_list.empty();}
    virtual int size() const {return _wire_list.size();}

    virtual void draw(const Cairo::RefPtr<Cairo::Context>& context);
    void add_wire(std::shared_ptr<GtkSpiceNode> node, Coordinate start, Coordinate end);
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
    std::vector<std::shared_ptr<GtkSpiceWire>> get_wires_by_node(std::shared_ptr<GtkSpiceNode> node);
    std::vector<std::shared_ptr<GtkSpiceWire>> get_wires_by_node(const Glib::ustring& node_name);

    // Returns a vector of Coordinate representing Wire intersections (only when sharing node)
    std::vector<Coordinate> get_intersections();
private:
    typedef std::multimap<Glib::ustring,std::shared_ptr<GtkSpiceWire>> WireMap;
    typedef std::pair<Glib::ustring,std::shared_ptr<GtkSpiceWire>> WireMapPair;
    WireMap _wire_list; // <node name, wire>
    std::shared_ptr<GtkSpiceWire> _active_wire = nullptr; // Active element
    int _active_wire_grabpoint = 1; // 0 = start, 1 = end; determines which end of wire is being moved

    std::set<Glib::ustring> _node_names; // List of all node names
};

#endif /* GTKSPICE_OBJECT_LIST_H */