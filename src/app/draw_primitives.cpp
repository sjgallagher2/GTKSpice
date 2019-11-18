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
#include <algorithm>
#include <cmath>

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

    context->restore();
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

    context->restore();
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

    context->arc(pos.x()+_center.x(),pos.y()+_center.y(),_radius,
        _start_angle_deg*M_PI/180,_end_angle_deg*M_PI/180);
    
    context->stroke();

    context->restore();
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

    context->arc(pos.x()+_center.x(),pos.y()+_center.y(),_radius,0,2*M_PI);
    
    context->stroke();

    context->restore();
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

    context->show_text(_text);

    context->restore();
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
		bb.width = 1;
	if(bb.height < 1)
		bb.height = 1;

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
    // TODO Haven't tested this for sign errors etc
    /* Method:
     *  We have theta1 and theta2, and an arc from theta1 to theta2. 
     *  The bounding box is either at a radius (when 0, 90, 180, 270 are
     *  contained in the arc) or is set by one of the end points. This
     *  excludes the circle center.
     */
    double x1,x2,y1,y2;
    x1 = _center.x() + _radius*std::cos(_start_angle_deg*M_PI/180);
    x2 = _center.x() + _radius*std::cos(_end_angle_deg*M_PI/180);
    y1 = _center.y() + _radius*std::sin(_start_angle_deg*M_PI/180);
    y2 = _center.y() + _radius*std::sin(_end_angle_deg*M_PI/180);

    bool contains0 = false,contains90 = false,contains180 = false,contains270 = false;
    if(_start_angle_deg < 0 && _end_angle_deg > 0) contains0 = true;
    if(_start_angle_deg < 90 && _end_angle_deg > 90) contains90 = true;
    if(_start_angle_deg < 180 && _end_angle_deg > 180) contains180 = true;
    if(_start_angle_deg < 270 && _end_angle_deg > 270) contains270 = true;

    double r,l,t,b;
    if(contains0) r = _center.x() + _radius;
    else r = std::max(x1,x2);
    
    if(contains180) l = _center.x() - _radius;
    else l = std::min(x1,x2);

    if(contains90) t = _center.y() + _radius;
    else t = std::max(y1,y2);

    if(contains270) b = _center.y() - _radius;
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
    Coordinate anch(_center.x() - _radius, _center.y() - _radius);
    bb.anchor = anch;
    bb.width = 2*_radius;
    bb.height = 2*_radius;
    return bb;
}
BoundingBox TextPrimitive::get_bounding_box(double fontsize,double fontwidth)
{
    BoundingBox bb;
    /* TODO This isn't so simple, even with a monospace font. We can do it
     * through the Cairo context with get_text_extents. Do this with
     * a single character, pass the width to this method, then we can talk.
     */

    bb.anchor = _anchor;
    bb.height = fontsize;
    bb.width = _text.size()*fontwidth;

    return bb;
}

SymbolPin::SymbolPin()
{
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

    SymbolPinAttribute dir_attr;
    dir_attr.name = "DIRECTION";
    dir_attr.value = "LEFT"; // LEFT, RIGHT, UP, or DOWN
    dir_attr.removable = false;
    dir_attr.required = false;
    dir_attr.show_on_schematic = false;
    dir_attr.editable = true;

    _attrs.clear();
	_attrs.insert(std::pair<Glib::ustring,SymbolPinAttribute>(spiceorder_attr.name,spiceorder_attr));
	_attrs.insert(std::pair<Glib::ustring,SymbolPinAttribute>(name_attr.name,name_attr));
	_attrs.insert(std::pair<Glib::ustring,SymbolPinAttribute>(dir_attr.name,dir_attr));
}

void SymbolPin::set_direction(Glib::ustring dir)
{
    /*
    if(_attrs.find("DIRECTION") != _attrs.end())
    {
        if(_attrs.find("DIRECTION")->second.value.compare("LEFT") == 0)
            _direction = "LEFT";
        else if(_attrs.find("DIRECTION")->second.value.compare("RIGHT") == 0)
            _direction = "RIGHT";
        else if(_attrs.find("DIRECTION")->second.value.compare("UP") == 0)
            _direction = "UP";
        else if(_attrs.find("DIRECTION")->second.value.compare("DOWN") == 0)
            _direction = "DOWN";
    }
    */
}

Glib::ustring SymbolPin::get_direction()
{
    return _direction;
}

void SymbolPin::draw(Cairo::RefPtr<Cairo::Context> context, 
	const Coordinate& pos, const DrawSettings& ds,
	bool highlight)
{
    context->save();
    context->set_source_rgb(ds.red,ds.green,ds.blue);
    context->set_line_width(ds.line_width);
    context->set_line_cap(ds.line_cap);
    context->set_line_join(ds.line_join);
    context->set_antialias(ds.antialias);

    context->move_to(pos.x(),pos.y());

    context->rel_move_to(_start.x(),_start.y());

	double pin_len = 5;

	Coordinate hbox_pos; // Bottom left corner of highlight box
	double hbox_size = 1;
    if(_direction == "LEFT")
    {
        context->rel_line_to(_start.x() - pin_len,_start.y());// Draw to the left
        hbox_pos = Coordinate(_start.x()-pin_len-hbox_size/2, _start.y()+hbox_size/2);
    }
    else if(_direction == "RIGHT")
    {
        context->rel_line_to(_start.x() + pin_len,_start.y());// Draw to the right
        hbox_pos = Coordinate(_start.x()+pin_len-hbox_size/2, _start.y()+hbox_size/2);
    }
    else if(_direction == "DOWN")
    {
        context->rel_line_to(_start.x(),_start.y()-pin_len);// Draw down
        hbox_pos = Coordinate(_start.x()-hbox_size/2, _start.y()-pin_len+hbox_size/2);
    }
    else if(_direction == "UP")
    {
        context->rel_line_to(_start.x(),_start.y()+pin_len);// Draw up
        hbox_pos = Coordinate(_start.x()-hbox_size/2, _start.y()+pin_len+hbox_size/2);
    }
    context->stroke();
    context->restore();

	if(highlight)
	{
		context->save();
		context->set_source_rgb(0.9,0.1,0.1);
		context->set_line_width(ds.line_width);
		context->set_line_cap(ds.line_cap);
		context->set_line_join(ds.line_join);
		context->set_antialias(ds.antialias);

		context->rectangle(hbox_pos.x(),hbox_pos.y(),hbox_size,hbox_size);
		
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
bool SymbolPin::under(const Coordinate& pos)
{
	BoundingBox hbox; // Highlight box
	double hbox_size = 1;
    double pin_len = 5;
    if(_direction == "LEFT")
        hbox.anchor = Coordinate(_start.x()-pin_len-hbox_size/2, _start.y()+hbox_size/2);
    if(_direction == "RIGHT")
        hbox.anchor = Coordinate(_start.x()+pin_len-hbox_size/2, _start.y()+hbox_size/2);
    if(_direction == "DOWN")
        hbox.anchor = Coordinate(_start.x()-hbox_size/2, _start.y()-pin_len+hbox_size/2);
    if(_direction == "UP")
        hbox.anchor = Coordinate(_start.x()-hbox_size/2, _start.y()+pin_len+hbox_size/2);
    hbox.width = hbox_size;
    hbox.height = hbox_size;

    return hbox.contains(pos);
}
BoundingBox SymbolPin::get_bounding_box()
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
		bb.width = 1;
	if(bb.height < 1)
		bb.height = 1;

    return bb;
}
