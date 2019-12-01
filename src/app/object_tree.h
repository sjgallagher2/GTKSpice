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


#ifndef OBJECT_TREE_H
#define OBJECT_TREE_H

#include <vector>
#include <memory>
#include <app/coordinate.h>
#include <app/gtkspice_object.h>


typedef std::vector<std::shared_ptr<GtkSpiceElement>> GtkSpiceTree;

/* @brief Data structure for storing and drawing objects in order
 *
 */
/*
class ObjectTree
{
public:
    ObjectTree();
    ~ObjectTree();

    void redraw(const Cairo::RefPtr<Cairo::Context>& context);

    void add_object(const Glib::ustring& sym_file, Coordinate pos);
    bool remove_component();
    bool has_active_wire();
    bool has_active_object();
    void get_object_under_cursor(Coordinate mousepos);
    void get_objects_in_selection(const Coordinate& start, const Coordinate& end);
    
private:
    std::vector<GtkSpiceTree*> _tree;
    GtkSpiceTree _objecttree;
    GtkSpiceTree _wiretree;

    int _object_auto_index;
    int _wire_auto_index;
    int register_component(std::shared_ptr<GtkSpiceObject> obj);
    void unregister_component(int index);
};
*/

#endif /* OBJECT_TREE_H */
