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

#include <app/workspace.h>
#include <iostream>

Workspace::Workspace(std::shared_ptr<Window> toplevel) :
    _toplevel(toplevel)
{
    _keymap = std::make_shared<KeyAccelMap>();
    _keymap->set_defaults();
    _actionfactory = std::make_shared<ActionFactory>();

    _objecttree = std::make_shared<ObjectTree>();
    _spicedata = std::make_shared<SpiceData>();
    _actionstack = std::make_shared<ActionStack>();
    
    _schem = std::make_shared<Schematic>(_objecttree);
    _keyaccel = std::make_shared<WorkspaceKeyAccel>(_actionfactory,_keymap); 
    _canvas = std::make_shared<Canvas>(_toplevel,_objecttree,_actionfactory,_keymap);

    _actionfactory->update(
        _objecttree,
        _schem,_canvas,_actionstack,
        _canvas->get_gtkspice_state(),
        _canvas->get_tool_manager(),
        _canvas->get_view_features());

    _canvas->new_action().connect(sigc::mem_fun(*this,&Workspace::get_action));
    _canvas->new_cursor().connect(sigc::mem_fun(*this,&Workspace::get_cursor));
}

Workspace::~Workspace()
{}

bool Workspace::get_action(std::shared_ptr<Action> a)
{
    // Push to action stack
    _actionstack->push(a);
}
bool Workspace::get_cursor(Glib::ustring cursor)
{
    _toplevel->set_new_cursor(cursor);
}
