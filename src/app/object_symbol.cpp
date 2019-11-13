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
    _attrs.push_back(symfile_attr);
    _attrs.push_back(name_attr);
    _attrs.push_back(value_attr);
    _attrs.push_back(desc_attr);
}
