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