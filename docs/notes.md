# My Notes
It's a file for leaving some notes about this mini project.

## Game rules

1. Underpopulation: A live cell with fewer than two live neighbors dies
2. Stasis: A live cell with two or three live neighbors lives on to the next generation
3. Overpopulation: A live cell with more than three live neighbors dies
4. Reproduction: A dead cell with exactly three live neighbors becomes a live cell.


## Efficiency

There is an easy way to implement game rules, but I don't really like the idea of reading full matrix (Field) and checking each cell's state. In a NxN grid it will take N^2 sets of operations each time we want to redraw the Field + N^2 draw actions. 

It's a bit too much. 

Instead, we can try to only keep track of alive cells. In the worst scenario we will have N^2 operations, but actually we won't even reach that number due to the Game of Life rules. 

So, how can we do that? 

### Understanding 2 stages of Update

For example, we have a current Generation of cells. To move to the next Generation we need to:

1. Apply rules to the current generation and note future changes
2. Apply changes

If we apply changes on the go, we will be modifying the current generation and changing the current state of the game, thus we will mess with the results for the next generation. 

### Tracking only alive cells

I believe there's no need to look through each dead cell. It will be enough to take into account only alive cells and neighboring dead cells. 

Each alive cell must undergo these 2 operations:

1. Send impulse
2. Check status [and clean it after]

"Send Impulse" operation affects neighbors, giving them +1 value (in other words, notifying them that their neighbor has an alive cell next to them)

After each alive cell has done it, each affected cell must check their status:
1. If the cell is dead and has received exactly +3 impulses, it becomes alive. -> REDRAW. Otherwise, it is still dead.
2. If the cell is alive and has received [+1 or less] or [+4 and more] impulses, it dies. -> REDRAW
3. If the cell is alive and has received +2 or +3 impulses, it continues to live. 

### Reimagining project algorithm

1. Class Field will be called Simulation.
2. Class Simulation:
   1. Has a variable world which is a matrix of all cells (uint16_t).
   2. Has a variable generation which is a vector of pairs<int, int> (coordinates of alive cells).
   3. Has a variable generationalChanges which is a hash set (unordered_set) of pairs<int, int> (coordinates of alive cells + affected dead cells). -> We need a hash set to avoid duplicates. 
   4. Has method update() that simulates one generation step of the Game of Life.

1. Method Update:
2. We fill generationalChanges with generation at the start of Update.
3. We go through generation:
    1. We get alive cells from generation.
    2. Each alive cell sends impulses to its neighbors (increments their value in a world by 1).
    3. Each affected cell is added to generationalChanges.
4. We empty generation.
5. We go through generationalChanges:
    1. We get affected cells from generationalChanges.
    2. We check if the cell passes the rules.
       1. According to the rules, we add cells to next generation ot leave them dead.
       2. We redraw cells if their state has changed.
       3. We delete impulses received by cells.
6. We empty generationalChanges.
7. End of Update. 