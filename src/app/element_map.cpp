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

#include <string>
#include <app/element_map.h>

GtkSpiceElementMap::GtkSpiceElementMap()
{
}

GtkSpiceElementMap::~GtkSpiceElementMap()
{
}

void GtkSpiceElementMap::redraw(const Cairo::RefPtr<Cairo::Context>& context)
{
    for(auto t = _element_map.begin(); t != _element_map.end(); ++t)
    {
        t->second->draw(context);
    }
}

void GtkSpiceElementMap::add_element(const Glib::ustring& sym_file, Coordinate pos)
{
    GtkSpiceElement obj(sym_file);
    _auto_name(obj);
    _element_map.insert(ElementPair(obj.get_inst_name(),std::make_shared<GtkSpiceElement>(obj)));
}
bool GtkSpiceElementMap::remove_element(const Glib::ustring& inst_name)
{
    if(_element_map.find(inst_name) != _element_map.end())
    {
        _element_map.erase(inst_name);
        return true;
    }
    return false;
}
std::shared_ptr<GtkSpiceElement> GtkSpiceElementMap::find_element(const Glib::ustring& inst_name)
{
    if(_element_map.find(inst_name) != _element_map.end())
        return _element_map.find(inst_name)->second;
    return nullptr;
    
}
std::shared_ptr<GtkSpiceElement> GtkSpiceElementMap::get_active_element()
{
    return _active_element;
}
std::shared_ptr<GtkSpiceElement> GtkSpiceElementMap::get_element_under_cursor(const Coordinate& mousepos)
{
    // TODO Iterate through all elements' near() and under() functions
    return nullptr;
}
std::shared_ptr<GtkSpiceElement> GtkSpiceElementMap::get_elements_in_selection(const Coordinate& start, const Coordinate& end)
{
    // TODO Iterate through all elements' within() functions
    return nullptr;
}

void _auto_name(GtkSpiceElement& element)
{
    // Use element prefix to get first available name (default names are numbers) 
}


