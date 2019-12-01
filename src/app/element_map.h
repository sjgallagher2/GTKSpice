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

#ifndef ELEMENT_MAP_H
#define ELEMENT_MAP_H

#include <gtkmm.h>
#include <memory>
#include <map>
#include <app/gtkspice_object.h>

class GtkSpiceElementMap
{
public:
    GtkSpiceElementMap();
    ~GtkSpiceElementMap();

    void redraw(const Cairo::RefPtr<Cairo::Context>& context);
    void add_element(const Glib::ustring& sym_file, Coordinate pos);
    bool remove_element(const Glib::ustring& inst_name);
    std::shared_ptr<GtkSpiceElement> find_element(const Glib::ustring& inst_name);
    std::shared_ptr<GtkSpiceElement> get_active_element(); // Return element being moved/dropped
    std::shared_ptr<GtkSpiceElement> get_element_under_cursor(const Coordinate& mousepos);
    std::shared_ptr<GtkSpiceElement> get_elements_in_selection(const Coordinate& start, const Coordinate& end);

private:
    typedef std::map<Glib::ustring, std::shared_ptr<GtkSpiceElement>> ElementMap;
    typedef std::pair<Glib::ustring, std::shared_ptr<GtkSpiceElement>> ElementPair;
    ElementMap _element_map;
    void _auto_name(GtkSpiceElement& element);

    std::shared_ptr<GtkSpiceElement> _active_element = nullptr; // Active element
    std::shared_ptr<GtkSpiceElement> _floating_element = nullptr; // Element which is "floating" (being dropped)
};

#endif /* ELEMENT_LIST_H */