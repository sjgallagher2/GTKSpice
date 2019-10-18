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

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <app/spice_data.h>
#include <app/schematic.h>
#include <app/workspace_keyaccel.h>
#include <app/canvas.h>
#include <app/action_stack.h>

class Workspace
{
public:
    Workspace();
    ~Workspace();

protected:
    SpiceData* _spicedata;
    WorkspaceKeyAccel* _keyaccel;
    Canvas* _canvas;
    ActionStack* _actionstack;
    ActionFactory* _actionfactory;
};

#endif /* WORKSPACE_H */