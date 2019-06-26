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

#include <cairomm/context.h>
#include <app/coordinate.h>
#include <math.h>


void Coordinate::set_to_user_coordinate(
    const Cairo::RefPtr<Cairo::Context>& context)
{
    context->device_to_user(_x, _y);
}

void Coordinate::set_to_user_distance(
    const Cairo::RefPtr<Cairo::Context>& context)
{
    context->device_to_user_distance(_x, _y);
}
void Coordinate::set_to_device_coordinate(
    const Cairo::RefPtr<Cairo::Context>& context)
{
    context->user_to_device(_x, _y);
}

void Coordinate::set_to_device_distance(
    const Cairo::RefPtr<Cairo::Context>& context)
{
    context->user_to_device_distance(_x, _y);
}
