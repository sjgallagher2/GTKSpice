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

#include <app/object_symbol.h>

ObjectSymbol::ObjectSymbol(ObjectGeometry geom, ObjectPins pins, Coordinate pos, bool visible) :
    _geometry(geom),
    _pins(pins),
    _position(pos),
    _visible(visible)
{
}

void ObjectSymbol::draw( Cairo::RefPtr<Cairo::Context> context )
{
    // Draw the symbol primitives and pin
    if(_visible)
    {
        for(auto itr = _geometry.begin(); itr != _geometry.end(); ++itr)
            (*itr)->draw(context, _position, _drawsettings);
        for(auto itr = _pins.begin(); itr != _pins.end(); ++itr)
            (*itr)->draw(context, _position, _drawsettings,true);
    }
}

void ObjectSymbol::init_attributes()
{
    SymbolAttribute symfile_attr;
    symfile_attr.name = "FILE";
    symfile_attr.value = "";
    symfile_attr.removable = false;
    symfile_attr.required = true;
    symfile_attr.show_on_schematic = false;
    symfile_attr.editable = false;

    SymbolAttribute name_attr;
    name_attr.name = "NAME";
    name_attr.value = "";
    name_attr.removable = false;
    name_attr.required = true;
    name_attr.show_on_schematic = true;
    name_attr.editable = true;

    SymbolAttribute value_attr;
    value_attr.name = "VALUE";
    value_attr.value = "";
    value_attr.removable = false;
    value_attr.required = false;
    value_attr.show_on_schematic = false;
    value_attr.editable = true;

    SymbolAttribute desc_attr;
    desc_attr.name = "DESCRIPTION";
    desc_attr.value = "";
    desc_attr.removable = false;
    desc_attr.required = false;
    desc_attr.show_on_schematic = false;
    desc_attr.editable = true;

    _attrs.clear();
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("FILE",symfile_attr));
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("NAME",name_attr));
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("VALUE",value_attr));
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("DESCRIPTION",desc_attr));

}

std::shared_ptr<SymbolAttribute> ObjectSymbol::get_attribute(Glib::ustring attr_name)
{
    if(has_attribute(attr_name))
    {
        return std::make_shared<SymbolAttribute>(_attrs.find(attr_name)->second);
    }
    else
        return nullptr;
}

bool ObjectSymbol::has_attribute(Glib::ustring attr_name)
{
    return (_attrs.find(attr_name) != _attrs.end());
}

std::shared_ptr<SymbolPin> ObjectSymbol::get_pin(Glib::ustring pin_name)
{}
void ObjectSymbol::set_pin(Glib::ustring pin_name, SymbolPin new_pin)
{
    if(has_pin(pin_name))
    {
        for(auto itr = _pins.begin(); itr != _pins.end(); ++itr)
            if((*itr)->get_attribute("NAME")->name.compare(pin_name) == 0)
                *itr = std::make_shared<SymbolPin>(new_pin);
    }
}
bool ObjectSymbol::has_pin(Glib::ustring pin_name)
{
    for(auto itr = _pins.begin(); itr != _pins.end(); ++itr)
    {
        if((*itr)->has_attribute("NAME"))
            if((*itr)->get_attribute("NAME")->name.compare(pin_name) == 0)
                return true;
    }
    return false;
}


