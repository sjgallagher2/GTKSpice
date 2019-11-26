%class-name LTSpiceSymbolParser
%filenames ltspice_symbol_parser
%parsefun-source ltspice_symbol_parser.cpp
%baseclass-preinclude   <string>
%scanner ltspice_symbol_lexer.h
%scanner-class-name LTSpiceSymbolLexer
%left ' '
//%debug

%union
{
    int u_int;
    std::string* u_str;
}

// Basic types
%token <u_str>  STRING
%token <u_int>  INT

// Header tokens
%token <u_str>  VERSION
%token <u_str>  SYMBOLTYPE
//%token <u_str>  BLOCK
//%token <u_str>  CELL

// Geometries
%token <u_str>  LINE
%token <u_str>  RECTANGLE
%token <u_str>  CIRCLE
%token <u_str>  ARC
%token <u_str>  TEXT
//%token <u_str>  NORMAL

// Justifications
//%token <u_str>  LEFT
//%token <u_str>  RIGHT
//%token <u_str>  CENTER
//%token <u_str>  TOP
//%token <u_str>  BOTTOM
//%token <u_str>  VLEFT
//%token <u_str>  VRIGHT
//%token <u_str>  VCENTER
//%token <u_str>  VTOP
//%token <u_str>  VBOTTOM
//%token <u_str>  NONE

// Attributes and pins
%token <u_str>  WINDOW
%token <u_str>  SYMATTR
%token <u_str>  PREFIX
%token <u_str>  SPICEMODEL
%token <u_str>  VALUE
%token <u_str>  VALUE2
%token <u_str>  SPICELINE
%token <u_str>  SPICELINE2
%token <u_str>  DESCRIPTION
%token <u_str>  MODELFILE

%token <u_str>  PIN 
%token <u_str>  PINATTR
%token <u_str>  PINNAME
%token <u_str>  SPICEORDER

// Semantic type definitions
%type <u_str>   ltspicesymbolfile

%type <u_str>   header
%type <u_str>   versionline
%type <u_str>   symboltypeline

%type <u_str>   body
%type <u_str>   lineline
%type <u_str>   rectangleline
%type <u_str>   circleline
%type <u_str>   arcline
%type <u_str>   textline
%type <u_str>   windowline
%type <u_str>   symattrline
%type <u_str>   symattrprefixstring
%type <u_str>   symattrmodelstring
%type <u_str>   symattrvaluestring
%type <u_str>   symattrvalue2string
%type <u_str>   symattrspicelinestring
%type <u_str>   symattrspiceline2string
%type <u_str>   symattrdescriptionstring
%type <u_str>   symattrmodelfilestring
%type <u_str>   pinlines // Collect pins and attributes
%type <u_str>   pinline // Collect pin with its attributes
%type <u_str>   pinattrline // Collect pin attributes
%type <u_int>   pinattrspiceorderstring 
%type <u_str>   pinattrpinnamestring 

%type <u_str>   string
%type <u_int>   integer

%%


ltspicesymbolfile:
    // empty
|
    ltspicesymbolfile '\n'
|
    ltspicesymbolfile header body
    {
        $$ = nullptr;
        delete $2;
    }
;

// HEADER LINES
header:
    header '\n'
|
    versionline symboltypeline
    {
        $$ = new std::string( *($1) + *($2) );
        delete $1;
        delete $2;
    }
;
versionline:
    VERSION ' ' integer '\n'
    {
        $$ = new std::string("Version "+std::to_string($3)+"\n");
    }
;
symboltypeline:
    SYMBOLTYPE ' ' string '\n'
    {
        $$ = new std::string("SymbolType "+*($3)+"\n");
        delete $3;
    }
;

// BODY LINES
body:
    body body
    {
        $$ = nullptr;
    }
|
    lineline
|
    rectangleline
|
    circleline
|
    arcline
|
    textline
|
    windowline
|
    symattrline
|
    pinlines
;

lineline:
    LINE ' ' string ' ' integer ' ' integer ' ' integer ' ' integer '\n'
    {
        $$ = nullptr;
        std::shared_ptr<LinePrimitive> line = std::make_shared<LinePrimitive>();
        line->start(Coordinate(($5)/LTSPICE_SCALE_FACTOR,($7)/LTSPICE_SCALE_FACTOR));
        line->end(Coordinate(($9)/LTSPICE_SCALE_FACTOR,($11)/LTSPICE_SCALE_FACTOR));
        ogeom_.push_back(line);
    }
|
    LINE ' ' string ' ' integer ' ' integer ' ' integer ' ' integer ' ' integer '\n'
    {
        $$ = nullptr;
        // NOTE: Ignoring line styles, because why have those in a symbol
        std::shared_ptr<LinePrimitive> line = std::make_shared<LinePrimitive>();
        line->start(Coordinate(($5)/LTSPICE_SCALE_FACTOR,($7)/LTSPICE_SCALE_FACTOR));
        line->end(Coordinate(($9)/LTSPICE_SCALE_FACTOR,($11)/LTSPICE_SCALE_FACTOR));
        ogeom_.push_back(line);
    }
;
rectangleline:
    RECTANGLE ' ' string ' ' integer ' ' integer ' ' integer ' ' integer '\n'
    {
        $$ = nullptr;
        std::shared_ptr<RectPrimitive> rect = std::make_shared<RectPrimitive>();
        rect->anchor(Coordinate(($5)/LTSPICE_SCALE_FACTOR, ($7)/LTSPICE_SCALE_FACTOR));
        rect->height(($11-$7)/LTSPICE_SCALE_FACTOR);
        rect->width(($9-$5)/LTSPICE_SCALE_FACTOR);
        ogeom_.push_back(rect);
    }
;
circleline:
    CIRCLE ' ' string ' ' integer ' ' integer ' ' integer ' ' integer '\n'
    {
        $$ = nullptr;
        std::shared_ptr<CirclePrimitive> circ = std::make_shared<CirclePrimitive>();
        circ->hradius( ($9 - $5)/2.0 / LTSPICE_SCALE_FACTOR);
        circ->vradius(($11-$7)/2.0 / LTSPICE_SCALE_FACTOR);
        circ->center( Coordinate( ($5)/LTSPICE_SCALE_FACTOR + circ->hradius(), ($7)/LTSPICE_SCALE_FACTOR + circ->vradius() ));
        ogeom_.push_back(circ);
    }
|
    CIRCLE ' ' string ' ' integer ' ' integer ' ' integer ' ' integer ' ' integer '\n'
    {
        $$ = nullptr;
        std::shared_ptr<CirclePrimitive> circ = std::make_shared<CirclePrimitive>();
        circ->hradius( ($9 - $5)/2.0 / LTSPICE_SCALE_FACTOR );
        circ->vradius(($11-$7)/2.0 / LTSPICE_SCALE_FACTOR);
        circ->center( Coordinate( ($5)/LTSPICE_SCALE_FACTOR + circ->hradius(), ($7)/LTSPICE_SCALE_FACTOR + circ->vradius() ));
        ogeom_.push_back(circ);
    }
;
arcline:
    ARC ' ' string ' ' integer ' ' integer ' ' integer ' ' integer ' ' integer ' ' integer ' ' integer ' ' integer '\n'
    {
        $$ = nullptr;

        // Get source circle
        std::shared_ptr<ArcPrimitive> arc = std::make_shared<ArcPrimitive>();
        arc->hradius( ($9-$5) / 2.0 / LTSPICE_SCALE_FACTOR );
        arc->vradius(($11-$7) / 2.0 / LTSPICE_SCALE_FACTOR);
        arc->center( Coordinate( ($5) / LTSPICE_SCALE_FACTOR + arc->hradius(), ($7) / LTSPICE_SCALE_FACTOR + arc->vradius() ));

        // Now get angles
        // We have a point for each, use atan2() to get angle relative to circle center
        Coordinate p1_abs(($13)/LTSPICE_SCALE_FACTOR - arc->center().x(), ($15)/LTSPICE_SCALE_FACTOR - arc->center().y());
        Coordinate p2_abs(($17)/LTSPICE_SCALE_FACTOR - arc->center().x(), ($19)/LTSPICE_SCALE_FACTOR - arc->center().y());
        float angle1 = std::atan2(p1_abs.y(),p1_abs.x()) / M_PI*180.0;
        float angle2 = std::atan2(p2_abs.y(),p2_abs.x()) / M_PI*180.0;
        
        arc->set_start_angle_degrees(angle2); // Note we swap the angles
        arc->set_end_angle_degrees(angle1);

        ogeom_.push_back(arc);
    }
|
    ARC ' ' string integer ' ' integer ' ' integer ' ' integer ' ' integer ' ' integer ' ' integer ' ' integer ' ' integer '\n'
    {
        $$ = nullptr;
    }
;
textline:
    TEXT ' ' integer ' ' integer ' ' string ' ' integer ' ' string '\n'
    {
        $$ = nullptr;
    }
;
windowline:
    WINDOW ' ' integer ' ' integer ' ' integer ' ' string  ' ' integer '\n'
    {
        $$ = nullptr;
        switch($3)
        {
        case 0:
            // Instname
            osymbol_.set_attribute_show_on_schematic("INSTNAME",true);
            osymbol_.set_attribute_text_position("INSTNAME",Coordinate($5 / LTSPICE_SCALE_FACTOR,$7 / LTSPICE_SCALE_FACTOR));
            break;
        case 1:
            // Type
            break;
        case 3:
            // Value 
            osymbol_.set_attribute_show_on_schematic("VALUE",true);
            osymbol_.set_attribute_text_position("VALUE",Coordinate($5 / LTSPICE_SCALE_FACTOR,$7 / LTSPICE_SCALE_FACTOR));
            break;
        case 38:
            // SpiceModel
            osymbol_.set_attribute_show_on_schematic("SPICEMODEL",true);
            osymbol_.set_attribute_text_position("SPICEMODEL",Coordinate($5 / LTSPICE_SCALE_FACTOR,$7 / LTSPICE_SCALE_FACTOR));
            break;
        case 39:
            // SpiceLine
            osymbol_.set_attribute_show_on_schematic("SPICELINE",true);
            osymbol_.set_attribute_text_position("SPICELINE",Coordinate($5 / LTSPICE_SCALE_FACTOR,$7 / LTSPICE_SCALE_FACTOR));
            break;
        case 40:
            // SpiceLine2
            break;
        case 123:
            // Value2
            break;
        }
    }
;
symattrline:
    symattrprefixstring '\n'
    {
        $$ = nullptr;
        osymbol_.set_attribute_value("PREFIX",*($1));
        delete $1;
    }
|
    symattrmodelstring '\n'
    {
        $$ = nullptr;
        osymbol_.set_attribute_value("SPICEMODEL",*($1));
        delete $1;
    }
|
    symattrvaluestring '\n'
    {
        $$ = nullptr;
        osymbol_.set_attribute_value("VALUE",*($1));
        delete $1;
    }
|
    symattrvalue2string '\n'
    {
        $$ = nullptr;
        delete $1;
    }
|
    symattrspicelinestring '\n'
    {
        $$ = nullptr;
        osymbol_.set_attribute_value("SPICELINE",*($1));
        delete $1;
    }
|
    symattrspiceline2string '\n'
    {
        $$ = nullptr;
        delete $1;
    }
|
    symattrdescriptionstring '\n'
    {
        $$ = nullptr;
        osymbol_.set_attribute_value("DESCRIPTION",*($1));
        delete $1;
    }
|
    symattrmodelfilestring '\n'
    {
        $$ = nullptr;
        osymbol_.set_attribute_value("MODELFILE",*($1));
        delete $1;
    }
;
symattrprefixstring:
    symattrprefixstring ' '
|
    SYMATTR ' ' PREFIX ' ' string
    {
        $$ = $5;
    }
;
symattrmodelstring:
    symattrmodelstring ' '
|
    SYMATTR ' ' SPICEMODEL ' ' string
    {
        $$ = $5;
    }
;
symattrvaluestring:
    symattrvaluestring string
    {
        $$ = new std::string(*($1) + *($2));
        delete $1;
    }
|
    symattrvaluestring integer
    {
        $$ = new std::string(*($1) + std::to_string($2));
        delete $1;
    }
|
    symattrvaluestring ' '
    {
        $$ = new std::string(*($1) + " ");
        delete $1;
    }
|
    SYMATTR ' ' VALUE ' ' string
    {
        $$ = $5;
    }
|
    SYMATTR ' ' VALUE ' ' integer
    {
        $$ = new std::string(std::to_string($5));
    }
;
symattrvalue2string:
    symattrvalue2string string
    {
        $$ = new std::string(*($1) + *($2));
        delete $1;
    }
|
    symattrvalue2string integer
    {
        $$ = new std::string(*($1) + std::to_string($2));
        delete $1;
    }
|
    symattrvalue2string ' '
    {
        $$ = new std::string(*($1) + " ");
        delete $1;
    }
|
    SYMATTR ' ' VALUE2 ' ' string
    {
        $$ = $5;
    }
|
    SYMATTR ' ' VALUE2 ' ' integer
    {
        $$ = new std::string(std::to_string($5));
    }
;
symattrspicelinestring:
    symattrspicelinestring string
    {
        $$ = new std::string(*($1) + *($2));
        delete $1;
    }
|
    symattrspicelinestring integer
    {
        $$ = new std::string(*($1) + std::to_string($2));
        delete $1;
    }
|
    symattrspicelinestring ' '
    {
        $$ = new std::string(*($1) + " ");
        delete $1;
    }
|
    SYMATTR ' ' SPICELINE ' ' string
    {
        $$ = $5;
    }
|
    SYMATTR ' ' SPICELINE ' ' integer
    {
        $$ = new std::string(std::to_string($5));
    }
;
symattrspiceline2string:
    symattrspiceline2string string
    {
        $$ = new std::string(*($1) + *($2));
        delete $1;
    }
|
    symattrspiceline2string integer
    {
        $$ = new std::string(*($1) + std::to_string($2));
        delete $1;
    }
|
    symattrspiceline2string ' '
    {
        $$ = new std::string(*($1) + " ");
        delete $1;
    }
|
    SYMATTR ' ' SPICELINE2 ' ' string
    {
        $$ = $5;
    }
;
symattrdescriptionstring:
    symattrdescriptionstring string
    {
        $$ = new std::string(*($1) + *($2));
        delete $1;
    }
|
    symattrdescriptionstring integer
    {
        $$ = new std::string(*($1) + std::to_string($2));
        delete $1;
    }
|
    symattrdescriptionstring ' '
    {
        $$ = new std::string(*($1) + " ");
        delete $1;
    }
|
    SYMATTR ' ' DESCRIPTION ' ' string
    {
        $$ = $5;
    }
;
symattrmodelfilestring:
    symattrmodelfilestring string
    {
        $$ = new std::string(*($1) + *($2));
        delete $1;
    }
|
    symattrmodelfilestring integer
    {
        $$ = new std::string(*($1) + std::to_string($2));
        delete $1;
    }
|
    SYMATTR ' ' MODELFILE ' ' string
    {
        $$ = $5;
    }
;

pinlines:
    pinlines pinattrline
    {
        $$ = nullptr;
    }
|
    pinline
    {
        $$ = nullptr;
    }
;

pinline:
    PIN ' ' integer ' ' integer ' ' string ' ' integer '\n'
    {
        $$ = nullptr;
        std::shared_ptr<SymbolPin> pin1 = std::make_shared<SymbolPin>();
        pin1->pin_location(Coordinate(($3)/LTSPICE_SCALE_FACTOR,($5)/LTSPICE_SCALE_FACTOR));
        opins_.push_back(pin1);
    }
;

pinattrline:
    pinattrpinnamestring '\n'
    {
        $$ = nullptr;
        if(!opins_.empty())
            (*(opins_.end()-1))->set_attribute_value("NAME",*($1));
        else
            std::cout << "WARNING: Pin attribute found but no pins added.\n";
        delete $1;
    }
|
    pinattrspiceorderstring '\n'
    {
        $$ = nullptr;
        if(!opins_.empty())
            (*(opins_.end()-1))->set_attribute_value("SPICE_ORDER",std::to_string($1));
        else
            std::cout << "WARNING: Pin attribute found but no pins added.\n";
    }
;

pinattrpinnamestring:
    pinattrpinnamestring ' '
|
    PINATTR ' ' PINNAME ' ' integer
    {
        $$ = new std::string(std::to_string($5));
    }
|
    PINATTR ' ' PINNAME ' ' string
    {
        $$ = $5;
    }
;

pinattrspiceorderstring:
    pinattrspiceorderstring ' '
|
    PINATTR ' ' SPICEORDER ' ' integer
    {
        $$ = $5;
    }
;


// BASIC TOKENS
string:
    STRING
    {
        $$ = d_scanner.strVal();
    }
|
    '-'
    {
        $$ = d_scanner.strVal();
    }
;
integer:
    INT
    {
        $$ = d_scanner.intVal();
    }
;

