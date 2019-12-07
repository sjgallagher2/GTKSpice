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
#include <iostream>
#include <algorithm>
#include <string>

ObjectSymbol::ObjectSymbol(ObjectGeometry geom, ObjectPins pins, Coordinate pos, bool visible) :
    _geometry(geom),
    _pins(pins),
    _position(pos),
    _visible(visible)
{
    if(_attrs.empty())
        init_attributes();
    _calculate_bounding_box();
}

void ObjectSymbol::draw( Cairo::RefPtr<Cairo::Context> context, std::vector<int> pin_highlights)
{
    // Draw the symbol primitives and pin
    if(_visible)
    {
        for(auto itr = _geometry.begin(); itr != _geometry.end(); ++itr)
            (*itr)->draw(context, _position, _drawsettings);
        for(auto itr = _pins.begin(); itr != _pins.end(); ++itr)
        {
            bool highlight = false;
            int spice_order = std::stoi((*itr)->get_attribute_value("SPICE_ORDER"));
            if(std::binary_search(pin_highlights.begin(),pin_highlights.end(),spice_order))
                highlight = true;
            (*itr)->draw(context, _position, _drawsettings,highlight);
        }
        for(auto itr = _attrs.begin(); itr != _attrs.end(); ++itr)
        {
            if(itr->second.show_on_schematic)
            {
                if(_attr_texts.find(itr->first) != _attr_texts.end())
                {
                    _attr_texts.find(itr->first)->second->text(itr->second.value);
                    _attr_texts.find(itr->first)->second->draw(context,_position,_drawsettings);
                }
            }
        }
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

    SymbolAttribute prefix_attr;
    prefix_attr.name = "PREFIX";
    prefix_attr.value = "";
    prefix_attr.removable = false;
    prefix_attr.required = true;
    prefix_attr.show_on_schematic = false;
    prefix_attr.editable = true;

    SymbolAttribute instname_attr;
    instname_attr.name = "INSTNAME";
    instname_attr.value = "";
    instname_attr.removable = false;
    instname_attr.required = true;
    instname_attr.show_on_schematic = false;
    instname_attr.editable = true;

    SymbolAttribute value_attr;
    value_attr.name = "VALUE";
    value_attr.value = "";
    value_attr.removable = false;
    value_attr.required = false;
    value_attr.show_on_schematic = false;
    value_attr.editable = true;

    SymbolAttribute model_attr;
    model_attr.name = "SPICEMODEL";
    model_attr.value = "";
    model_attr.removable = false;
    model_attr.required = false;
    model_attr.show_on_schematic = false;
    model_attr.editable = true;

    SymbolAttribute modelfile_attr;
    modelfile_attr.name = "MODELFILE";
    modelfile_attr.value = "";
    modelfile_attr.removable = false;
    modelfile_attr.required = false;
    modelfile_attr.show_on_schematic = false;
    modelfile_attr.editable = true;

    SymbolAttribute spiceline_attr;
    spiceline_attr.name = "SPICELINE";
    spiceline_attr.value = "";
    spiceline_attr.removable = false;
    spiceline_attr.required = false;
    spiceline_attr.show_on_schematic = false;
    spiceline_attr.editable = true;

    SymbolAttribute desc_attr;
    desc_attr.name = "DESCRIPTION";
    desc_attr.value = "";
    desc_attr.removable = false;
    desc_attr.required = false;
    desc_attr.show_on_schematic = false;
    desc_attr.editable = true;

    _attrs.clear();
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("FILE",symfile_attr));
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("PREFIX",prefix_attr));
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("VALUE",value_attr));
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("SPICEMODEL",model_attr));
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("MODELFILE",modelfile_attr));
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("SPICELINE",spiceline_attr));
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("DESCRIPTION",desc_attr));
    _attrs.insert(std::pair<Glib::ustring, SymbolAttribute>("INSTNAME",instname_attr));

    _attr_texts.insert(std::pair<Glib::ustring,std::shared_ptr<TextPrimitive>>(
        "FILE",std::make_shared<TextPrimitive>()));
    _attr_texts.insert(std::pair<Glib::ustring,std::shared_ptr<TextPrimitive>>(
        "VALUE",std::make_shared<TextPrimitive>()));
    _attr_texts.insert(std::pair<Glib::ustring,std::shared_ptr<TextPrimitive>>(
        "SPICEMODEL",std::make_shared<TextPrimitive>()));
    _attr_texts.insert(std::pair<Glib::ustring,std::shared_ptr<TextPrimitive>>(
        "SPICELINE",std::make_shared<TextPrimitive>()));
    _attr_texts.insert(std::pair<Glib::ustring,std::shared_ptr<TextPrimitive>>(
        "DESCRIPTION",std::make_shared<TextPrimitive>()));
    _attr_texts.insert(std::pair<Glib::ustring,std::shared_ptr<TextPrimitive>>(
        "INSTNAME",std::make_shared<TextPrimitive>()));

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
void ObjectSymbol::_calculate_bounding_box()
{
    BoundingBox geom_bb = _geometry.at(0)->get_bounding_box();
    double left,right,bottom,top;
    left = geom_bb.anchor.x();
    right = geom_bb.anchor.x() + geom_bb.width;
    bottom = geom_bb.anchor.y();
    top = geom_bb.anchor.y() + geom_bb.height;

    for(auto itr = _geometry.begin(); itr != _geometry.end(); ++itr)
    {
        // Adjust boundaries
        BoundingBox geom_bb = (*itr)->get_bounding_box();
        double tleft,tright,tbottom,ttop;
        tleft = geom_bb.anchor.x();
        tright = geom_bb.anchor.x() + geom_bb.width;
        tbottom = geom_bb.anchor.y();
        ttop = geom_bb.anchor.y() + geom_bb.height;


        left = std::min(left,tleft);
        right = std::min(right,tright);
        bottom = std::min(bottom,tbottom);
        top = std::min(top,ttop);
    }
    _boundingbox.anchor.x(_position.x() + left);
    _boundingbox.anchor.y(_position.y() + bottom);
    _boundingbox.width = right - left;
    _boundingbox.height = top-bottom;
}

bool ObjectSymbol::under(const Coordinate& pos)
{
    // Check if any geometry is under pos
    bool is_under = false;
    Coordinate offset_pos(pos.x() - _position.x(), pos.y() - _position.y());

    for(auto itr = _geometry.begin(); itr != _geometry.end(); ++itr)
    {
        is_under = is_under | (*itr)->under(offset_pos);
    }
    return is_under;
}
bool ObjectSymbol::near(const Coordinate& pos)
{
    // Check if pos is in bounding box
    return _boundingbox.contains(pos);
}
bool ObjectSymbol::within(const Coordinate& start, const Coordinate& end)
{
    bool within = false;
    BoundingBox bb;
    bb.anchor = start;
    bb.width = end.x()-start.x();
    bb.height = end.y()-start.y();
    within = bb.contains(Coordinate(_boundingbox.anchor.x(),
        _boundingbox.anchor.y()));
    within = within & bb.contains(Coordinate(_boundingbox.anchor.x() + _boundingbox.width,
        _boundingbox.anchor.y()+_boundingbox.height));
    
    return within;
}

void ObjectSymbol::rotate90()
{
    for(auto& itr : _geometry)
        itr->rotate90();
    for(auto& itr : _pins)
        itr->rotate90();
    for(auto& itr : _attr_texts)
        itr.second->rotate90();
}
void ObjectSymbol::hflip()
{
    for(auto& itr : _geometry)
        itr->hflip();
    for(auto& itr : _pins)
        itr->hflip();
    for(auto& itr : _attr_texts)
        itr.second->hflip();

}
void ObjectSymbol::vflip()
{
    for(auto& itr : _geometry)
        itr->vflip();
    for(auto& itr : _pins)
        itr->vflip();
    for(auto& itr : _attr_texts)
        itr.second->vflip();

}