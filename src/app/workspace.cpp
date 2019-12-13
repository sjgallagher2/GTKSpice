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

    _spicedata = std::make_shared<SpiceData>();
    _actionstack = std::make_shared<ActionStack>();
    
    _schem = std::make_shared<GtkSpiceSchematic>();
    _keyaccel = std::make_shared<WorkspaceKeyAccel>(_actionfactory,_keymap); 
    _canvas = std::make_shared<Canvas>(_toplevel,_schem,_actionfactory,_keymap);

    _actionfactory->update(
        _schem,_canvas,_actionstack,
        _canvas->get_gtkspice_state(),
        _canvas->get_tool_manager(),
        _canvas->get_view_features());

    _canvas->new_action().connect(sigc::mem_fun(*this,&Workspace::get_action));
    _canvas->new_cursor().connect(sigc::mem_fun(*this,&Workspace::get_cursor));

    /* TESTING */
    if(_schem->get_active_sheet())
    {
        std::shared_ptr<GtkSpiceElementList> elems = _schem->get_active_sheet()->get_element_list();
        std::shared_ptr<GtkSpiceWireList> wires = _schem->get_active_sheet()->get_wire_list();
        std::shared_ptr<GtkSpicePortList> ports = _schem->get_active_sheet()->get_port_list();
        std::shared_ptr<NodeManager> nodemgr = _schem->get_active_sheet()->get_node_manager();
        if(nodemgr->empty())
        {
            nodemgr->add_node("0");
            nodemgr->add_node("vin");
            nodemgr->add_node("vout");

            Glib::ustring v1name = elems->add_element("/home/sam/Documents/Electronics/SPICE/lib/sym/voltage.asy",Coordinate(10,10));
            Glib::ustring res1name = elems->add_element("/home/sam/Documents/Electronics/SPICE/lib/sym/ind.asy",Coordinate(80,0));
            elems->find_element(res1name)->rotate90();
            wires->add_wire(nodemgr->find_node("vin"),Coordinate(10,18),Coordinate(10,8));
            wires->add_wire(nodemgr->find_node("vin"),Coordinate(10,8),Coordinate(32,8));
            wires->add_wire(nodemgr->find_node("vout"),Coordinate(72,8),Coordinate(88,8));

            ports->add_gnd_port(Coordinate(10,48));
            wires->add_wire(nodemgr->find_node("0"),Coordinate(10,58),Coordinate(10,68));
            if(ports->get_port_under_cursor(Coordinate(10,69)))
                ports->get_port_under_cursor(Coordinate(10,69))->set_highlight(false);

            nodemgr->connect_node("vin",elems->find_element(res1name),2);
            nodemgr->connect_node("vin",elems->find_element(v1name),1);
            nodemgr->connect_node("0",elems->find_element(v1name),2);
            nodemgr->connect_node("vout", elems->find_element(res1name),1);

            std::shared_ptr<SchematicSheet> sheet = _schem->get_active_sheet();
            sheet->add_element("/home/sam/Documents/Electronics/SPICE/lib/sym/cap.asy",Coordinate(80,8));

            std::string spicelines = _schem->get_spice_lines();
            std::cout << spicelines;
        }
    }
    /***********/
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
