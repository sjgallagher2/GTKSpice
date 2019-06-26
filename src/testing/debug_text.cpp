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

#include <gtkmm.h>
#include <iostream>
#include <cairomm/context.h>
#include <testing/debug_text.h>

DebugText::DebugText()
{ 
}

DebugText::~DebugText()
{
}

void DebugText::display_coordinate(const Cairo::RefPtr<Cairo::Context>& context, std::string label, float x, float y, int line)
{
    double xd = (double)x;
    double yd = (double)y;

    context->save();
    context->select_font_face("monospace", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
    context->set_font_size (10);
    context->set_source_rgb (0, 0, 0);
    context->move_to(10, 10+10*line);
    std::string coor = label;
    coor += "(";
    coor += std::to_string(x);
    coor += ", ";
    coor += std::to_string(y);
    coor += ")/(";

    context->device_to_user(xd,yd);

    coor += std::to_string(xd);
    coor += ", ";
    coor += std::to_string(yd);
    coor += ")";

    context->show_text(coor);
    context->restore();
}

void DebugText::display_value(const Cairo::RefPtr<Cairo::Context>& context, std::string label, float x, int line)
{
    context->save();
    context->select_font_face("monospace", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
    context->set_font_size (10);
    context->set_source_rgb (0, 0, 0);
    context->move_to(10, 10+10*line);
    std::string coor = label;
    coor += std::to_string(x);
    context->show_text(coor);
    context->restore();
}
