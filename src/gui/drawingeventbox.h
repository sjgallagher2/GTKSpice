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

#ifndef DRAWINGEVENTBOX_H
#define DRAWINGEVENTBOX_H

#include <gtkmm/eventbox.h>
#include <gui/drawingeventbox_keyaccel.h>
#include <gui/view.h>

class View;

class DrawingEventBox : public Gtk::EventBox
{
public:
    DrawingEventBox();
    virtual ~DrawingEventBox();
    
    typedef sigc::signal<void,Coordinate,int,int,int> clicksig_type;
    clicksig_type button_click() const {return _button_click;}
    typedef sigc::signal<void,Coordinate> movesig_type;
    movesig_type mouse_move() const {return _mouse_move;}
    typedef sigc::signal<void,int,int> keypress_sig_type;
    keypress_sig_type key_press() const {return _key_press;}

    virtual bool on_key_press_event(GdkEventKey* event) override;
    virtual bool on_button_press_event(GdkEventButton* button_event);
    virtual bool on_button_release_event(GdkEventButton* release_event);
    virtual bool on_mouse_move_event(GdkEventMotion* movement_event);
    virtual bool on_mouse_cross_event(GdkEventCrossing* cross_event);

    void force_redraw();
protected:
    // SIGNALS
    clicksig_type _button_click;
    movesig_type _mouse_move;
    keypress_sig_type _key_press;
    //
    //bool on_key_release_event(GdkEventKey* event) override;
    int _mouse_button = 0;
    int _modifier = 0;
    View* _v; // TODO Should the view be here? 
    DrawingEventBoxKeyAccel* _keyaccel;
    
};


#endif /* DRAWINGEVENTBOX_H */
