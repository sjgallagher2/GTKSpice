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

class ObjectTree
{
public:
    static ObjectTree* Instance();
    static void init();
    ~ObjectTree();

    static void redraw(const Cairo::RefPtr<Cairo::Context>& context);

    static int add_component(PointParameters pp);
    static int add_component(LineParameters lp);

    static bool remove_component(Glib::ustring type,int index);

    static LineParameters get_line_parameters(int index);
    // Move the active vertex of the active line
    static bool move_line_vertex(Coordinate pos);
    // NOTE: If you want to move a line's vertex outside of line editing,
    // use a move vertex action
    static bool add_line_vertex(Coordinate pos);
    static bool remove_line_vertex(Coordinate pos);
    static void finish_line(); // Remove floating vertex and deactivate
    static void set_line_active(int index,bool active = true);
    static void set_no_line_active();

    static bool has_active_line();
    static bool has_active_point();

    static int get_line_under_cursor(Coordinate mousepos);

    typedef std::vector<std::shared_ptr<Component>> Tree;
    typedef Tree::iterator OIter;
    
private:
    ObjectTree() {}

    static ObjectTree* _ot;
    static std::vector<Tree*> _tree;
    static Tree _pointtree;
    static Tree _linetree;

    static int _point_auto_index;
    static int _line_auto_index;

    static OIter _get_active_line();
};

#endif /* OBJECT_TREE_H */
