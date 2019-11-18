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

#ifndef GTKSPICE_OBJECT_H
#define GTKSPICE_OBJECT_H

#include <gtkmm.h>
#include <memory>
#include <app/coordinate.h>
#include <app/object_symbol.h>

class GtkSpiceObject 
{
public:
    GtkSpiceObject() {}
    virtual ~GtkSpiceObject() {}
    int index;
    bool active;
    void draw(Cairo::RefPtr<Cairo::Context> context);
    Coordinate position;
    BoundingBox boundingbox;
    
    std::shared_ptr<ObjectSymbol> symbol;
    
    bool near(const Coordinate& pos); // Return true if Coordinate is in the BoundingBox
    bool under(const Coordinate& pos); // Return true if Coordinate is on the Symbol
};


class CompositeObject : public GtkSpiceObject 
{};

class Wire : public GtkSpiceObject 
{};

class ObjectFactory 
{};

#endif /* GTKSPICE_OBJECT_H */
