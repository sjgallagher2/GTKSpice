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
#include <gui/view_features.h>

ViewFeatures::ViewFeatures()
{
    /*
    _rp.cp.editable = false;
    _rp.cp.filled = false;
    _rp.cp.is_active = false;
    _rp.cp.selectable = false;
    _rp.cp.stroke_thickness = 1;
    _rp.cp.red = 0.4;
    _rp.cp.blue = 0.4;
    _rp.cp.green = 0.4;
    _temp_rect = Rect(_rp);
    */
}

void ViewFeatures::draw_features(const Cairo::RefPtr<Cairo::Context>& context)
{
    // Draw all features to context
    /*
    if(_rp.cp.is_active)
    {
        double stroke = _rp.cp.stroke_thickness;
        context->device_to_user_distance(stroke,stroke);
        _rp.cp.stroke_thickness = stroke;
        _temp_rect.set_params(_rp);
        _temp_rect.draw(context);

        context->user_to_device_distance(stroke,stroke);
        _rp.cp.stroke_thickness = stroke;
        _temp_rect.set_params(_rp);
    }
    */
}

void ViewFeatures::StartTempRectangle(const Coordinate& mousepos)
{
    /*
    // Start the temporary rectangle
    _rp.left = mousepos.x();
    _rp.top = mousepos.y();
    // Init lower right corner
    _rp.right = mousepos.x();
    _rp.bottom = mousepos.y();
    _rp.cp.is_active = true;
    _temp_rect.set_params(_rp);
    */
}

void ViewFeatures::UpdateTempRectangle(const Coordinate& mousepos)
{
    /*
    // Update rectangle lower-right corner
    if(_rp.cp.is_active)
    {
        _rp.bottom = mousepos.y();
        _rp.right = mousepos.x();
        _temp_rect.move_corner(mousepos);
    }
    */
}

void ViewFeatures::FinishTempRectangle()
{
    /*
    // Delete the temporary rectangle
    _rp.cp.is_active = false;
    _temp_rect.set_params(_rp);
    */
}
