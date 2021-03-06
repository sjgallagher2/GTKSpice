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

#ifndef WORKSPACE_KEYACCEL_H
#define WORKSPACE_KEYACCEL_H

#include <memory>
#include <app/action.h>
#include <app/key_accel_map.h>

class WorkspaceKeyAccel
{
public:
    WorkspaceKeyAccel(std::shared_ptr<ActionFactory> actionfactory,
        std::shared_ptr<KeyAccelMap> keymap);
    ~WorkspaceKeyAccel();
    
    typedef sigc::signal<bool,std::shared_ptr<Action>> new_action_type;
    new_action_type new_action() const {return _new_action;}
private:
    new_action_type _new_action;
    std::shared_ptr<ActionFactory> _actionfactory;

    std::shared_ptr<KeyAccelMap> _keymap;
};

#endif /* WORKSPACE_KEYACCEL_H */
