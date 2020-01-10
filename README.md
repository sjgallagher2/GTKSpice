# GTKSPICE
## Sam Gallagher 2017-2020

This is a proper implementation of SPICE with the GTK toolkit.

It runs on gtkmm and uses ngspice as a backend (not yet implemented). I'm constantly learning new things and use
this project to test design patterns etc so progress can be twisty. But hopefully one day we can have an
open-source alternative to LTSpice and others.

Current Functionality:
* Reasonable glue code
* Functional coordinate system and drawing space
* Undo/redo available
* Schematic objects with schematic sheets made of elements, wires, and ports
* Parsing of LTSpice-formatted symbol files
* Automatic connection of elements, wires, ports
* Node management, generation of SPICE netlist

TODO (There's lots)
* Handling of floating objects (being dropped onto the schematic)
* Handling of active objects (selected and being manipulated by a tool)
* Re-implementation of all tools
* Re-implementation of relevant actions for undo/redo
* Developing custom schematic and symbol file formats as needed
* Addition of GUI elements (menus, toolbar)
* Addition of symbol editing envirnoment
* Addition of data plotting and probing


