# Snake-Bot
The project is a snake game programmed in C, offering the option to play manually or observe AI utilizing the A* algorithm with or without customized adjustments.

## Overview

The project consists of several files: 
- **main.h:**  Header file containing constants, structs, and function declarations used in `main.c`. 
- **main.c:**  Source file containing the logic of the snake, including initialization, drawing, input handling, and game logic. 
- **humanInput.h:**  Header file containing function declarations for human input control. 
- **humanInput.c:**  Source file containing the implementation of human input control. 
- **snakeBot.h:**  Header file containing function declarations for the AI-controlled snake. 
- **snakeBot.c:**  Source file containing the implementation of the AI-controlled snake using the A* search algorithm. 
- **snakeBotv2.c:**  Source file containing an improved version of the AI-controlled snake using the A* search algortihm + logic to avoid snake to be stuck.
## Usage 
1. Clone the repository: `git clone https://github.com/Mathis-L/Snake-Bot.git` 
2. Compile the project using a C compiler, such as gcc: `gcc main.c humanInput.c snakeBotv2.c -o snake_game` 
3. Run the executable: `./snake_game`
4. Follow the on-screen instructions to choose between human or AI control.
5. Control the snake using the arrow keys `Z` `Q` `S` `D`(for human control (french keyboard)) or observe the AI-controlled snake.
6. Try to eat fruits and avoid hitting walls or yourself.
## Change AI algo
This project come with two AI version (feel free to add your own), to modify them modify line 37 to put your AI.    

My best version so far is the `snakeBotv2.c`, to compile this version write line 37 in main.c: `player_dir = giveDirection(jeu);` and compile with this command line: `gcc main.c humanInput.c snakeBotv2.c -o snake_game`

To use and see only the A* algorithm change line 37 : `player_dir = performAStarSearch(jeu);` and compile with this command line: `gcc main.c humanInput.c snakeBot.c -o snake_game`


## Executables 
- `snakeBot.exe`: Executable for Windows compiled with AI control using the A* search algorithm. 
- `snakeBotv2.exe`: Executable for Windows compiled with an improved version of AI control.---

Feel free to customize this README with additional information about your project or instructions on how to compile and run it on different platforms.

## A* algorithm
The A* algorithm, pronounced 'A-star', is a pathfinding algorithm widely applied in domains such as video games, robotics, and motion planning. Its primary aim is to determine the shortest path between a starting node and a target node within a weighted graph, factoring in the cost of movement between nodes and an estimation of the remaining distance to the destination.

Here's how A* operates:

Initialization: The algorithm begins with a starting node and a target node. It maintains two lists: one for nodes to explore, initially empty, and another for already explored nodes, also initially empty.

Node Evaluation: At each iteration, the algorithm selects the node for exploration based on a combined cost value, which is the sum of the cost of moving from the starting node to the current node (actual cost) and an estimation of the remaining cost to the destination (heuristic cost). Typically, this heuristic estimation utilizes the Manhattan distance, measuring horizontal and vertical distance without considering diagonal movement.

Exploring Neighbor Nodes: Once a node is selected, the algorithm examines neighboring nodes accessible from this node. For each neighboring node, it calculates the total path cost by adding the cost of movement from the current node and the heuristic estimation of the remaining path cost. If this total cost is lower than the current cost assigned to the node or if the node has not yet been explored, the algorithm updates or adds this node to the list of nodes to explore.

Repetition: The algorithm repeats these steps until the target node is reached or until the list of nodes to explore is empty, indicating that there is no path between the starting node and the target node.

In the context of snakeBot implementation, A* calculates the optimal path for the snake to reach its next target, i.e. food.

## SnakeBotv2 
In summary, snakeBotv2 incorporates the A* algorithm as a baseline for pathfinding and adds significant enhancements to evaluate space and avoid self-trapping. This makes the AI more robust and capable of handling complex scenarios in the game.
1. **Enhancements**:
The `giveDirection` function first calls `performAStarSearch` to find the shortest path to the fruit. The resulting direction is stored in `AstarResult`. This result is used as a baseline but is not always followed strictly. Instead, the AI evaluates potential moves based on available space and the risk of self-blockage. 
4. **areaSize** : 
- After determining the direction from A*, the AI evaluates the available space in each potential move direction (north, south, east, west) using the `areaSize` function.
- This function performs a flood fill (breadth-first search) to calculate the area size starting from a given coordinate, ensuring the snake does not trap itself in a small enclosed space. 
3. **Choosing the Best Direction** : 
- It gives a slight preference to the direction suggested by A* by incrementing its area size value.
- The direction with the maximum area size is then chosen.
4. **Avoiding One-Space Gaps Near Walls** :
If moving in a direction (north, south, east, west) results in a position next to the wall (i.e., a space adjacent to the wall with no escape) and there is no fruit in the corresponding row or column, the AI penalizes this direction by decrementing its area size value.

## Results
I tested the two versions of the AI algorithm on a 20x20 grid (18x18 for usable cells). The following results show the number of fruits consumed by the SnakeBot for each version over 10 different games. the goal was to code a bot that can obtain 100 points on the 18*18 = 324 available (if we substract the snake head it's 323 points maximum).
### SnakeBot with A*:
Number of fruits consumed: [52, 63, 66, 50, 45, 59, 52, 37, 63, 75]   
Average fruits consumed: 56.2 
### SnakeBotv2:
Number of fruits consumed: [82, 86, 76, 87, 105, 110, 127, 92, 92, 116]  
Average fruits consumed: 97.3   
The best result of SnakeBotv2 is 127 fruits consumed, which corresponds to filling 127 / 324 ≈ 39.3% of the usable grid (18x18).
### Results Analysis 
These results show that SnakeBotv2, with its enhancements to avoid traps and evaluate available space, is significantly more efficient than the basic A* algorithm in terms of the number of fruits consumed. SnakeBotv2 consumes more fruits on average, indicating a better ability to navigate and survive in the game environment.
