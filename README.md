# Optimal Pathfinding with Dynamic Weights and Red-Black Tree Caching

## Table of Contents

- [Project Overview](#project-overview)
- [Components](#components)
- [Algorithms and Data Structures Explanation](#algorithms-and-data-structures-explanation)
  - [1. A-star (A*) Algorithm](#1-a-star-a-algorithm)
  - [2. Red Black Tree Data Structure](#2-red-black-tree-data-structure)
- [Usage](#usage)
  - [Requirements](#requirements)
  - [Commands and detailed process](#commands-and-detailed-process)
- [Flow Chart](#flow-chart)

---

## Project Overview

The project focuses on efficiently finding paths from a single source to multiple destination nodes within a strongly connected, directed graph with positive edge weights, specifically tailored for an Optimal Route Managing System in real-time traffic scenarios. It employs the A* search algorithm enhanced by a Euclidean heuristic, effectively guiding the search process toward optimal route selection by estimating the distance to target nodes and minimizing overall path costs. A notable feature of this approach is the dynamic updating of edge weights, which reflects real-time changes in traffic conditions as nodes are visited. This adaptability ensures the algorithm accurately represents the current state of the graph, addressing fluctuations in edge weights due to varying traffic conditions or user-defined constraints.To manage these edge weight updates and delete unwanted edges based on user-defined routes to be avoided, Red-Black Trees are utilized. This enhances the flexibility of the graph's structure and improves performance during the pathfinding process, allowing for a responsive and efficient navigation experience in the context of real-time traffic management.

## Components

1. **A star-Based TSP Solver**:
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

The A algorithm* is a best-first search method used to find the shortest path to a target. It calculates a cost for each node in the path using two main components:

**g(n)**: This is the actual cost from the start node to the current node `𝑛`. It reflects the total distance or effort already expended to reach that point.

**h(n)**: This is the heuristic or estimated cost from the current node `𝑛` to the destination node. It’s a guess of the remaining distance, intended to guide the search in the right direction.

**f(n)**: This is the sum of the two costs, `f(n) = g(n) + h(n)` .It represents the total estimated cost of the path through node `𝑛`: the actual cost so far plus the estimated cost to the goal.

The A* search strategy always selects the node with the lowest `f(n)` value to expand next. This method is guaranteed to find the shortest path if the heuristic `h(n)` is admissible (never overestimates the cost to reach the goal) and consistent (the heuristic cost between nodes does not decrease along the path).

By balancing both actual and estimated costs, A* search ensures a complete and optimal solution, reaching the destination by the shortest possible route.

**Euclidean Distance Heuristic**: Used in situations where diagonal movements are allowed. It calculates the straight-line (Euclidean) distance between the current node and the goal node. 
```
 h = sqrt ( (current_cell.x – goal.x)2 + (current_cell.y – goal.y)2 )
```

#### Conditions for Optimal and Consistent Heuristic:

For optimality, the heuristic `h(n)` must be admissible. An admissible heuristic never overestimates the cost to reach the goal. Since `g(n)` is the actual cost to reach `n` along the current path, and `f(n) = g(n) + h(n)`, it follows that `f(n)` never overestimates the cost of a solution along the current path through `n`.

#### Other important Heuristics:
**Manhattan Distance Heuristic**: Commonly used in grid-based pathfinding, such as in games or robotics. It calculates the sum of the horizontal and vertical distances from the current node to the goal node, assuming only orthogonal movements are allowed.
```
h = abs (current_cell.x – goal.x) + abs (current_cell.y – goal.y)
```

**Diagonal Distance Heuristic**: Used in grid-based environments where diagonal moves are allowed. It estimates the distance by considering both horizontal and vertical moves, as well as diagonal moves, using a combination of these distances.
```
   dx = abs(current_cell.x – goal.x)
   dy = abs(current_cell.y – goal.y)
   
   h = D * (dx + dy) + (D2 - 2 * D) * min(dx, dy)
   where D is length of each node(usually = 1) and D2 is diagonal distance between each node (usually = sqrt(2) ). 
```

**Octile Distance Heuristic**: A variation of the diagonal distance heuristic that assumes diagonal moves are more costly. It uses a weighted combination of horizontal, vertical, and diagonal distances, reflecting the higher cost of diagonal moves.

**Custom Heuristics**: In many cases, custom heuristics specific to the problem domain can be designed. These heuristics might take into account domain-specific knowledge and features to estimate the cost more accurately.

_For more details, refer to this part of the book_: [Artificial Intelligence: A Modern Approach by Russell and Norvig](https://people.engr.tamu.edu/guni/csce421/files/AI_Russell_Norvig.pdf#page=111)

#### Why choose A* Search over Dijkstra's algorithm:
A* is generally preferred over Dijkstra’s algorithm when we need an efficient, goal-directed search. While Dijkstra’s explores all possible paths within a given distance, A* focuses on the most promising paths by combining actual path cost with a heuristic estimate to the goal. This allows A* to reach the target faster, especially in large graphs, provided a good heuristic is available.

<p align="center">
  <img src="https://github.com/user-attachments/assets/b93bb35b-1c43-4ba7-b13e-f3d72d418e70" alt="Screenshot from 2024-11-03 12-10-11" width="45%">
  <img src="https://github.com/user-attachments/assets/5472306c-22fd-4537-b05b-dd2a8caf01d4" alt="Screenshot from 2024-11-03 12-13-03" width="45%">

 _This above image depicts how effective A* search algorithm performs over dijkstra's when a source and destination is fixed_
</p>

### 2. Red Black Tree Data Structure: 
The Red-Black Tree is a balanced binary search tree (BST) that ensures O(log n) complexity for insertion, deletion, and search operations. Unlike regular BSTs, which can degrade to O(n) time in worst cases (like a skewed tree), a Red-Black Tree maintains a balanced structure, making it highly efficient for dynamic datasets.

#### Self Balancing property:
The self balancing property is solely responsible for a RB Tree to perform operations in O(log n) complexity including the worst cases.
 
 - Node Color: Each node is either red or black.
 - Root Property: The root of the tree is always black.
 - Red Property: Red nodes cannot have red children (no two consecutive red nodes on any path).
 - Black Property: Every path from a node to its descendant null nodes (leaves) has the same number of black nodes.
 - Leaf Property: All leaves (NIL nodes) are black.

To uphold these properties, Red-Black Trees utilize rotations and recoloring during inserts and deletes. After each insertion or deletion, the tree checks for property violations and applies either a left or right rotation, alongside recoloring, to restore balance.
 
#### RB Tree Usage in this Project:
When multiple destinations are input by the user, our project allows users to specify certain edges to avoid, enhancing user control over the path.

##### Purpose: 
To efficiently manage this functionality, we delete specific edges from the graph, as requested by the user.

##### Why RB Tree?  
We store the graph data from graph_data.csv in a Red-Black Tree to ensure that searches and deletions of user-specified “avoided edges” are handled in 
𝑂(log 𝑛) time, a significant improvement over the linear O(n) time complexity.

##### Structuring Graph as RB Tree:
- Edge Structure: Each edge has three attributes: `node1`, `node2`, and `weight`.
- RB Tree Node Design: Each `RB_Node` in the Red-Black Tree represents an edge and stores these attributes.
- To keep the Red-Black Tree ordered, each node needs a unique property for comparison. Here, we use `node1` as the main attribute for sorting. If two edges have the same `node1`, we then 
compare their `node2` values. This two-level comparison guarantees that each `RB_Node` is unique because the graph only allows one direct edge from `node1` to `node2`.

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
   ### Example (the file should look like this):
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
4. **Run the code**
   ```
   ./main
   ```
5. **Output description:**
   The Program promises to output the Total minimum path cost to cover all the destinations and the Optimal path through which the minimum cost is obtained.
Output also include warnings for any violations occured during the input.

Here is an example testcase for better understanding of input formatting and output expectation.

   ```
      Enter the number of edges to exclude: 0
      Enter the source city (0 to 1999): 3
      Enter the number of destinations: 1
      Enter the destination cities (0 to 1999):
      Destination 1: 1
      Path: 3 -> 2 -> 1
      Total minimal path cost to cover all destinations: 3
   ```

When we have executed the command once you would observe there will be a change of edge weights in `graph_data.csv` file. Which simulates the dynamic traffic updation by randomly assigning weights.
Please check the `graph_data.csv` again to check wheter the intended input is present or not.

   ```
      Enter the number of edges to exclude: 1
      Enter edge to exclude (node1,node2): 3,2
      Enter the source city (0 to 1999): 3
      Enter the number of destinations: 1
      Enter the destination cities (0 to 1999):
      Destination 1: 1
      Path: 3 -> 4 -> 2 -> 1
      Total minimal path cost to cover all destinations: 12
   ```
Again running for the original `graph_data.csv` this time excluding an edge from 3 to 2 which makes the shortest path changed from the first case. 

## Flow chart: 
Our project focuses on identifying the nearest destination from the current node by utilizing the A* search algorithm. This process is repeated iteratively until all destination nodes have been visited. The flowchart below provides a clearer and more representation of this approach, illustrating the inputs and outputs involved in the overall process.
   
![Flowchart (1)](https://github.com/user-attachments/assets/01801169-5646-41ae-b790-5e2abcd952cb)
