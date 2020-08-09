*****Final Exam Practical by Emily Horton - eh8582@rit.edu*****
Copied C22 Physics from the class repo as a starting point to this project

*****CONTROLS*****
WASD- Move camera forward/left/back/right
Q - Move camera up
E - Move camera down
+/n - Add an obstacle
-/M - Remove an obstacle

*****INSTRUCTIONS*****
This app will spawn a grid of blocks, a zombie, and a player (steve). The zombie will try to chase the player in the world. 
However, the world will have obstacles in it, so the zombie will try to go around the obstacles in the most-efficient path.

Use the arrow keys to move Steve around and watch the zombie chase him. You and the zombie may each push obstacles around, but the zombie 
will typically just go around them.

By default there should be 15 obstacles. You can spawn new obstacles using + or N (up to a maximum of 30) or remove obstacles by pressing 
- or M. 

Any grid object should show its rigid body in red if it is weighted by an obstacle.

As Steve moves, a new path will be calculated between the zombie and him. This path is shown visually with planes on top of the blocks.
The starting point (where the zombie began) is in blue, the end point (where Steve is) is in green, and the rest of the path will be yellow.

*****CODING DOCS*****
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

Zombie
-----------
This class inherits MyEntity and contains a path for the zombie to follow as well as an int keeping track of the next index of the path
it should move towards.
The member function MoveZombie will LERP the zombie towards the position at the next index in the path. It will not move if the index
is out of range.

MyEntityManager
-----------
I added one extra function called AddExistingEntity so that I could create a zombie and add it to the entity manager.

AppClass
-----------
I added some member variables for tracking AStar entities such as the grid, grid size, and a pointer to the zombie.
I also added the methods HandleAStar in Update, AddObstacle and Remove Obstacle to create more/remove obstacles

*****KNOWN ISSUES*****
-While Steve is moving, the Zombie can sometimes move around to different starting points due to the starting point for A* being the first 
point in the path the zombie must follow. It looks a little weird but doesn't break anything.
-The path finding algorithm will occasionally a sub-optimal path if Steve moves onto a block that is weighted by an obstacle.
-Steve and the Zombie can sometimes get a little stuck together because the zombie keeps trying to get onto Steve. It's 
easy enough to break free.
-The rendering of block rigidbodies to indicate they have an obstacle on them only updates when a new AStar path is calculated. Move steve 
onto another block to update them.