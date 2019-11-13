#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

/* Make an object model with a parent Object class, Primitive classes (Line, Rect, Circle, Text), and a CompositeObject
 * class (made up a Symbol and other CompositeObjects). Every Object should be uniquely indexed and should be 
 * accessible. Objects should be able to be "active" and should have a draw() function which accepts a Cairo::Context. 
 * All objects must have locations and bounding boxes, and given a coordinate we should be able to find different objects 
 * which are at or near that location.
 *
 * Every Object should have a Symbol class which stores drawing information specific to that Object (excluding generic
 * non-editable elements like highlights). Symbols should have drawing settings including stroke and fill settings. The
 * Symbol should be made up of Primitives, Pins, and SymbolAttributes. Pins should also have PinAttributes.
 * 
 * An Object factory should be used to create Objects based on ObjectParameters classes. Objects should also have 
 * non-editable and optionally visible elements such as vertex handlers. Objects should have optional Text which can print
 * the index, name, etc of the Object, and the Text should be moveable by itself, while also moving with the Object.
 * 
 * A Wire class should exist which allows addition to a line, and only orthogonal ('ortho') lines are allowed. The ortho 
 * mode should have two options, one for each of the two ortho paths that exist between two points. The Wire class should 
 * be able to connect nodes of Objects. Moving the wire should not break the connections. Moving the wire should also 
 * maintain orthogonality, and should not create awkward overhangs around corners. Vertices which are redundent should 
 * be eliminated automatically.
 * 
 * 
 */

struct Coordinate{double x,y;};
struct BoundingBox{double width,height;};


/********* SYMBOLS ***********/
class DrawingSettings 
{};
class StrokeSettings : public DrawingSettings 
{};
class FillSettings : public DrawingSettings
{};

/* Primitives are used to describe a component's Symbol */
enum PrimitiveType {PRIMITIVE_LINE,PRIMITIVE_RECT,PRIMITIVE_POLYGON,PRIMITIVE_CIRCLE,PRIMITIVE_TEXT};
class Primitive
{
public:
    PrimitiveType type;
};

class Line : public Primitive 
{
public:
    Line() {type=PRIMITIVE_LINE;}
};
class Rect : public Primitive 
{
public:
    Rect() {type=PRIMITIVE_RECT;}
};
class Circle : public Primitive 
{
public:
    Circle() {type=PRIMITIVE_CIRCLE;}
};
class Polygon : public Primitive 
{
public:
    Polygon() {type=PRIMITIVE_POLYGON;}
};
class Text : public Primitive 
{
public:
    Text() {type=PRIMITIVE_TEXT;}
};

class SymbolPinAttribute {};
class SymbolPin {
public:
    std::vector<SymbolPinAttribute> attrs;
};

class SymbolAttribute {};

class ObjectSymbol 
{
public:
    StrokeSettings stroke;
    FillSettings fill;
    bool visible;
    std::vector<std::shared_ptr<Primitive>> geometry;
    std::vector<SymbolPin> pins;
    std::vector<SymbolAttribute> attrs;
    
    bool on(Coordinate); // Return true if Coordinate is (almost) on top of visible drawing element (stroke,fill)
};

/*********** OBJECTS ***********/
class Object 
{
public:
    int index;
    bool active;
    void draw(Cairo::Context);
    Coordinate anchor;
    BoundingBox boundingbox;
    
    ObjectSymbol symbol;
    
    bool near(Coordinate); // Return true if Coordinate is in the BoundingBox
    bool on(Coordinate); // Return true if Coordinate is on the Symbol
};


class CompositeObject : public Object 
{};

class Wire : public Object 
{};

class ObjectFactory 
{};

int main()
{


    return 0;
}

