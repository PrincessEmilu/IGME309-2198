Controls:
Pageup: Display Next Octant
PageDown: Display Previous Octant
Space: Display All Octants
Movement is the same as Simplex

**Known Issues**
Frame Rate Still Low
Everything in this assignment should be functioning properly. However, on my machine the FPS is still very low.
To be sure, I am getting noticeable and consistent frame rate increases, but they are not very big increases.

ON my machine, I observed
1 Octant (no spatial optimization): 11 FPS
2 Octants : 19 FPS
3 Octants : 22 FPS
4 Octants : 17 FPS

I am not sure exactly what is causing this issue. The first thing I checked was that only the leaf nodes are storing 
entities, and that seems to be true. Maybe it is because I am storing my children octants in a vector instead of an array, but
I am not sure.