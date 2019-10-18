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

/* Data structure for storing and drawing objects in order
 *
 */

#ifndef OBJECT_TREE_H
#define OBJECT_TREE_H

#include <stack>
#include <memory>
#include <app/component.h>
#include <app/coordinate.h>

// TODO: Un-Singleton

class ObjectTree
{
public:
    ObjectTree();
    ~ObjectTree();

    void redraw(const Cairo::RefPtr<Cairo::Context>& context);

    int add_component(PointParameters pp);
    int add_component(LineParameters lp);

    bool remove_component(Glib::ustring type,int index);

    LineParameters get_line_parameters(int index);
    // Move the active vertex of the active line
    bool move_line_vertex(Coordinate pos);
    // NOTE: If you want to move a line's vertex outside of line editing,
    // use a move vertex action
    bool add_line_vertex(Coordinate pos);
    bool remove_line_vertex(Coordinate pos);
    void finish_line(); // Remove floating vertex and deactivate
    void set_line_active(int index,bool active = true);
    void set_no_line_active();

    bool has_active_line();
    bool has_active_point();

    int get_line_under_cursor(Coordinate mousepos);

    typedef std::vector<std::shared_ptr<Component>> Tree;
    typedef Tree::iterator OIter;
    
private:
    std::vector<Tree*> _tree;
    Tree _pointtree;
    Tree _linetree;

    int _point_auto_index;
    int _line_auto_index;

    OIter _get_active_line();
};

#endif /* OBJECT_TREE_H */
