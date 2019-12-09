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

#include <memory>
#include <gui/window.h>
#include <app/schematic.h>
#include <app/spice_data.h>
#include <app/schematic.h>
#include <app/workspace_keyaccel.h>
#include <app/canvas.h>
#include <app/action.h>
#include <app/action_stack.h>
#include <app/key_accel_map.h>

/* A Workspace represents the program session for one or more schematics,
 * managing the Canvas, ActionStack, Schematic, and SimulationData for
 * the current session. 
 *
 */
class Workspace
{
public:
    Workspace(std::shared_ptr<Window> toplevel);
    ~Workspace();
    
protected:
    std::shared_ptr<Window> _toplevel;
    std::shared_ptr<ActionFactory> _actionfactory;
    std::shared_ptr<SpiceData> _spicedata;
    std::shared_ptr<ActionStack> _actionstack;
    std::shared_ptr<GtkSpiceSchematic> _schem;
    std::shared_ptr<WorkspaceKeyAccel> _keyaccel;
    std::shared_ptr<Canvas> _canvas;

    std::shared_ptr<KeyAccelMap> _keymap;

    bool get_action(std::shared_ptr<Action> a);
    bool get_cursor(Glib::ustring cursor);
};

#endif /* WORKSPACE_H */