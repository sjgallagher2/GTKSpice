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

class Action;
class ActionFactory;
class DrawingEventBox;
class GtkSpiceState;
class Window;

class Canvas
{
public:
    Canvas(std::shared_ptr<Window> toplevel, std::shared_ptr<ObjectTree> ot, std::shared_ptr<ActionFactory> af);
    virtual ~Canvas();

    std::shared_ptr<const ObjectTree> get_canvas_object_tree() const {return _objecttree;}

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

    new_action_type _new_action;

    bool send_test;
};

#endif /* CANVAS_H */
