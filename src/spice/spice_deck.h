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

#ifndef SPICE_DECK_H
#define SPICE_DECK_H

/* SPICE Deck
 * A collection of circuit elements and their connections
 * with necessary directives
 */

#include <iostream>
#include <vector>
#include <spice/spice_element.h>
#include <spice/spice_directive.h>

class SpiceDeck
{
public:
    /* @brief Add an element to the SPICE deck 
     * @params
     * line     One full line in a SPICE deck
     * 
     * @return  Line index
     * 
     */
    void add_line(Glib::ustring line)
    {
        _lines.push_back(line);
    }

    /* @brief Remove a line from the SPICE deck
     * @params
     * 
     * @return None
     * 
     */
    void remove_line()
    {
    }

    /* @brief Return a ustring with all lines, separated by \n newlines
     * @params None
     * @return ustring  String containing SPICE deck lines
     * 
     */
    Glib::ustring get_spice_lines()
    {
        Glib::ustring ret = "";
        for(auto it = _lines.begin(); it != _lines.end(); ++it)
            if(it+1 != _lines.end())
                ret.append((*it) + "\n");
            else
                ret.append(*it);
        return ret;
    }

    /* @brief Print the SPICE deck 
     * @params None
     * 
     * @return None
     * 
     */
    void print()
    {
        for(auto it = _lines.begin(); it != _lines.end(); ++it)
            std::cout << (*it) <<"\n";
    }

    /* @brief Run a simulation
     * @params
     * 
     * @return 
     * 
     */
    //void run_simulation();

    /* @brief Get simulation data
     * @params
     * 
     * @return 
     * 
     */
    //void get_data();

    /* @brief Get netlist in ngspice format
     * @params
     * 
     * @return
     * 
     */
    //void get_netlist();

protected:
    std::vector<Glib::ustring> _lines;
};

typedef SpiceDeck SpiceSubcircuitDeck;

#endif /* SPICE_DECK_H */
