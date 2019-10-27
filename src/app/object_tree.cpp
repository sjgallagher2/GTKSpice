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

#include <iostream>
#include <app/object_tree.h>

// TODO This should not be indexing lines, change so it only accepts new
// objects, other objects can use the ComponentFactory to make new component
// shared_pointers?
// Implement a register() function for indices
// This interacts closely with Action

ObjectTree::ObjectTree() : _point_auto_index(0), _line_auto_index(0)
{
    _tree.push_back(&_pointtree);
    _tree.push_back(&_linetree);
}

ObjectTree::~ObjectTree()
{
}

void ObjectTree::redraw(const Cairo::RefPtr<Cairo::Context>& context)
{
    for(auto t = _tree.begin(); t != _tree.end(); ++t)
    {
        for(OIter i = (*t)->begin(); i != (*t)->end(); ++i)
        {
            (*i)->draw(context);
        }
    }
}

void ObjectTree::add_component(PointParameters pp)
{
    std::shared_ptr<Component> point;
    point = _factory->CreateComponent(ComponentFactory::POINT,pp);

    register_component(point);
    _pointtree.push_back(point);
}
void ObjectTree::add_component(LineParameters lp)
{
    std::shared_ptr<Component> line;
    line = _factory->CreateComponent(ComponentFactory::LINE,lp);

    register_component(line);
    _linetree.push_back(line);
}


int ObjectTree::register_component(std::shared_ptr<Component> component)
{
    if(component->index() == -1)
    {
        if(component->type() == "point")
            component->index(_point_auto_index);
        else if(component->type() == "line")
            component->index(_line_auto_index);
    }
    else
    {
        if(component->type() == "point")
            for(OIter t = _pointtree.begin(); t != _pointtree.end();++t)
                if((*t)->index() == component->index())
                    return -1;
        else if (component->type() == "line")
            for(OIter t = _linetree.begin(); t != _linetree.end();++t)
                if((*t)->index() == component->index())
                    return -1;
    }
    return component->index();
}

bool ObjectTree::remove_component(Glib::ustring type, int index)
{
    if(type.compare("line")==0)
        for(OIter t = _linetree.begin(); t != _linetree.end(); ++t)
            if((*t)->index() == index)
            {
                _linetree.erase(t);
                return true;
            }
    else if(type.compare("point")==0)
        for(OIter t = _pointtree.begin(); t != _pointtree.end(); ++t)
            if((*t)->index() == index)
            {
                _pointtree.erase(t);
                return true;
            }
    return false;
}

LineParameters ObjectTree::get_line_parameters(int index)
{
    for(OIter t = _linetree.begin(); t!= _linetree.end(); ++t)
        if((*t)->index() == index)
            return std::static_pointer_cast<Line>(*t)->line_parameters();
}

/* ACTIVE LINE CONTROL */
bool ObjectTree::move_line_vertex(Coordinate pos)
{
    OIter activei = _get_active_line();
    if(activei != _linetree.end())
        std::static_pointer_cast<Line>(*activei)->move_last_vertex(pos);
    else
        return false;
    return true;
}
bool ObjectTree::add_line_vertex(Coordinate pos)
{
    OIter activei = _get_active_line();
    if(activei != _linetree.end())
        std::static_pointer_cast<Line>(*activei)->push_vertex(pos);
    else 
        return false;
    return true;
}
bool ObjectTree::remove_line_vertex(Coordinate pos) // Position is where we should be after removal
{
    OIter activei = _get_active_line();
    if(activei != _linetree.end())
    {
        std::shared_ptr<Line> a = std::static_pointer_cast<Line>(*activei);
        a->pop_vertex();
        if(a->empty())
            _linetree.pop_back();
        else
            a->move_last_vertex(pos);
    }
    else 
        return false;
    return true;
}
void ObjectTree::finish_line()
{
    OIter activei = _get_active_line();
    if(activei != _linetree.end())
    {
        std::shared_ptr<Line> a = std::static_pointer_cast<Line>(*activei);
        a->pop_vertex();
        if(a->empty())
            _linetree.pop_back();
        else
            a->active(false);
    }
}
void ObjectTree::set_line_active(int index,bool active)
{
    for(auto t = _linetree.begin(); t != _linetree.end(); ++t)
        if(std::static_pointer_cast<Line>(*t)->index() == index)
            std::static_pointer_cast<Line>(*t)->active(active);
}
void ObjectTree::set_no_line_active()
{
    for(auto t = _linetree.begin(); t != _linetree.end(); ++t)
        std::static_pointer_cast<Line>(*t)->active(false);
}

bool ObjectTree::has_active_line()
{
    if(_get_active_line() != _linetree.end())
        return true;
    return false;
}

// TODO Make this general for components
int ObjectTree::get_line_under_cursor(Coordinate mousepos)
{
    if(!_linetree.empty())
    {
    /*
    */
        for(OIter t = _linetree.end()-1; t >= _linetree.begin(); --t)
        {
            if(std::static_pointer_cast<Line>(*t)->is_under_cursor(mousepos))
                return (*t)->index();
        }
    }
    return -1;
}

ObjectTree::OIter ObjectTree::_get_active_line()
{
    if(!_linetree.empty())
        for(OIter t = _linetree.begin(); t!= _linetree.end();++t)
            if(std::static_pointer_cast<Line>(*t)->active())
                return t;
    return _linetree.end();
}

