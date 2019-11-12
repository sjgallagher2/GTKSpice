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

/* Note: I don't know if I'm using this widget correctly, but it does
add a much needed layer of abstraction so things like this aren't handled
in the view class */

#include <iostream>
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <app/coordinate.h>
#include <app/coordinate_system.h>
#include <gui/drawingeventbox.h>
#include <app/gtkspice_state.h>

DrawingEventBox::DrawingEventBox(std::shared_ptr<CoordinateSystem> cs) : 
    _v(std::make_shared<View>(cs)),_keyaccel(new DrawingEventBoxKeyAccel)
{
    add_events(Gdk::ENTER_NOTIFY_MASK);
    add_events(Gdk::LEAVE_NOTIFY_MASK);

    signal_enter_notify_event().connect(sigc::mem_fun(*this,&DrawingEventBox::on_mouse_cross_event));
    signal_leave_notify_event().connect(sigc::mem_fun(*this,&DrawingEventBox::on_mouse_cross_event));

    add(*_v);
    show_all_children();
    show();
}

DrawingEventBox::~DrawingEventBox()
{}

void DrawingEventBox::set_object_tree(std::shared_ptr<ObjectTree> ot)
{
    _v->set_object_tree(ot);
}
void DrawingEventBox::set_view_features(std::shared_ptr<ViewFeatures> vf)
{
    _v->set_view_features(vf);
}


bool DrawingEventBox::on_mouse_cross_event(GdkEventCrossing* cross_event)
{
    if(cross_event->type == GDK_ENTER_NOTIFY)
    {
    }
    else if(cross_event->type == GDK_LEAVE_NOTIFY)
    {
        // Set cursor to default
        _v->unset_cursor();
    }
    _v->force_redraw();
}

bool DrawingEventBox::on_button_press_event(GdkEventButton* button_event)
{
    // TODO: All instances of click handling and key handling must be updated
    Coordinate mousepos(button_event->x,button_event->y);
    _view_mat = _v->tmatrix();
    mousepos.set_view_matrix(_view_mat);
    mousepos.set_to_user_coordinate();

    switch(button_event->button)
    {
    case 1:
        if(button_event->type == GDK_2BUTTON_PRESS)
            _mouse_button = static_cast<int>(DOUBLE_LEFT);
        else
            _mouse_button = static_cast<int>(LEFT_PRESS);
        break;
    case 2:
        _mouse_button = static_cast<int>(MIDDLE_PRESS);
        break;
    case 3:
        if(button_event->type == GDK_2BUTTON_PRESS)
            _mouse_button = static_cast<int>(DOUBLE_RIGHT);
        else
            _mouse_button = static_cast<int>(RIGHT_PRESS);
        break;
    default:
        break;
    }

    switch(button_event->state)
    {
    case GDK_SHIFT_MASK:
        _modifier = static_cast<int>(SHIFT);
        break;
    case GDK_CONTROL_MASK:
        _modifier = static_cast<int>(CTRL);
        break;
    case GDK_MOD1_MASK:
        _modifier = static_cast<int>(ALT);
        break;
    default:
        _modifier = static_cast<int>(NO_MOD);
        break;
    }
    
    //int lineselect = ObjectTree::get_line_under_cursor(mousepos);
    int lineselect = -1;
    _button_click.emit(mousepos,_mouse_button,_modifier,lineselect);
    
    return true;
}

bool DrawingEventBox::on_button_release_event(GdkEventButton* button_event)
{
    Coordinate mousepos(button_event->x,button_event->y);
    _view_mat = _v->tmatrix();
    mousepos.set_view_matrix(_view_mat);
    mousepos.set_to_user_coordinate();

    switch(button_event->button)
    {
    case 1:
        if(_mouse_button != static_cast<int>(DOUBLE_LEFT))
            _mouse_button = static_cast<int>(LEFT_RELEASE);
        break;
    case 2:
        _mouse_button = static_cast<int>(MIDDLE_RELEASE);
        break;
    case 3:
        if(_mouse_button != static_cast<int>(DOUBLE_RIGHT))
            _mouse_button = static_cast<int>(RIGHT_RELEASE);
        break;
    default:
        break;
    }
    switch(button_event->state)
    {
    case GDK_SHIFT_MASK:
        _modifier = static_cast<int>(SHIFT);
        break;
    case GDK_CONTROL_MASK:
        _modifier = static_cast<int>(CTRL);
        break;
    case GDK_MOD1_MASK:
        _modifier = static_cast<int>(ALT);
        break;
    default:
        _modifier = static_cast<int>(NO_MOD);
        break;
    }

    //int lineselect = ObjectTree::get_line_under_cursor(mousepos);
    int lineselect = -1;
    _button_click.emit(mousepos,_mouse_button,_modifier,lineselect);
    
    return true;
}

bool DrawingEventBox::on_mouse_move_event(GdkEventMotion* movement_event)
{
//    auto win = _v->get_window();
//    win->set_cursor(_v.get_cursor());

    Coordinate mousepos(movement_event->x,movement_event->y);
    _view_mat = _v->tmatrix();
    mousepos.set_view_matrix(_view_mat);
    mousepos.set_to_user_coordinate();
    
    _mouse_move.emit(mousepos);
    
    return true;
}

bool DrawingEventBox::on_scroll_wheel_event(GdkEventScroll* scroll_event)
{
    Coordinate mousepos(scroll_event->x,scroll_event->y);
    _view_mat = _v->tmatrix();
    mousepos.set_view_matrix(_view_mat);
    mousepos.set_to_user_coordinate();
    ScrollDirections dir;
    if(scroll_event->direction == GDK_SCROLL_UP)
    {
        dir = SCROLL_UP;
    }
    else if(scroll_event->direction == GDK_SCROLL_DOWN)
    {
        dir = SCROLL_DOWN;
    }
    
    _scroll_wheel.emit(mousepos,dir);
    return true;
}

bool DrawingEventBox::on_key_press_event(GdkEventKey* key_event)
{
    auto win = _v->get_window();
//    win->set_cursor(_v.get_cursor());
    
    switch(key_event->state)
    {
    case GDK_SHIFT_MASK:
        _modifier = static_cast<int>(SHIFT);
        break;
    case GDK_CONTROL_MASK:
        _modifier = static_cast<int>(CTRL);
        break;
    case GDK_MOD1_MASK:
        _modifier = static_cast<int>(ALT);
        break;
    default:
        _modifier = static_cast<int>(NO_MOD);
        break;
    }
    
    // Emit key press signal
    _key_press.emit(key_event->keyval,_modifier);
    
    return true;
}

void DrawingEventBox::force_redraw()
{
    _v->force_redraw();
}
