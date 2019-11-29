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
    Coordinate() : _view_mat(1,0,0,1,0,0) {}
    Coordinate(double X, double Y) {x(X);y(Y);}
    Coordinate(const Coordinate& r) {x(r.x());y(r.y());_view_mat=r.get_view_matrix();}
    virtual ~Coordinate() {};
    
    void set_view_matrix(Cairo::Matrix tmat) {_view_mat = tmat;}
    Cairo::Matrix get_view_matrix() const { return _view_mat;}

    void set_to_user_coordinate(const Cairo::RefPtr<Cairo::Context>& context);
    void set_to_user_coordinate(); // This uses _view_matrix, which defaults to identity
    
    void set_to_user_distance(const Cairo::RefPtr<Cairo::Context>& context);
    void set_to_user_distance();
    
    void set_to_device_coordinate(const Cairo::RefPtr<Cairo::Context>& context);
    void set_to_device_coordinate();

    void set_to_device_distance(const Cairo::RefPtr<Cairo::Context>& context);
    void set_to_device_distance();

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
    virtual Coordinate operator+(const Coordinate& r) const {return Coordinate(r.x() + _x, r.y() + _y);}
    virtual Coordinate operator-(const Coordinate& r) const {return Coordinate(r.x() - _x, r.y() - _y);}
    virtual Coordinate operator*(const Coordinate& r) const {return Coordinate(r.x() * _x, r.y() + _y);}

protected:
    double _x;
    double _y;
    static float _grid; // Grid interval
    Cairo::Matrix _view_mat;

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

struct BoundingBox
{
    Coordinate anchor; // Bottom left corner
    double width,height;
    bool contains(Coordinate pos)
    {
        // Check if pos is within the bounding box
        if(pos.x() > anchor.x() &&
           pos.x() < anchor.x() + width &&
           pos.y() > anchor.y() &&
           pos.y() < anchor.y() + height)
        {
           return true;
        }
        return false;
    }
};


#endif /* COORDINATE_H */