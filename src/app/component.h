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

/* Base class for selectable objects that can be drawn. 
 *
 */

#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <vector>
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <app/vertex_list.h>
#include <app/component_params.h>

// TODO Add wire, circuitelement, subcircuit

class Component;
class Point;
class Line;

class ComponentFactory
{
public:
    enum TYPE {POINT,LINE,WIRE};

    template<typename T>
    std::shared_ptr<Component> CreateComponent(const ComponentFactory::TYPE& type, T params)
    {
        std::shared_ptr<Component> component;
        switch(type)
        {
        case ComponentFactory::POINT:
            std::make_shared<Point>(params);
            break;
        case ComponentFactory::LINE:
            std::make_shared<Line>(params);
            break;
        }

        return component;
    }
};

class Component
{
public:
    Component() {}
    Component(ComponentParameters cp);
    virtual ~Component() {}

    virtual void draw(Cairo::RefPtr<Cairo::Context> context)  {}

    virtual void index(int i) {_cp.index = i;}
    virtual int index() const {return _cp.index;}
    virtual void label(Glib::ustring l) {_cp.label = l;}
    virtual Glib::ustring label() const {return _cp.label;}
    virtual Glib::ustring type() const {return _type;}

    virtual bool is_under_cursor(Coordinate mousepos);
    virtual std::vector<float> get_bounding_box();
    

protected:
    Glib::ustring _type = "none";
    std::vector<float> _bb; // Bounding box, [left,bottom,right,top]
private:
    ComponentParameters _cp;
};

class Point : public Component
{
public:
    Point(PointParameters pp);
    virtual ~Point();

    void draw(Cairo::RefPtr<Cairo::Context> context);
private:
    PointParameters _pp;
    Coordinate _pos;
};


class Line : public Component
{
public:
    Line(LineParameters lp);
    virtual ~Line();

    virtual void draw(Cairo::RefPtr<Cairo::Context> context);

    virtual bool empty() {return _lp.vertices.size() <= 1;}
    virtual void push_vertex(Coordinate pos);
    virtual void pop_vertex();
    virtual void move_last_vertex(Coordinate new_pos);
    virtual bool move_vertex(Coordinate loc);

    virtual void index(int i) {_lp.cp.index = i;}
    virtual int index() const {return _lp.cp.index;}
    virtual void label(Glib::ustring l) {_lp.cp.label = l;}
    virtual Glib::ustring label() const {return _lp.cp.label;}
    virtual bool active() const {return _is_active;}
    virtual void active(bool a) {_is_active = a;}
    virtual bool is_under_cursor(Coordinate mousepos);
    virtual std::vector<float> get_bounding_box();
    bool mouse_nearby(Coordinate mousepos,float tol=2);

    virtual LineParameters line_parameters() const {return _lp;}

    typedef VertexList::iterator VIter;
protected:
    LineParameters _lp;
    bool _is_active = false;
    VIter _get_vertex_from_location(Coordinate loc); // Get a vertex object of a vertex at a specified location
    VIter _get_vertex_from_index(int index); // Get a vertex from its index
    void _calculate_bounding_box();
};

#endif /* COMPONENT_H */