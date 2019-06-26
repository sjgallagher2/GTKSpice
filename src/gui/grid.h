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

#ifndef GRID_H
#define GRID_H

#include <gtkmm.h>
#include <app/coordinate_system.h>
#include <app/coordinate.h>

class Grid
{
public:
    Grid();
    virtual ~Grid();

    void draw_origin(const Cairo::RefPtr<Cairo::Context>& context, float scale);
    void draw_grid(const Cairo::RefPtr<Cairo::Context>& context, const float scale,
                    const int left, const int right, const int bottom, const int top);

private:
    double _radius;
};

#endif /* GRID_H */
