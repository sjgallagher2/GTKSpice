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

#include <gui/drawingeventbox.h>
#include <gui/view_features.h>
#include <app/gtkspice_state.h>
#include <app/action.h>

class Canvas
{
public:
    Canvas(std::shared_ptr<const ObjectTree> ot);
    virtual ~Canvas();

    typedef sigc::signal<bool,std::shared_ptr<Action>> new_action_type;
    new_action_type new_action() const {return _new_action;}
protected:
    std::shared_ptr<const ObjectTree> _objecttree;
    std::shared_ptr<DrawingEventBox> _ebox;
    std::shared_ptr<ViewFeatures> _vfeatures;
    std::shared_ptr<GtkSpiceState> _state;

    new_action_type _new_action;
};

#endif /* CANVAS_H */
