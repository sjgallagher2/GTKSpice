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
#include <app/gtkspice_object.h>

class GtkSpiceObjectList
{
public:
    GtkSpiceObjectList() {}
    virtual ~GtkSpiceObjectList() {}
    virtual bool empty() = 0;
    virtual int size() = 0;

    virtual void redraw(const Cairo::RefPtr<Cairo::Context>& context) = 0;

protected:
    virtual void _auto_name() = 0;
};

class GtkSpiceElementList : public GtkSpiceObjectList
{
public:
    GtkSpiceElementList();
    virtual ~GtkSpiceElementList();

    virtual bool empty() const {return _element_list.empty();}
    virtual int size() const {return _element_list.size();}

    virtual void redraw(const Cairo::RefPtr<Cairo::Context>& context);
    void add_element(const Glib::ustring& sym_file, Coordinate pos);
    bool remove_element(const Glib::ustring& inst_name);
    std::shared_ptr<GtkSpiceElement> find_element(const Glib::ustring& inst_name);
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

class GtkSpiceWireList : public GtkSpiceObjectList
{
public:
    GtkSpiceWireList();
    virtual ~GtkSpiceWireList();

    virtual bool empty();
    virtual int size();

    virtual void redraw(const Cairo::RefPtr<Cairo::Context>& context);
    void add_wire();
    bool remove_wire(const Glib::ustring& wire_name);
    std::shared_ptr<GtkSpiceWire> find_wire(const Glib::ustring& wire_name);
    std::shared_ptr<GtkSpiceWire> get_active_wire(); // Return element being moved/dropped
    std::shared_ptr<GtkSpiceWire> get_wire_under_cursor(const Coordinate& mousepos);
    std::vector<std::shared_ptr<GtkSpiceWire>> get_wires_in_selection(const Coordinate& start, const Coordinate& end);

protected:
    virtual void _auto_name(GtkSpiceWire& element);
private:
    typedef std::map<Glib::ustring, std::shared_ptr<GtkSpiceWire>> WireList;
    typedef std::pair<Glib::ustring, std::shared_ptr<GtkSpiceWire>> WirePair;
    WireList _wire_list;

    std::shared_ptr<GtkSpiceWire> _active_wire = nullptr; // Active element
};

#endif /* GTKSPICE_OBJECT_LIST_H */