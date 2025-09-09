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

## Problems

Having written a draft version of the code, implementing the logic above, I have encountered some problems:

1. **Performance**: Simulation works slowly. Hypothesis:
   1. Raylib and redrawing issue. (not a problem)
   2. Overhead of using unordered_set. (is the problem)
   3. Hash function for pair<int, int> is not optimal. (was not significant)
   4. Division, mod (not significant, though played its part) 
2. **Structure and logic**:
   1. Code doesn't feel clean. I'd like to rewrite it using Cell class and probably completely abandoning the usage of matrix for storing all cells. That's sparse encoding method of implementation. 

## What I learned?

1. `unordered_set` and `unordered_map` use hash function. There are built-in hash functions, but only for basic data types. For custom ones and even such ones as `pair<int, int>` developer needs to create a custom hash function. It is important to create such function that reduces the number of duplicates in order to properly use capabilities of such data structures. 

```c++
// 0x - HEXADECIMAL
// ULL - Unsigned Long Long
// U - Unsigned
#define HASH64CONST 0x9e3779b97f4a7c15ULL
#define HASH32CONST 0x9e3779b9U

// Custom hash for unordered_set
namespace std {
      template <>
      struct hash<pair<int, int>> {
          size_t operator()(const pair<int, int>& key) const {
              size_t h1 = hash<int>()(key.first);
              size_t h2 = hash<int>()(key.second);
  
              size_t resultingHash =  h1^( h2 + HASH64CONST + (h1 << 6) + (h1 >>2));
              /
              return resultingHash;
          }  
      };
  }
```

2. Using Textures instead of raw drawing is faster and preffered when working with lots of elements. 

3. Using mod (%) is costly and it's better to avoid it or to speed it up (custom function). Using ifs is better.

4. The main caveat was the usage of `unordered_set`. It was an overhead (comsumption of computing resources that are required to perform a task, but are not directly contributing to the core functionality or desired outcome of that task. Extra work). Probably, due to:
   1. Usage of hash function for each insertion.
   2. Ineffective allocation of memory with dozens of thousands of insertions.

> Instead I used `vector<pair<int, int>> generation`, which significantly increased program. Now I can easily generate grid of 800x1600 size and insert around 320'000 alive cells there at the beginning (whereas the old program couldn't handle grid 400x400 with 20'000 alive cells).   

