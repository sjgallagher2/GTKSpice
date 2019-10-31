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

#include <iostream>
#include <gui/window.h>

Window::Window()
{
    add_events(Gdk::SCROLL_MASK);
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
    add_events(Gdk::POINTER_MOTION_MASK);
    add_events(Gdk::KEY_PRESS_MASK);
    add_events(Gdk::KEY_RELEASE_MASK);

    //add(_drawevents);
	//GtkSpiceState::init(&_drawevents);
    //_drawevents.show();

    set_title("GTKSpice");
    maximize();
    signal_key_press_event().connect(sigc::mem_fun(*this,&Window::on_key_press_event));
    // TODO Does Window need to see these? Maybe just pass along?
    /*
    signal_key_press_event().connect(sigc::mem_fun(_drawevents,&DrawingEventBox::on_key_press_event));
    signal_button_press_event().connect(sigc::mem_fun(_drawevents, &DrawingEventBox::on_button_press_event));
    signal_button_release_event().connect(sigc::mem_fun(_drawevents, &DrawingEventBox::on_button_release_event));
    signal_motion_notify_event().connect(sigc::mem_fun(_drawevents, &DrawingEventBox::on_mouse_move_event));
    */

}
Window::~Window()
{}

bool Window::on_key_press_event(GdkEventKey* key_event)
{
    return false;
}
