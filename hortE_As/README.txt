Final Exam Practical by Emily Horton - eh8582@rit.edu
Copied C22 Physics from the class repo as a starting point to this project

CONTROLS

CODING DOCS
-----------
Here I would like to quickly explain the classes I created for this project and give a brief rundown of how they work.
Every function and member variable in each class is commented to explain what it does.

BlockGrid
-----------
This class contains a vector of block objects that represent the spaces in a world that characters (Steve and the Zombie) will move on.
This class contains a vector keeping track off all block objects. It is responsible for calculating A* and making sure that state changes
are passed to all containing blocks.

Block
-----------
This class inherits MyEntity. It contains all of the necesary information to calculate A*. It keeps track of its weight, distance from 
start, heuristic costs, and other variables. Blocks that are a part of a calculated A* path will have a plane rendered on top to indicated 
visually that they are part of the path, and the colors represent what part of the path (Blue for start, Yellow for path, Green for finish)

KNOWN ISSUES
-The path finding algorithm will occasionally a sub-optimal path if Steve moves onto a block that is weighted by an obstacle.