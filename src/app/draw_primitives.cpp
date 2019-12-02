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

#include <app/draw_primitives.h>
#include <app/geometry.h>
#include <algorithm>
#include <cmath>

void draw_bb(Cairo::RefPtr<Cairo::Context> context, Coordinate pos, const DrawSettings& ds, const BoundingBox& bb)
{
		context->save();
		context->set_source_rgb(0.9,0.1,0.1); // BBOX HIGHLIGHT COLOR
        double lw = ds.line_width;
        context->device_to_user_distance(lw,lw);
		context->set_line_width(lw);
		context->set_line_cap(ds.line_cap);
		context->set_line_join(ds.line_join);
		context->set_antialias(ds.antialias);

        context->move_to(pos.x(),pos.y());

        context->rel_move_to(bb.anchor.x(),bb.anchor.y());
        context->rel_line_to(0,bb.height); // Left wall
        context->rel_line_to(bb.width,0); // Top wall
        context->rel_line_to(0,-bb.height); // Right wall
        context->close_path(); // Bottom wall

		context->stroke();
		context->restore();

}


void LinePrimitive::draw(Cairo::RefPtr<Cairo::Context> context, 
    Coordinate pos, const DrawSettings& ds)
{
    context->save();
    context->set_source_rgb(ds.red,ds.green,ds.blue);
    double lw = ds.line_width;
    context->device_to_user_distance(lw,lw);
    context->set_line_width(lw);
    context->set_line_cap(ds.line_cap);
    context->set_line_join(ds.line_join);
    context->set_antialias(ds.antialias);

    context->move_to(pos.x(),pos.y());

    context->rel_move_to(_start.x(),_start.y());
    context->rel_line_to(_end.x()-_start.x(),_end.y()-_start.y());
    context->stroke();

    //draw_bb(context,pos,ds,get_bounding_box());

    context->restore();
}
bool LinePrimitive::under(Coordinate pos, float tol)
{
    // Check if the line is under pos
    float dist = Geometry::distance_from_line(pos,_start,_end);
    return dist < tol;
}
void LinePrimitive::rotate90()
{
    // Rotate 90 degrees about origin
    Coordinate tstart = _start;
    Coordinate tend = _end;
    _start.x(-_start.y());
    _start.y(tstart.x());
    _end.x(-_end.y());
    _end.y(tend.x());
}
void LinePrimitive::hflip()
{
    // Mirror horizontally across origin
    _start.x(-_start.x());
    _end.x(-_end.x());
}
void LinePrimitive::vflip()
{
    // Mirror vertically across origin
    _start.y(-_start.y());
    _end.y(-_end.y());
}

void RectPrimitive::draw(Cairo::RefPtr<Cairo::Context> context, 
    Coordinate pos, const DrawSettings& ds)
{
    context->save();
    context->set_source_rgb(ds.red,ds.green,ds.blue);
    double lw = ds.line_width;
    context->device_to_user_distance(lw,lw);
    context->set_line_width(lw);
    context->set_line_cap(ds.line_cap);
    context->set_line_join(ds.line_join);
    context->set_antialias(ds.antialias);

    context->move_to(pos.x(),pos.y());
    
    context->rel_move_to(_anchor.x(),_anchor.y());
    context->rel_line_to(0,_height); // Left wall
    context->rel_line_to(_width,0); // Top wall
    context->rel_line_to(0,-_height); // Right wall
    context->close_path(); // Bottom wall
    context->stroke();
    
    //draw_bb(context,pos,ds,get_bounding_box());

    context->restore();
}
bool RectPrimitive::under(Coordinate pos, float tol)
{
    // Check if the line is under pos
    float dist = Geometry::distance_from_rect(pos,_anchor,_width,_height);
    return dist < tol;
}
void RectPrimitive::rotate90()
{
    // Rotate about origin
    Coordinate tanchor = _anchor;
    _anchor.x(-_anchor.y());
    _anchor.y(tanchor.x());
    double twidth = _width;
    _width = -_height;
    _height = twidth;
}
void RectPrimitive::hflip()
{
    // Mirror horizontally across origin
    _anchor.x(_anchor.x());
    _width = -_width;
}
void RectPrimitive::vflip()
{
    // Mirror vertically across origin
    _anchor.y(_anchor.y());
    _height = -_height;
}

void ArcPrimitive::draw(Cairo::RefPtr<Cairo::Context> context, 
    Coordinate pos, const DrawSettings& ds)
{
    context->save();
    context->set_source_rgb(ds.red,ds.green,ds.blue);
    double lw = ds.line_width;
    context->device_to_user_distance(lw,lw);
    context->set_line_width(lw);
    context->set_line_cap(ds.line_cap);
    context->set_line_join(ds.line_join);
    context->set_antialias(ds.antialias);

    Cairo::Matrix save_matrix = context->get_matrix();
    context->translate(pos.x()+_center.x(),pos.y()+_center.y());
    context->scale(1,_vradius/_hradius);
    context->translate(-pos.x()-_center.x(),-pos.y()-_center.y());
    context->arc(pos.x()+_center.x(),pos.y()+_center.y(),_hradius,
        _start_angle_deg*M_PI/180,_end_angle_deg*M_PI/180);
    context->set_matrix(save_matrix);

    
    context->stroke();
    
    //draw_bb(context,pos,ds,get_bounding_box());

    context->restore();
}
bool ArcPrimitive::under(Coordinate pos, float tol)
{
    // Check if the line is under pos
    // TODO Elliptical arcs
    if(_vradius - _hradius < tol) // Nearly circular
    {
        float dist = Geometry::distance_from_arc(pos,_center,_vradius,_start_angle_deg,_end_angle_deg);
        return dist < tol;
    }
    else
        return false;
}
void ArcPrimitive::rotate90()
{
    // Rotate about origin
    Coordinate tcenter = _center;
    _center.x(-_center.y());
    _center.y(tcenter.x());
    _start_angle_deg += 90;
    _end_angle_deg += 90;
    if(_end_angle_deg >= 360)
    {
        _start_angle_deg -= 360;
        _end_angle_deg -= 360;
    }
}
void ArcPrimitive::hflip()
{
    // Mirror horizontally across origin
    _center.x(-_center.x());
    
    double tstart_angle = _start_angle_deg;
    _start_angle_deg = 180 - _end_angle_deg;
    _end_angle_deg = 180 - tstart_angle;
    if(_end_angle_deg <= -360)
    {
        _start_angle_deg += 360;
        _end_angle_deg += 360;
    }
}
void ArcPrimitive::vflip()
{
    // Mirror vertically across origin
    _center.y(-_center.y());
    
    double tstart_angle = _start_angle_deg;
    _start_angle_deg = -_end_angle_deg;
    _end_angle_deg = -tstart_angle;
}

void CirclePrimitive::draw(Cairo::RefPtr<Cairo::Context> context, 
    Coordinate pos, const DrawSettings& ds)
{
    context->save();
    context->set_source_rgb(ds.red,ds.green,ds.blue);
    double lw = ds.line_width;
    context->device_to_user_distance(lw,lw);
    context->set_line_width(lw);
    context->set_line_cap(ds.line_cap);
    context->set_line_join(ds.line_join);
    context->set_antialias(ds.antialias);

    Cairo::Matrix save_matrix = context->get_matrix();
    // With ellipse support (but why someone would use ellipses in a symbol...)
    context->translate(pos.x()+_center.x(),pos.y()+_center.y());
    context->scale(1,_vradius/_hradius);
    context->translate(-pos.x()-_center.x(),-pos.y()-_center.y());
    context->arc(pos.x()+_center.x(),pos.y()+_center.y(),_hradius,0,2*M_PI);
    context->set_matrix(save_matrix);
    context->stroke();

    //draw_bb(context,pos,ds,get_bounding_box());

    context->restore();
}
bool CirclePrimitive::under(Coordinate pos, float tol)
{
    // Check if the line is under pos
    // TODO Adapt for ellipses
    if(_vradius - _hradius < tol) // Nearly circular
    {
        float dist = Geometry::distance_from_circle(pos,_center,_vradius);
        return dist < tol;
    }
    else
        return false;
}
void CirclePrimitive::rotate90()
{
    // Rotate about origin
    Coordinate tcenter = _center;
    _center.x(-_center.y());
    _center.y(tcenter.x());
    
}
void CirclePrimitive::hflip()
{
    // Mirror horizontally across origin
    _center.x(-_center.x());
}
void CirclePrimitive::vflip()
{
    // Mirror vertically across origin
    _center.y(-_center.y());
}

void TextPrimitive::draw(Cairo::RefPtr<Cairo::Context> context, 
    Coordinate pos, const DrawSettings& ds)
{
    context->save();
    context->set_source_rgb(ds.red,ds.green,ds.blue);
    context->set_antialias(ds.antialias);
    context->select_font_face(ds.font_family,ds.font_slant,ds.font_weight);
    context->set_font_size (ds.font_size);

    context->move_to(pos.x(),pos.y());
    context->rel_move_to(_anchor.x(),_anchor.y());
    context->show_text(_text);
    
    //draw_bb(context,pos,ds,get_bounding_box(ds.font_size));

    context->restore();
}
void TextPrimitive::rotate90()
{
    // Rotate about origin
    Coordinate tanchor = _anchor;
    _anchor.x(-_anchor.y());
    _anchor.y(tanchor.x());
}
void TextPrimitive::hflip()
{
    // Mirror horizontally across origin
    _anchor.x(-_anchor.x());
}
void TextPrimitive::vflip()
{
    // Mirror vertically across origin
    _anchor.y(-_anchor.y());
}

BoundingBox LinePrimitive::get_bounding_box()
{
    BoundingBox bb;
    bb.width = std::abs(_start.x() - _end.x());
    bb.height = std::abs(_start.y() - _end.y());

    Coordinate anch;
    anch.x(std::min(_start.x(),_end.x()));
    anch.y(std::min(_start.y(),_end.y()));
    bb.anchor = anch;

	// Avoid issues with ortho lines
	if(bb.width < 1)
    {
		bb.width = 2;
        bb.anchor.x(bb.anchor.x()-1);
    }
	if(bb.height < 1)
    {
		bb.height = 2;
        bb.anchor.y(bb.anchor.y()-1);
    }

    return bb;
}
BoundingBox RectPrimitive::get_bounding_box()
{
    BoundingBox bb;
    bb.anchor = _anchor;
    bb.width = _width;
    bb.height = _height;
    return bb;
}
BoundingBox ArcPrimitive::get_bounding_box()
{
    BoundingBox bb;
    /* Method:
     *  We have theta1 and theta2, and an arc from theta1 to theta2. 
     *  The bounding box is either at a radius (when 0, 90, 180, 270 are
     *  contained in the arc) or is set by one of the end points. This
     *  excludes the circle center.
     */
    double x1,x2,y1,y2;
    x1 = _center.x() + _hradius*std::cos(_start_angle_deg*M_PI/180);
    x2 = _center.x() + _hradius*std::cos(_end_angle_deg*M_PI/180);
    y1 = _center.y() + _vradius*std::sin(_start_angle_deg*M_PI/180);
    y2 = _center.y() + _vradius*std::sin(_end_angle_deg*M_PI/180);

    bool contains0 = false,contains90 = false,contains180 = false,contains270 = false;
    if(_start_angle_deg < 0 && _end_angle_deg > 0) contains0 = true;
    if(_start_angle_deg < 90 && _end_angle_deg > 90) contains90 = true;
    if(_start_angle_deg < 180 && _end_angle_deg > 180) contains180 = true;
    if(_start_angle_deg < 270 && _end_angle_deg > 270) contains270 = true;

    double r,l,t,b;
    if(contains0) r = _center.x() + _hradius;
    else r = std::max(x1,x2);
    
    if(contains180) l = _center.x() - _hradius;
    else l = std::min(x1,x2);

    if(contains90) t = _center.y() + _vradius;
    else t = std::max(y1,y2);

    if(contains270) b = _center.y() - _vradius;
    else b = std::min(y1,y2);

    Coordinate anch(l,b);
    bb.anchor = anch;
    bb.width = std::abs(r-l);
    bb.height = std::abs(t-b);

    return bb;
}
BoundingBox CirclePrimitive::get_bounding_box()
{
    BoundingBox bb;
    Coordinate anch(_center.x() - _hradius, _center.y() - _vradius);
    bb.anchor = anch;
    bb.width = 2*_hradius;
    bb.height = 2*_vradius;
    return bb;
}
BoundingBox TextPrimitive::get_bounding_box(double fontsize,double fontwidth)
{
    BoundingBox bb;
    /* NOTE This isn't so simple, even with a monospace font. We can do it
     * through the Cairo context with get_text_extents. Do this with
     * a single character, pass the width to this method, or try
     * a fixed ratio.
     */

    bb.anchor = _anchor;
    bb.height = -fontsize;
    bb.width = _text.size()*fontwidth;

    return bb;
}
BoundingBox TextPrimitive::get_bounding_box(double fontsize)
{
    BoundingBox bb;
    /* NOTE This overload assumes a 3/5 ratio.
     * A ratio of 3/5 is somewhat common in monospace fonts.
     */

    bb.anchor = _anchor;
    bb.height = -fontsize;
    bb.width = _text.size()*fontsize*(3.0/5.0);

    return bb;
}

SymbolPin::SymbolPin()
{
    if(_attrs.empty())
        init_attributes();
}
void SymbolPin::init_attributes()
{
    SymbolPinAttribute spiceorder_attr;
    spiceorder_attr.name = "SPICE_ORDER";
    spiceorder_attr.value = "";
    spiceorder_attr.removable = false;
    spiceorder_attr.required = true;
    spiceorder_attr.show_on_schematic = false;
    spiceorder_attr.editable = true;

    SymbolPinAttribute name_attr;
    name_attr.name = "NAME";
    name_attr.value = "";
    name_attr.removable = false;
    name_attr.required = true;
    name_attr.show_on_schematic = true;
    name_attr.editable = true;

    _attrs.clear();
	_attrs.insert(std::pair<Glib::ustring,SymbolPinAttribute>(spiceorder_attr.name,spiceorder_attr));
	_attrs.insert(std::pair<Glib::ustring,SymbolPinAttribute>(name_attr.name,name_attr));
}

void SymbolPin::draw(Cairo::RefPtr<Cairo::Context> context, 
	const Coordinate& pos, const DrawSettings& ds,
	bool highlight)
{
	if(highlight)
	{
		context->save();
		context->set_source_rgb(0.1,0.1,0.9); // PIN HIGHLIGHT COLOR
        double lw = ds.line_width;
        context->device_to_user_distance(lw,lw);
		context->set_line_width(lw);
		context->set_line_cap(ds.line_cap);
		context->set_line_join(ds.line_join);
		context->set_antialias(ds.antialias);

        context->move_to(pos.x(),pos.y());
        
        context->rel_move_to(_pinloc.x()-_hbox_size/2,_pinloc.y()-_hbox_size/2);
        context->rel_line_to(0,_hbox_size); // Left wall
        context->rel_line_to(_hbox_size,0); // Top wall
        context->rel_line_to(0,-_hbox_size); // Right wall
        context->close_path(); // Bottom wall

		context->stroke();
		context->restore();
	}
}

std::shared_ptr<SymbolPinAttribute> SymbolPin::get_attribute(Glib::ustring attr_name)
{
    if(has_attribute(attr_name))
    {
        return std::make_shared<SymbolPinAttribute>(_attrs.find(attr_name)->second);
    }
    return nullptr;
}
bool SymbolPin::has_attribute(Glib::ustring attr_name)
{
    if(_attrs.find(attr_name) != _attrs.end())
        return true;
    return false;
}
BoundingBox SymbolPin::get_bounding_box()
{
    BoundingBox bb;
    bb.width = _hbox_size;
    bb.height = _hbox_size;

    Coordinate anch;
    anch.x(_pinloc.x());
    anch.y(_pinloc.y());
    bb.anchor = anch;

    return bb;
}
void SymbolPin::rotate90()
{
    // Rotate about origin
    Coordinate tpinloc = _pinloc;
    _pinloc.x(-_pinloc.y());
    _pinloc.y(tpinloc.x());
}
void SymbolPin::hflip()
{
    // Mirror horizontally across origin
    _pinloc.x(-_pinloc.x());
}
void SymbolPin::vflip()
{
    // Mirror vertically across origin
    _pinloc.y(-_pinloc.y());
}
