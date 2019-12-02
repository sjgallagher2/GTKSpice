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
#include <vector>
#include <algorithm>
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
    for(auto& itr : _element_map)
        if(itr.second->near(mousepos))
            if(itr.second->under(mousepos))
                return itr.second;
    return nullptr;
}
std::vector<std::shared_ptr<GtkSpiceElement>> GtkSpiceElementMap::get_elements_in_selection(const Coordinate& start, const Coordinate& end)
{
    std::vector<std::shared_ptr<GtkSpiceElement>> ret;
    for(auto& itr : _element_map)
        if(itr.second->within(start,end))
            ret.push_back(itr.second);
    return ret;
}

void GtkSpiceElementMap::_auto_name(GtkSpiceElement& element)
{
    // Use element prefix to get first available name (default names are numbers) 
    // Note: use std::map's lower_bound and upper_bound with the prefix to 
    // get a range to search within, and then iterate through this range
    // checking object names. Use std::stoi to check if name is a number 
    // before recording into a vector and sorting. Find first missing number
    // and use as the name.

    Glib::ustring prefix = element.get_symbol()->get_attribute_value("PREFIX");

    int element_namei = 0;
    std::vector<int> namesi;

    // Fill in a vector of the names as ints
    if(_element_map.lower_bound(prefix) != _element_map.upper_bound(prefix))
    {
        for(auto itr = _element_map.lower_bound(prefix); itr != _element_map.upper_bound(prefix); ++itr)
        {
            size_t next_char;
            int current_namei;
            try
            {
                // Try/catch is for stoi if name is not a number
                current_namei = std::stoi(_element_map.lower_bound(prefix)->second->get_inst_name(),&next_char);
                if (next_char == 0)
                    namesi.push_back(current_namei);
            }catch (std::invalid_argument){}
        }
    }
    std::sort( namesi.begin(), namesi.end() );
    // This iterates through all values in the list assuming no gaps, and does a binary
    // search to determine if the name is taken. If it isn't, take it.
    // Note that we iterate from 0 to size() inclusion, so if all names are taken so far,
    // the next name (incrementally) is used.
    for(int m = 0; m <= namesi.size(); m++)
        if(std::binary_search(namesi.begin(),namesi.end(),m) == false)
            element_namei = m;
    
    element.set_name(std::to_string(element_namei));
}

