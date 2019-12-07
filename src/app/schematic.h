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

#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include <memory>
#include <app/gtkspice_object_list.h>
#include <app/gtkspice_object.h>

class SchematicSheet
{
public:
    SchematicSheet();
    ~SchematicSheet();

    void draw(Cairo::RefPtr<Cairo::Context> context)
    {
        _elementlist->draw(context);
        _wirelist->draw(context);
    }
    int get_id() const {return _sheet_id;}
    void set_id(int id) {_sheet_id = id;}

private:
    std::shared_ptr<GtkSpiceElementList> _elementlist;
    std::shared_ptr<GtkSpiceWireList> _wirelist;
    std::shared_ptr<NodeManager> _nodemanager;
    int _sheet_id;
};

class GtkSpiceSchematic
{
public:
    GtkSpiceSchematic() {}
    ~GtkSpiceSchematic() {}

    void redraw(Cairo::RefPtr<Cairo::Context> context, int sheet)
    {
        if(sheet < _sheets.size())
            _sheets.at(sheet)->draw(context);
    }

private:
    std::vector<std::shared_ptr<SchematicSheet>> _sheets;

};

#endif /* SCHEMATIC_H */
