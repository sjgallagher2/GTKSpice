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

#ifndef DRAW_PRIMITIVES_H
#define DRAW_PRIMITIVES_H

#include <memory>
#include <vector>
#include <map>
#include <gtkmm.h>
#include <app/coordinate.h>

struct DrawSettings
{
    double red=0,green=0,blue=0; // Stroke color
    double line_width=2;
    Cairo::LineCap line_cap = Cairo::LINE_CAP_BUTT;
    Cairo::LineJoin line_join = Cairo::LINE_JOIN_BEVEL;
    Cairo::Antialias antialias = Cairo::ANTIALIAS_NONE;
    double font_size = 10;
    Glib::ustring font_family = "monospace";
    Cairo::FontSlant font_slant = Cairo::FONT_SLANT_NORMAL;
    Cairo::FontWeight font_weight = Cairo::FONT_WEIGHT_NORMAL;
};


/* Primitives are used to describe a component's Symbol */
enum PrimitiveType {
    LINE_PRIMITIVE,
    RECT_PRIMITIVE,
    ARC_PRIMITIVE,
    CIRCLE_PRIMITIVE,
    TEXT_PRIMITIVE
};

class Primitive
{
public:
    PrimitiveType type() const {return _type;}

    /* @brief Draw primitive to context, at position pos
     * 
     */
    virtual void draw(Cairo::RefPtr<Cairo::Context> context, 
        Coordinate pos, const DrawSettings& drawsettings) = 0;

    bool selectable() const {return _selectable;}
    void selectable(bool s) {_selectable = s;}

    virtual BoundingBox get_bounding_box() = 0;
	virtual bool under(Coordinate pos, float tol = 1)
    {
        return false;
    }

protected:
    PrimitiveType _type;
    bool _selectable;
};

class LinePrimitive : public Primitive 
{
public:
    LinePrimitive() {_type=LINE_PRIMITIVE;}

    /* @brief Draw primitive to context, at position pos
     * 
     */
    virtual void draw(Cairo::RefPtr<Cairo::Context> context, 
        Coordinate pos, const DrawSettings& drawsettings);

    virtual BoundingBox get_bounding_box();

    virtual void start(Coordinate s) {_start = s;}
    virtual void end(Coordinate e) {_end = e;}
    virtual Coordinate start() const {return _start;}
    virtual Coordinate end() const {return _end;}

    virtual bool under(Coordinate pos, float tol = 1);

protected:
    Coordinate _start,_end;

};

class RectPrimitive : public Primitive 
{
public:
    RectPrimitive() {_type=RECT_PRIMITIVE;}

    /* @brief Draw primitive to context, at position pos
     * 
     */
    virtual void draw(Cairo::RefPtr<Cairo::Context> context, 
        Coordinate pos, const DrawSettings& drawsettings);

    virtual BoundingBox get_bounding_box();

    virtual void anchor(Coordinate a) {_anchor = a;}
    virtual void width(double w) {_width = w;}
    virtual void height(double h) {_height = h;}

    virtual Coordinate anchor() const {return _anchor;}
    virtual double width() const {return _width;}
    virtual double height() const {return _height;}
    
    virtual bool under(Coordinate pos, float tol = 1);

    virtual void set_rect(Coordinate anchor, double width, double height)
    {
        _anchor = anchor;
        _width = width;
        _height = height;
    }

protected:
    Coordinate _anchor; // Bottom left
    double _width,_height;
};

class ArcPrimitive : public Primitive 
{
public:
    ArcPrimitive() {_type=ARC_PRIMITIVE;}

    /* @brief Draw primitive to context, at position pos
     * 
     */
    virtual void draw(Cairo::RefPtr<Cairo::Context> context, 
        Coordinate pos, const DrawSettings& drawsettings);

    virtual BoundingBox get_bounding_box();
    
    virtual void center(Coordinate c) {_center = c;}
    virtual void hradius(double r) {_hradius = r;}
    virtual void vradius(double r) {_vradius = r;}
    virtual void set_start_angle_degrees(double theta_deg) {_start_angle_deg = theta_deg;}
    virtual void set_end_angle_degrees(double theta_deg) {_end_angle_deg = theta_deg;}
    virtual Coordinate center() const {return _center;}
    virtual double hradius() const {return _hradius;}
    virtual double vradius() const {return _vradius;}
    virtual double get_start_angle_degrees() const {return _start_angle_deg;}
    virtual double get_end_angle_degrees() const {return _end_angle_deg;}

    virtual bool under(Coordinate pos, float tol = 1);

protected:
    Coordinate _center;
    double _hradius;
    double _vradius;
    double _start_angle_deg; // Angle in degrees
    double _end_angle_deg; // Angle in degrees
};

class CirclePrimitive : public Primitive 
{
public:
    CirclePrimitive() {_type=CIRCLE_PRIMITIVE;}

    /* @brief Draw primitive to context, at position pos
     * 
     */
    virtual void draw(Cairo::RefPtr<Cairo::Context> context, 
        Coordinate pos, const DrawSettings& drawsettings);

    virtual BoundingBox get_bounding_box();
    
    virtual void center(Coordinate c) {_center = c;}
    virtual void hradius(double r) {_hradius = r;}
    virtual void vradius(double r) {_vradius = r;}
    virtual Coordinate center() const {return _center;}
    virtual double hradius() const {return _hradius;}
    virtual double vradius() const {return _vradius;}

    virtual bool under(Coordinate pos, float tol = 1);

protected:
    Coordinate _center;
    double _hradius;
    double _vradius;
};

class TextPrimitive : public Primitive 
{
public:
    TextPrimitive() {_type=TEXT_PRIMITIVE;}

    /* @brief Draw primitive to context, at position pos
     * 
     */
    virtual void draw(Cairo::RefPtr<Cairo::Context> context, 
        Coordinate pos, const DrawSettings& drawsettings);

    virtual BoundingBox get_bounding_box() 
        {BoundingBox empty;empty.anchor = Coordinate(0,0);empty.width=0;empty.height=0;return empty;}
    virtual BoundingBox get_bounding_box(double fontsize,double fontwidth);
    virtual BoundingBox get_bounding_box(double fontsize); // Assume W:H ratio of 3:5

    virtual void anchor(Coordinate a) {_anchor = a;}
    virtual void text(Glib::ustring s) {_text = s;}
    virtual Coordinate anchor() const {return _anchor;}
    virtual Glib::ustring text() const {return _text;}
    
	virtual bool under(const Coordinate& pos, float tol = 1)
    {
        return get_bounding_box().contains(pos);
    }

protected:
    Glib::ustring _text;
    Coordinate _anchor;
};


struct SymbolPinAttribute 
{
    Glib::ustring name;
    Glib::ustring value;
    bool required; // @about Can this attribute have an empty value?
    bool show_on_schematic; // @about Should this attribute be shown on the schematic?
    bool editable; // @about Can the user change this attribute?
    bool removable; // @about Can the user remove this attribute from the symbol?
};

class SymbolPin {
public:
    SymbolPin();

    /* @brief Draw pin to context, at position pos
     * 
     */
    virtual void draw(Cairo::RefPtr<Cairo::Context> context, 
        const Coordinate& pos, const DrawSettings& ds,
        bool highlight = false);
    
    virtual void pin_location(Coordinate s) {_pinloc = s;}
    virtual Coordinate pin_location() const {return _pinloc;}

    std::shared_ptr<SymbolPinAttribute> get_attribute(Glib::ustring attr_name);
	void add_attribute(SymbolPinAttribute attr) 
        {_attrs.insert(std::pair<Glib::ustring,SymbolPinAttribute>(attr.name,attr));}
	bool has_attribute(Glib::ustring attr_name);

    Glib::ustring get_attribute_value(Glib::ustring attr_name)
    {
        if(has_attribute(attr_name))
            return _attrs.find(attr_name)->second.value;
        return "";
    }
    bool get_attribute_removable(Glib::ustring attr_name)
    {
        if(has_attribute(attr_name))
            return _attrs.find(attr_name)->second.removable;
        return false;
    }
    bool get_attribute_required(Glib::ustring attr_name)
    {
        if(has_attribute(attr_name))
            return _attrs.find(attr_name)->second.required;
        return false;
    }
    bool get_attribute_show_on_schematic(Glib::ustring attr_name)
    {
        if(has_attribute(attr_name))
            return _attrs.find(attr_name)->second.show_on_schematic;
        return false;
    }
    bool get_attribute_editable(Glib::ustring attr_name)
    {
        if(has_attribute(attr_name))
            return _attrs.find(attr_name)->second.editable;
        return false;
    }

    void set_attribute_value(Glib::ustring attr_name, Glib::ustring value)
    {
        if(has_attribute(attr_name))
            _attrs.find(attr_name)->second.value = value;
    }
    void set_attribute_removable(Glib::ustring attr_name, bool removable)
    {
        if(has_attribute(attr_name))
            _attrs.find(attr_name)->second.removable = removable;
    }
    void set_attribute_required(Glib::ustring attr_name, bool required)
    {
        if(has_attribute(attr_name))
            _attrs.find(attr_name)->second.required = required;
    }
    void set_attribute_show_on_schematic(Glib::ustring attr_name, bool show)
    {
        if(has_attribute(attr_name))
            _attrs.find(attr_name)->second.show_on_schematic = show;
    }
    void set_attribute_editable(Glib::ustring attr_name, bool editable)
    {
        if(has_attribute(attr_name))
            _attrs.find(attr_name)->second.editable = editable;
    }

    virtual BoundingBox get_bounding_box();
	virtual bool under(Coordinate pos,float tol = 1)
    {
        return get_bounding_box().contains(pos);
    }
protected:
    virtual void init_attributes();
    std::map<Glib::ustring,SymbolPinAttribute> _attrs;
    Coordinate _pinloc;
    double _hbox_size = 1.5; // Highlight box size
};


#endif /* DRAW_PRIMITIVES_H */
