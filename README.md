# Optimal Pathfinding with Dynamic Weights and Red-Black Tree Caching

## Table of Contents

- [Project Overview](https://github.com/PolisettyTharunSai/project---optimal-route-managing-system/edit/tharun/README.md#project-overview)
- [Features](https://github.com/PolisettyTharunSai/project---optimal-route-managing-system/edit/tharun/README.md#features)
- [Components](https://github.com/PolisettyTharunSai/project---optimal-route-managing-system/edit/tharun/README.md#components)
- [Requirements](https://github.com/PolisettyTharunSai/project---optimal-route-managing-system/edit/tharun/README.md#requirements)
- [Usage](https://github.com/PolisettyTharunSai/project---optimal-route-managing-system/edit/tharun/README.md#usage)
- [Input File Descriptions](#input-file-descriptions)

---

## Project Overview

This project provides an efficient pathfinding solution for dynamic graphs, focusing on determining optimal routes between a source and multiple destinations with live traffic data integration. Using A* and Red-Black Trees (RBT) as a caching mechanism, the program can dynamically update path costs and maintain frequently traversed paths for improved performance over repeated queries.

Key components include:
1. **A*-Based TSP Solver**: An A*-based solution for the Traveling Salesman Problem (TSP), adapted for dynamic updates and optimized shortest paths.
2. **Red-Black Tree Path Caching**: A Red-Black Tree implementation to cache paths, aiding performance by avoiding redundant path calculations.
3. **Dynamic Weight Management**: Weight updates in real-time to simulate traffic conditions, providing more accurate pathfinding as conditions evolve.

## Features

- **Efficient Shortest Path Calculation**: A* algorithm implementation for rapid, optimal path discovery.
- **Dynamic Weight Updates**: Edge weights change based on simulated live traffic conditions, updated iteratively.
- **Persistent Path Caching**: Red-Black Tree structure to cache frequently used paths for future requests.
- **Cycle Detection**: Ensures that updated paths do not introduce cycles, allowing for reliable and robust path selection.
- **File-based Graph Management**: Input graph data from files and update weights as conditions change. Includes utilities to read, write, and update edges on file.

## Components

1. **A* Pathfinding Algorithm**:
   - Heuristic-based shortest path finder between nodes, with Euclidean distance as the primary heuristic.
   - Integrates with the Red-Black Tree caching to avoid redundant path calculations.
   - Used in a modified TSP algorithm to find the nearest neighbor at each iteration for efficient route selection.

2. **Red-Black Tree (RBT)**:
   - Efficiently manages path caching by storing edges with O(log n) time complexity for insertion, search, and deletion.
   - Ensures balanced tree structure with red-black properties (nodes are colored and rotated to maintain balance).
   - Supports quick edge look-up for cached paths.

3. **Dynamic Weight Updates**:
   - Randomizes weights after reaching each destination to simulate real-time traffic changes.
   - Includes utilities to update weight files and reload graphs to reflect current conditions.

## Requirements

- **Compiler**: GCC or similar with support for C99 or above.
- **Libraries**: Standard libraries (`stdio.h`, `stdlib.h`, `math.h`, `limits.h`, `time.h`) included.
- **File-based Inputs**: Edge list formatted in a CSV file for easy graph updates.
- **NOTE** Please make sure that all the files mentioned above optimal_pathfinding main.c and graph_data.csv files to be in the *Same Directory*.
## Usage

1. **Compiling the Code**:
   ```bash
   gcc -o optimal_pathfinding main optimal_pathfinding main.c -lm
   ```
2. **Run the code**:
   ```bash
   ./optimal_pathfinding main
   ```
## Input File Descriptions

1. **FIle name**:
   ```js
   graph_data.csv
   ```
2. **File format**: To represent a directed *edge from node1 to node2* with a *weight of w*.  

   ```js
    <node1>,<node2>,<w>   
   ```
     
