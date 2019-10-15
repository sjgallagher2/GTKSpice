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

#include <app/gtkspice.h>
#include <app/object_tree.h>

/* APPLICATION */
GTKSpice::GTKSpice():
	Gtk::Application("com.gtkspice.GTKSpice", Gio::APPLICATION_HANDLES_OPEN)
{
}

void GTKSpice::on_activate()
{
	// Startup procedure (where there are no input args)
	// Initialize
	//_win.set_default_size(1600, 800);
	add_window(_win);
	ObjectTree::init();
    _as = ActionStack::Instance();
	_win.present();
}

Glib::RefPtr<GTKSpice> GTKSpice::create()
{
	return Glib::RefPtr<GTKSpice>(new GTKSpice());
}

int main(int argc, char* argv[])
{
	auto app =
			GTKSpice::create();


	return app->run();
}

