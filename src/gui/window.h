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

#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <gtkmm.h>
#include <gui/drawingeventbox.h>
#include <app/gtkspice_state.h>
#include <gui/cursor_manager.h>

/* 
 * A simple window class. Subclass of Gtk::ApplicationWindow.
 */

class Window : public Gtk::ApplicationWindow
{
public:
	Window();
	~Window();

    //void set_view_object_tree(std::shared_ptr<ObjectTree> ot) {_drawevents.set_object_tree(ot);}
	void set_new_cursor(Glib::ustring cursor);

protected:
	//DrawingEventBox _drawevents;
	virtual bool on_key_press_event(GdkEventKey* key_event) override;
	//void on_action_quit();
	CursorManager _cmanager;
};

#endif /* WINDOW_H */