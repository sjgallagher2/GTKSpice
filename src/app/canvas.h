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

#ifndef CANVAS_H
#define CANVAS_H

#include <gtkmm.h>
#include <memory>

#include <gui/window.h>
#include <gui/drawingeventbox.h>
#include <gui/view_features.h>
#include <app/gtkspice_state.h>
#include <app/action.h>
#include <app/coordinate_system.h>
#include <tools/tool_component_drag.h>
#include <tools/tool_component_fliplr.h>
#include <tools/tool_component_flipud.h>
#include <tools/tool_component_rotateccw.h>
#include <tools/tool_component_rotatecw.h>
#include <tools/tool_delete.h>
#include <tools/tool_draw_wire.h>
#include <tools/tool_text_add.h>
#include <tools/tool_text_modify.h>
#include <tools/tool_view_pan.h>
#include <tools/tool_pointer.h>

class Action;
class ActionFactory;
class DrawingEventBox;
class GtkSpiceState;
class Window;

class PointerTool;

class Canvas
{
public:
    Canvas(std::shared_ptr<Window> toplevel, std::shared_ptr<ObjectTree> ot, std::shared_ptr<ActionFactory> af);
    virtual ~Canvas();

    std::shared_ptr<const ObjectTree> get_canvas_object_tree() const {return _objecttree;}

    void click_handler(Coordinate mousepos, int button, int modifier, int cselect);
    void move_handler(Coordinate mousepos);
    void key_handler(int key,int modifier);

    void send_test_action(Coordinate x,int y,int z,int t);
    typedef sigc::signal<bool,std::shared_ptr<Action>> new_action_type;
    new_action_type new_action() const {return _new_action;}
protected:
    std::shared_ptr<ObjectTree> _objecttree;
    std::shared_ptr<ActionFactory> _actionfactory;
    std::shared_ptr<Window> _toplevel;
    std::shared_ptr<DrawingEventBox> _ebox;
    std::shared_ptr<ViewFeatures> _vfeatures;
    std::shared_ptr<GtkSpiceState> _state;
    std::shared_ptr<CoordinateSystem> _cs;

    std::shared_ptr<PointerTool> _pointer; // TODO Add the other tools



    new_action_type _new_action;

    bool send_test;
};

#endif /* CANVAS_H */
