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

#ifndef VIEW_FEATURES
#define VIEW_FEATURES

#include <gtkmm.h>
#include <vector>
#include <app/component.h>
#include <app/coordinate.h>

class ViewFeatures
{
public:
    ViewFeatures();
    ~ViewFeatures() {}

    void draw_features(const Cairo::RefPtr<Cairo::Context>& context);

    void StartTempRectangle(const Coordinate& mousepos);
    void UpdateTempRectangle(const Coordinate& mousepos);
    void FinishTempRectangle();

private:
    //RectParameters _rp;
    //Rect _temp_rect;
};

#endif /* VIEW_FEATURES */
