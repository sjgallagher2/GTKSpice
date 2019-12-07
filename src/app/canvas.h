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
#include <app/gtkspice_object_list.h>
#include <app/gtkspice_state.h>
#include <app/action.h>
#include <app/coordinate_system.h>
#include <tools/tool_manager.h>

class Action;
class ActionFactory;
class DrawingEventBox;
class GtkSpiceState;
class Window;
class KeyAccelMap;
class ViewFeatures;

class PointerTool;

class Canvas
{
public:
    Canvas(std::shared_ptr<Window> toplevel, 
        std::shared_ptr<ActionFactory> af,
        std::shared_ptr<KeyAccelMap> keymap);
    virtual ~Canvas();

    std::shared_ptr<GtkSpiceState> get_gtkspice_state() const {return _state;}
    std::shared_ptr<ToolManager> get_tool_manager() const {return _toolmgr;}
    std::shared_ptr<ViewFeatures> get_view_features() const {return _vfeatures;}

    void click_handler(Coordinate mousepos, int button, int modifier, int cselect);
    void move_handler(Coordinate mousepos);
    void key_handler(int key,int modifier);
    void scroll_handler(Coordinate mousepos, int scroll_dir);
    void update_cursor();

    typedef sigc::signal<bool,std::shared_ptr<Action>> new_action_type;
    new_action_type new_action() const {return _new_action;}
    typedef sigc::signal<bool,Glib::ustring> new_cursor_type;
    new_cursor_type new_cursor() const {return _new_cursor;}
protected:
    std::shared_ptr<ActionFactory> _actionfactory;
    std::shared_ptr<Window> _toplevel;
    std::shared_ptr<DrawingEventBox> _ebox;
    std::shared_ptr<ViewFeatures> _vfeatures;
    std::shared_ptr<GtkSpiceState> _state;
    std::shared_ptr<CoordinateSystem> _cs;
    std::shared_ptr<ToolManager> _toolmgr;

    std::shared_ptr<KeyAccelMap> _keymap;

    new_action_type _new_action;
    new_cursor_type _new_cursor;

    bool send_test;
};

#endif /* CANVAS_H */
