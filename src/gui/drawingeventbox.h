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

#include <memory>
#include <gtkmm/eventbox.h>
#include <cairomm/matrix.h>
#include <app/schematic.h>
#include <gui/view_features.h>
#include <gui/drawingeventbox_keyaccel.h>
#include <gui/view.h>

class View;

enum MouseClicks {LEFT_PRESS=1,LEFT_RELEASE, 
    MIDDLE_PRESS, MIDDLE_RELEASE,
    RIGHT_PRESS, RIGHT_RELEASE, 
    DOUBLE_LEFT, DOUBLE_RIGHT};
enum KeyModifiers {NO_MOD,SHIFT,CTRL,ALT};
enum ScrollDirections {SCROLL_UP, SCROLL_DOWN};

class DrawingEventBox : public Gtk::EventBox
{
public:
    DrawingEventBox(std::shared_ptr<CoordinateSystem> cs);
    virtual ~DrawingEventBox();

    void set_view_features(std::shared_ptr<ViewFeatures> vf);
    void set_view_schematic(std::shared_ptr<GtkSpiceSchematic> sch);

    Cairo::Matrix get_view_matrix() const {return _view_mat;}
    
    typedef sigc::signal<void,Coordinate,int,int,int> clicksig_type;
    clicksig_type button_click() const {return _button_click;}
    typedef sigc::signal<void,Coordinate> movesig_type;
    movesig_type mouse_move() const {return _mouse_move;}
    typedef sigc::signal<void,int,int> keypress_sig_type;
    keypress_sig_type key_press() const {return _key_press;}
    typedef sigc::signal<void,Coordinate,int> scroll_sig_type;
    scroll_sig_type scroll_wheel() const {return _scroll_wheel;}

    virtual bool on_key_press_event(GdkEventKey* event) override;
    virtual bool on_button_press_event(GdkEventButton* button_event);
    virtual bool on_button_release_event(GdkEventButton* release_event);
    virtual bool on_mouse_move_event(GdkEventMotion* movement_event);
    virtual bool on_mouse_cross_event(GdkEventCrossing* cross_event);
    virtual bool on_scroll_wheel_event(GdkEventScroll* scroll_event);

    void force_redraw();
protected:
    // SIGNALS
    clicksig_type _button_click;
    movesig_type _mouse_move;
    keypress_sig_type _key_press;
    scroll_sig_type _scroll_wheel;
    //
    //bool on_key_release_event(GdkEventKey* event) override;
    int _mouse_button = 0;
    int _modifier = 0;
    std::shared_ptr<View> _v; 
    DrawingEventBoxKeyAccel* _keyaccel;
    Cairo::Matrix _view_mat;
    
};


#endif /* DRAWINGEVENTBOX_H */
