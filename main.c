#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX
#define MAX_CITIES 5000
#define MAX_EDGES 1000 // Maximum number of edges per city

typedef struct {
    int city;
    int weight;
} Edge;

typedef struct {
    Edge edges[MAX_EDGES];
    int size; // Number of edges
} Graph;

typedef struct {
    int distance;
    int city;
} PQNode;

typedef struct {
    PQNode nodes[MAX_CITIES];
    int size;
} PriorityQueue;

Graph graph[MAX_CITIES]; // Array of graphs for each city
int n; // Number of cities

// Function to create a priority queue
void pq_push(PriorityQueue *pq, int distance, int city) {
    pq->nodes[pq->size].distance = distance;
    pq->nodes[pq->size].city = city;
    pq->size++;
}

PQNode pq_pop(PriorityQueue *pq) {
    // Find the node with the minimum distance
    int minIndex = 0;
    for (int i = 1; i < pq->size; i++) {
        if (pq->nodes[i].distance < pq->nodes[minIndex].distance) {
            minIndex = i;
        }
    }
    PQNode minNode = pq->nodes[minIndex];
    pq->nodes[minIndex] = pq->nodes[pq->size - 1]; // Replace with the last node
    pq->size--;
    return minNode;
}

int pq_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

// Dijkstra's algorithm
void dijkstra(int source, int dist[]) {
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }
    dist[source] = 0;

    PriorityQueue pq = { .size = 0 };
    pq_push(&pq, 0, source);

    while (!pq_empty(&pq)) {
        PQNode current = pq_pop(&pq);
        int u = current.city;

        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].edges[i].city;
            int weight = graph[u].edges[i].weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq_push(&pq, dist[v], v);
            }
        }
    }
}

// TSP Nearest Neighbor Heuristic
int tsp_nearest_neighbor(int start, int dist_matrix[MAX_CITIES][MAX_CITIES], int cities[], int num_cities) {
    int total_cost = 0;
    int visited[MAX_CITIES] = { 0 };
    int current_city = start;

    visited[start] = 1;

    for (int i = 1; i < num_cities; i++) {
        int nearest_city = -1;
        int min_distance = INF;

        for (int j = 0; j < num_cities; j++) {
            if (!visited[j] && dist_matrix[current_city][j] < min_distance) {
                min_distance = dist_matrix[current_city][j];
                nearest_city = j;
            }
        }

        if (nearest_city != -1) {
            total_cost += min_distance;
            visited[nearest_city] = 1;
            current_city = nearest_city;
        }
    }

    return total_cost;
}

// Function to read graph data from a CSV file and stop at -1, -1, 0
void graph_input(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Clear the graph
    for (int i = 0; i < MAX_CITIES; i++) {
        graph[i].size = 0;
    }

    // Read the edges from the file
    int node1, node2, weight;
    while (fscanf(file, "%d,%d,%d", &node1, &node2, &weight) != EOF) {
        // Stop input when we encounter -1, -1, 0
        if (node1 == -1 && node2 == -1 && weight == 0) {
            break;
        }

        // Add the edge to the graph
        graph[node1].edges[graph[node1].size] = (Edge){node2, weight};
        graph[node1].size++;
    }

    fclose(file);
}

// Function to rewrite the file based on the updated graph
void rewrite_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Write the updated edges to the file
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < graph[i].size; j++) {
            fprintf(file, "%d,%d,%d\n", i, graph[i].edges[j].city, graph[i].edges[j].weight);
        }
    }

    fclose(file);
}

// Main MDMSMD with TSP Heuristic
int MDMSMD_TSP(int source, int destinations[], int num_destinations, const char *filename) {
    int dist_matrix[MAX_CITIES][MAX_CITIES];

    int current_source = source;
    
    while (num_destinations > 1) { // While there's more than 1 city to visit
        // Step 1: Run Dijkstra's from each city
        for (int i = 0; i < num_destinations; i++) {
            int dist[MAX_CITIES];
            dijkstra(destinations[i], dist);
            for (int j = 0; j < num_destinations; j++) {
                dist_matrix[i][j] = dist[destinations[j]];
            }
        }

        // Step 2: Apply TSP heuristic to find the next nearest city
        int travel_cost = tsp_nearest_neighbor(0, dist_matrix, destinations, num_destinations);
        printf("Travel cost: %d\n", travel_cost);

        // Step 3: Find the nearest node from the current source
        int nearest_city = -1;
        int min_distance = INF;
        for (int i = 0; i < num_destinations; i++) {
            if (dist_matrix[current_source][i] < min_distance) {
                min_distance = dist_matrix[current_source][i];
                nearest_city = i;
            }
        }

        // Step 4: Update the graph by rewriting the file
        rewrite_file(filename);

        // Step 5: Set the current source to the nearest node and repeat the process
        current_source = nearest_city;
    }

    return 0;
}

int main() {
    const char *filename = "graph_data.csv";

    // Load the graph from the CSV file
    graph_input(filename);

    n = 5; // Number of cities (for example)

    // Cities to visit
    int destinations[] = {0, 1, 3, 4};
    int num_destinations = sizeof(destinations) / sizeof(destinations[0]);

    // Run the algorithm
    MDMSMD_TSP(0, destinations, num_destinations, filename);

    return 0;
}
