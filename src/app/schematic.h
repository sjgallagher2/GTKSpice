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
    
    std::shared_ptr<GtkSpiceElement> get_element_under_cursor(Coordinate pos);
    std::shared_ptr<GtkSpiceWire> get_wire_under_cursor(Coordinate pos);

private:
    std::shared_ptr<GtkSpiceElementList> _elementlist;
    std::shared_ptr<GtkSpiceWireList> _wirelist;
    std::shared_ptr<GtkSpicePortList> _portlist;
    std::shared_ptr<NodeManager> _nodemanager;
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

    int add_sheet() {_sheets.push_back(std::make_shared<SchematicSheet>());return _sheets.size()-1;}
    void remove_sheet(int index) 
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
    std::shared_ptr<SchematicSheet> get_sheet(int index) {if(index < _sheets.size()) return _sheets.at(index);}

    void set_active_sheet(int index) {if(index < _sheets.size()) _active_sheet = _sheets.at(index);}
    std::shared_ptr<SchematicSheet> get_active_sheet() {return _active_sheet;}
    int get_active_sheet_index() 
    {
        auto itr = std::find(_sheets.begin(),_sheets.end(),_active_sheet);
        int index = std::distance(_sheets.begin(),itr);
        return index;
    }

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
