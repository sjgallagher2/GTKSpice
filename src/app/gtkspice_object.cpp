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

#include <fstream>
#include <app/gtkspice_object.h>
#include <app/ltspice_symbol_parser.h>

GtkSpiceElement::GtkSpiceElement(const Glib::ustring& symbol_file)
{
    std::ifstream sfilestrm(symbol_file,std::fstream::in);
    if(sfilestrm.good())
    {
        LTSpiceSymbolParser parser(sfilestrm);
        if(parser.parse() == 0)
            _symbol = std::make_shared<ObjectSymbol>(parser.get_symbol());
        else
            std::cerr << "Encountered error while reading file: " << symbol_file << "\n";
    }
    else
        std::cerr << "Error: Could not open file: " << symbol_file << "\n";
}

bool GtkSpiceElement::set_symbol_file(const Glib::ustring& symbol_file)
{
    std::ifstream sfilestrm(symbol_file,std::fstream::in);
    if(sfilestrm.good())
    {
        LTSpiceSymbolParser parser(sfilestrm);
        if(parser.parse() == 0)
        {
            _symbol = std::make_shared<ObjectSymbol>(parser.get_symbol());
            return true;
        }
        else
            std::cerr << "Encountered error while reading file: " << symbol_file << "\n";
    }
    else
        std::cerr << "Error: Could not open file: " << symbol_file << "\n";
    return false;
}

void GtkSpiceElement::set_name(Glib::ustring name)
{
    // Note: It is the schematic's responsibility to manage names
    if(_symbol)
    {
        _inst_name = _symbol->get_attribute_value("PREFIX") + name;
        _symbol->set_attribute_value("INSTNAME",_inst_name);
    }
}

void GtkSpiceElement::draw(Cairo::RefPtr<Cairo::Context> context)
{
    if(_symbol)
        _symbol->draw(context);
}
