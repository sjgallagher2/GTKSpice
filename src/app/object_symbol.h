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

#ifndef OBJECT_SYMBOL_H
#define OBJECT_SYMBOL_H

#include <memory>
#include <vector>
#include <map>
#include <app/coordinate.h>
#include <app/draw_primitives.h>

struct SymbolAttribute 
{
    Glib::ustring name;
    Glib::ustring value;
    bool required; // Can this attribute have an empty value?
    bool show_on_schematic; // Should this attribute be shown on the schematic?
    bool editable; // Can the user change this attribute?
    bool removable; // Can the user remove this attribute from the symbol?
};

typedef std::vector<std::shared_ptr<Primitive>> ObjectGeometry;
typedef std::vector<std::shared_ptr<SymbolPin>> ObjectPins;

class ObjectSymbol 
{
public:
    ObjectSymbol() {init_attributes();} // TODO Overload constructor for input _geometry and pins
	ObjectSymbol(ObjectGeometry geom, ObjectPins pins, Coordinate pos, bool visible = true);
    
    void position(Coordinate pos) {_position = pos;}
	Coordinate position() const {return _position;}
	void visible(bool vis) {_visible = vis;}
	bool visible() const {return _visible;}

    std::shared_ptr<DrawSettings> get_draw_settings() const {return std::make_shared<DrawSettings>(_drawsettings);}
    void set_draw_settings(DrawSettings ds) {_drawsettings = ds;}

    std::shared_ptr<SymbolAttribute> get_attribute(Glib::ustring attr_name);
	void add_attribute(SymbolAttribute attr) 
        {_attrs.insert(std::pair<Glib::ustring,SymbolAttribute>(attr.name,attr));}
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

    std::shared_ptr<ObjectGeometry> get_geometry() const {return std::make_shared<ObjectGeometry>(_geometry);}

	std::shared_ptr<ObjectPins> get_pins() const {return std::make_shared<ObjectPins>(_pins);}
    std::shared_ptr<SymbolPin> get_pin(Glib::ustring pin_name);
	void add_pin(SymbolPin new_pin) {_pins.push_back(std::make_shared<SymbolPin>(new_pin));} // Add a new pin
	void set_pin(Glib::ustring pin_name, SymbolPin new_pin); // Set the pin with pin_name to new_pin
    void set_pins(ObjectPins pins) {_pins = pins;}
    void set_geometry(ObjectGeometry geom) {_geometry = geom;}

    void draw(Cairo::RefPtr<Cairo::Context> context);
    bool under(Coordinate pos); // Return true if visible and selectable parts of Symbol are (roughly) under pos

protected:
    virtual void init_attributes(); // Create default attributes: FILE, NAME, VALUE, and DESCRIPTION
    virtual bool has_pin(Glib::ustring pin_name);
    DrawSettings _drawsettings;
    Coordinate _position;
    bool _visible;
    ObjectGeometry _geometry;
    BoundingBox _boundingbox;
    ObjectPins _pins;
    std::map<Glib::ustring, SymbolAttribute> _attrs;

};

#endif /* OBJECT_SYMBOL_H */
