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
#include <vector>
#include <app/component.h>
#include <app/geometry.h>

/*
bool Component::is_under_cursor(Coordinate mousepos)
{
    return false;
}
std::vector<float> Component::get_bounding_box()
{
    if(_bb.empty())
    {
        _bb.push_back(0);
        _bb.push_back(0);
        _bb.push_back(0);
        _bb.push_back(0);
    }
}
Point::Point(PointParameters pp) : _pp(pp)
{
}
Line::Line(LineParameters lp) : _lp(lp)
{
    _type = "line";
}
Point::~Point()
{}
Line::~Line()
{}

void Point::draw(Cairo::RefPtr<Cairo::Context> context)
{
    context->save();
    
    context->rectangle(_pp.pos.x()-0.5,_pp.pos.y()-0.5,1,1);
    context->set_source_rgba(0,0,0,1);
    context->fill();

    context->restore();
}

void Line::draw(Cairo::RefPtr<Cairo::Context> context)
{
    context->save();
    context->set_source_rgba(0,0,0,1.0);
    context->set_line_width(0.5);
    context->set_line_cap(Cairo::LINE_CAP_BUTT);

    auto t = _lp.vertices.begin();
    context->move_to((*t)->x(),(*t)->y());

    for(VIter t = _lp.vertices.begin()+1; t != _lp.vertices.end(); ++t)
    {
        context->line_to((*t)->x(),(*t)->y());
    }

    context->stroke();
    context->restore();
}

void Line::push_vertex(Coordinate pos)
{
    _lp.vertices.push_back(std::make_shared<Vertex>(pos));
    _calculate_bounding_box();
}
void Line::pop_vertex()
{
    _lp.vertices.pop_back();
    _calculate_bounding_box();
}
void Line::move_last_vertex(Coordinate new_pos)
{
    _lp.vertices.back()->set_coordinate(new_pos.x(),new_pos.y());
    _calculate_bounding_box();
}
bool Line::move_vertex(Coordinate loc)
{
    _calculate_bounding_box();
    return false;
}
*/
//bool Line::is_under_cursor(Coordinate mousepos)
//{
//    _calculate_bounding_box();
//    bool underBB = false;
//    if(!_bb.empty())
//    {
//        underBB = 
//            (mousepos.x() > _bb.at(0)) && /*left*/
//            (mousepos.y() < _bb.at(1)) && /*bottom*/
//            (mousepos.x() < _bb.at(2)) && /*right*/
//            (mousepos.y() > _bb.at(3)); /*top*/
//    }
//    if(underBB)
//    {
//        return mouse_nearby(mousepos);
//    }
//    return false;
//}
/*
bool Line::is_in_selection(const Coordinate& start, const Coordinate& end)
{
    _calculate_bounding_box();
    float left,bottom,right,top;
    if(start.x() < end.x()) 
    {
        left = start.x();
        right = end.x();
    }
    else 
    {
        left = end.x();
        right = start.x();
    }
    if(start.y() < end.y()) 
    {
        top = start.y();
        bottom = end.y();
    }
    else 
    {
        top = end.y();
        bottom = start.y();
    }
    bool overBB = false;
*/
//    if(!_bb.empty())
//    {
//        overBB = 
//            (left < _bb.at(0)) && /*left*/
//            (bottom > _bb.at(1)) &&/*bottom*/
//            (right > _bb.at(2)) &&/*right*/
//            (top < _bb.at(3)); /*top*/
//    }
//    return overBB;
//}
/*
std::vector<float> Line::get_bounding_box()
{
    return _bb;
}
bool Line::mouse_nearby(Coordinate mousepos,float tol)
{*/
    /*
    More intensive computation, check if mouse is within tol distance 
    at any point along the line
    The algorithm should determine if the mouse is "close" to the interpolated
    vertices. This means calculating a distance from many straight lines.
    */
   /*
    float d;
    for(int i = 0; i < _lp.vertices.size()-1;i++)
    {
        d = Geometry::distance_from_line(mousepos,_lp.vertices.at(i),_lp.vertices.at(i+1));
        if(d < tol)
            return true;
    }
    return false;
}*/
/*
Rect::Rect(RectParameters rp) : _rp(rp)
{
    // Init
    _type="rectangle";
}

void Rect::move_anchor(Coordinate newpos)
{
    // Move active corner
    _rp.left = newpos.x();
    _rp.top = newpos.y();
}
void Rect::move_corner(Coordinate newpos)
{
    // Move active corner
    _rp.right = newpos.x();
    _rp.bottom = newpos.y();
}

void Rect::draw(Cairo::RefPtr<Cairo::Context> context)
{
    context->save();
    context->set_source_rgba(_rp.cp.red,_rp.cp.green,_rp.cp.blue,1.0);
    context->set_line_width(_rp.cp.stroke_thickness);
    context->set_line_cap(Cairo::LINE_CAP_BUTT);
    context->set_dash(std::vector<double>{_rp.cp.stroke_thickness},0);
    context->rectangle(_rp.left,_rp.top,(_rp.right- _rp.left),(_rp.bottom-_rp.top));

    context->stroke();
    context->restore();
}
*/
/**** PRIVATE METHODS *****/
/*
void Line::_calculate_bounding_box()
{
    if(!_lp.vertices.empty())
    {
        float left = _lp.vertices.at(0)->x();
        float right = left;
        float bottom = _lp.vertices.at(0)->y();
        float top = bottom;

        for(VIter t = _lp.vertices.begin()+1; t != _lp.vertices.end();++t)
        {
            if( (*t)->x() < left)
                left = (*t)->x();
            if( (*t)->x() > right)
                right = (*t)->x();
            if( (*t)->y() < top)
                top = (*t)->y();
            if( (*t)->y() > bottom)
                bottom = (*t)->y();
        }
        if(_bb.empty())
            _bb.resize(4);
        if(left==right)
        {
            // Vertical line, add some buffer space
            left-=2;
            right+=2;
        }
        if(top==bottom)
        {
            // Horizontal line, add some buffer space
            top-=2;
            bottom+=2;
        }
        _bb.at(0) = left;
        _bb.at(1) = bottom;
        _bb.at(2) = right;
        _bb.at(3) = top;
    }
}


Line::VIter Line::_get_vertex_from_index(int index)
{
    for(VIter t = _lp.vertices.begin(); t != _lp.vertices.end(); ++t)
    {
        if((*t)->index() == index)
            return t;
    }
    return _lp.vertices.end();
}
Line::VIter Line::_get_vertex_from_location(Coordinate loc)
{
    for(VIter t = _lp.vertices.begin(); t!= _lp.vertices.end(); ++t)
    {
        if((**t) == loc)
            return t;
    }
    return _lp.vertices.end();
}
*/

