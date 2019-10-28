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

#ifndef COORDINATE_H
#define COORDINATE_H

#include <gtkmm.h>

// A coordinate class used for storing screen and application coordinate points
// Coordinates are always stored as doubles
class Coordinate
{
public:
    Coordinate() {}
    Coordinate(double X, double Y) {x(X);y(Y);}
    virtual ~Coordinate() {};

    void set_to_user_coordinate(const Cairo::RefPtr<Cairo::Context>& context);
    void set_to_user_distance(const Cairo::RefPtr<Cairo::Context>& context);
    void set_to_device_coordinate(const Cairo::RefPtr<Cairo::Context>& context);
    void set_to_device_distance(const Cairo::RefPtr<Cairo::Context>& context);
    void set_coordinate(double X, double Y,bool snapped=false) { x(X,snapped); y(Y,snapped); }
    void set_to_snapped();
    
    /* Sets and Gets */
    virtual void x(const double X, bool snapped = false);
    virtual void y(const double Y, bool snapped = false);
    virtual double x()  const { return _x; }
    virtual double y() const { return _y; }

    /* Static */   
    static void grid(float g) {_grid = g;}
    static float grid() {return _grid;}

    /* Operator overloads */
    virtual bool operator==(const Coordinate& r) const {return (r.x() == _x && r.y() == _y);}

protected:
    double _x;
    double _y;
    static float _grid; // Grid interval

};

class Vertex : public Coordinate
{
public:
    Vertex() {}
    Vertex(double X,double Y) : Coordinate(X,Y) {}
    Vertex(Coordinate v) : Coordinate(v.x(),v.y()) {}
    Vertex(int index) : _index(index) {}
    Vertex(double X,double Y,int index) : Coordinate(X,Y),_index(index) {}
    Vertex(Coordinate v,int index) : Coordinate(v.x(),v.y()),_index(index) {}

    bool active() const {return _is_active;}
    void active(bool a) {_is_active = a;}
    int index() const {return _index;}
    void index(int i) {_index = i;}

private:
    int _index = -1;
    bool _is_active = false;
};


#endif /* COORDINATE_H */