# Optimal Pathfinding with Dynamic Weights and Red-Black Tree Caching

## Table of Contents

- [Project Overview](#project-overview)
- [Components](#components)
- [Algorithms and Data Structures Explanation](#algorithms-and-data-structures-explanation)
- [Usage](#usage)
- [Flow Chart](#flow-chart)

---

## Project Overview

The project involves finding an efficient path from a single source to
multiple destinations in a strongly connected, directed graph with positive weights,
relaxing constraints compared to the Traveling Salesman Problem (TSP) by allow-
ing nodes to be visited multiple times and focusing on minimizing computational
operations rather than achieving a strictly optimal path. The solution combines
TSP principles with the A* algorithm, leveraging heuristics to prioritize promis-
ing paths and dynamically updating edge weights as nodes are visited. Red-Black
Trees are utilized for efficient data management, facilitating quick updates and
retrieval of edge weights to optimize performance. Further enhancements may in-
clude reducing the search space with bidirectional search or clustering closely con-
nected nodes. This practical and flexible approach effectively balances efficiency
and near-optimality in single-source, multi-destination pathfinding while maintain-
ing an organized structure that supports rapid access and modifications to graph
data.

## Components

1. **A*-Based TSP Solver**:
   - An A*-based solution for the Traveling Salesman Problem (TSP), adapted for dynamic updates and optimized shortest paths.
   - Heuristic-based shortest path finder between nodes, with Euclidean distance as the primary heuristic.
   - Used in a modified TSP algorithm to find the nearest neighbor at each iteration for efficient route selection.

2. **Red-Black Tree (RBT)**:
   - O(log n) time complexity for insertion, search, and deletion.
   - Most of the BST operations (e.g., search, max, min, insert, delete.. etc) take O(h) time where h is the height of the BST.
   - The height of a Red-Black tree is always O(log n) where n is the number of nodes in the tree.

3. **Dynamic Weight Updates**:
   - Randomizes weights after reaching each destination to simulate real-time traffic changes.
   - Includes utilities to update weight files and reload graphs to reflect current conditions.
   - Weight updates in real-time to simulate traffic conditions, providing more accurate pathfinding as conditions evolve.

## Algorithms and Data Structures Explanation:

### 1. A-star (A\*) algorithm: 
a well-known form of best-first search that minimizes the total estimated solution cost. The A\* algorithm evaluates nodes by combining the cost to reach the node, `g(n)`, and the estimated cost to get from the node to the destination, `h(n)`:

**f(n) = g(n) + h(n)**

- **g(n)**: The cost to reach the node
- **h(n)**: The cost to get from the node to the destination
A\* search is a strategy where we first try the node with the lowest value of `g(n) + h(n)`. This approach ensures that A\* search is both complete and optimal, provided that the heuristic function `h(n)` satisfies certain conditions.

#### Conditions for Optimal and Consistent Heuristic:

For optimality, the heuristic `h(n)` must be admissible. An admissible heuristic never overestimates the cost to reach the goal. Since `g(n)` is the actual cost to reach `n` along the current path, and `f(n) = g(n) + h(n)`, it follows that `f(n)` never overestimates the cost of a solution along the current path through `n`.

_Admissible heuristics_ are optimistic because they assume the cost of solving the problem is less than it actually is. An example of an admissible heuristic is the straight-line distance `hSLD` used for getting to Bucharest. The straight-line distance is admissible because the shortest path between any two points is a straight line, which cannot be an overestimate.

![A* Tree Search Progress](https://github.com/user-attachments/assets/99b7a888-2fd4-4917-a54c-9c4c8c011e12)

The above figure shows the progress of an A\* tree search for Bucharest.

#### Other important Heuristics:
**Manhattan Distance Heuristic**: Commonly used in grid-based pathfinding, such as in games or robotics. It calculates the sum of the horizontal and vertical distances from the current node to the goal node, assuming only orthogonal movements are allowed.

**Euclidean Distance Heuristic**: Used in situations where diagonal movements are allowed. It calculates the straight-line (Euclidean) distance between the current node and the goal node.

**Diagonal Distance Heuristic**: Used in grid-based environments where diagonal moves are allowed. It estimates the distance by considering both horizontal and vertical moves, as well as diagonal moves, using a combination of these distances.

**Octile Distance Heuristic**: A variation of the diagonal distance heuristic that assumes diagonal moves are more costly. It uses a weighted combination of horizontal, vertical, and diagonal distances, reflecting the higher cost of diagonal moves.

**Custom Heuristics**: In many cases, custom heuristics specific to the problem domain can be designed. These heuristics might take into account domain-specific knowledge and features to estimate the cost more accurately.

_For more details, refer to this part of the book_: [Artificial Intelligence: A Modern Approach by Russell and Norvig](https://people.engr.tamu.edu/guni/csce421/files/AI_Russell_Norvig.pdf#page=111)

## 2 .Red Black Tree Data Structure: 

also called as RB Trees which are highly used data structure since they efficiently performs search, insert and delete operation in O(log(n)).

### self balancing property
#### left rotate right rotate
- Left Rotation:
  
      x                  y                                              
       \                / \                               
        y       -->    x   b                                        
       / \              \                                         
      a   b              a                                         


- Right Rotation:    
   
          x              y
         /              / \
        y       -->    a   x
       / \                / 
      a   b              b

### easy search property
#### instert fix and delete fix 
### where we used rb tree in code
  
## Usage

### Requirements:
- **Compiler**: GCC or similar with support for C99 or above.
- **Libraries**: Standard libraries (`stdio.h`, `stdlib.h`, `math.h`, `limits.h`, `time.h`) included.

### Commands and detailed process:
1. **Clone the GitHub repository and navigate to the project directory:**
   ```
   git clone https://github.com/PolisettyTharunSai/project---optimal-route-managing-system.git
   cd project---optimal-route-managing-system
   ```
2. **Input file description**
- The repository itself contains an input file named `graph_data.csv`.
- For giving custom inputs follow these commands.
  ```
  vim graph_data.csv
  ```
  here you would see the original input file data, copy and paste in other file if required.
  - To add a directed *edge from node1 to node2* with a *weight of w*.  
  -  ```
     <node1>,<node2>,<w>
     ```
   ### Example:
     ```
      0,1,10
      0,2,7
      0,3,15
      0,4,6
      1,0,12
      1,2,3
      1,3,11
      1,4,8
      2,0,14
      2,1,1
      2,3,4
      2,4,13
      3,0,5
      3,1,17
      3,2,2
      3,4,10
      4,0,8
      4,1,16
      4,2,1
      4,3,9
     ```
3. **Compiling the Code**
   ```
   gcc -o main main.c -lm
   ```
2. **Run the code**
   ```
   ./main
   ``` 
## Flow chart: 
Our project is based on finding the nearest destination to the present node through using A_start algorithm, then repeating this process until all the destinations are visited. This below Flowchart showcases a simpler understanding of above mentioned process including the inputs and outputs.
   
![Flowchart (1)](https://github.com/user-attachments/assets/01801169-5646-41ae-b790-5e2abcd952cb)
