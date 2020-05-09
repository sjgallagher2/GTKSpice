# Organization

To handle the schematic space (where we have elements, wires, and ports) which can be drawn (with symbols including pins) we have to use a few levels of abstraction. Particularly important are the wires and nodes, which serve as logical and graphical connections between components, respectively. 

Nodes and the node manager are described in gtkspice_object.h. A node stores:

* Its name
* Its element connections - a (multi)map of pointers to elements which are associated with this node
* The wires - a vector of pointers to wires associated with this node
* Its priority - priorities are used to handle node merges

