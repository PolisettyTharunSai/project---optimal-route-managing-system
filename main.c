#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INF INT_MAX
#define MAX_CITIES 5000
#define MAX_EDGES 10000 // Maximum number of edges per city
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

// Main MDMSMD with TSP Heuristic
int MDMSMD_TSP(int source, int destinations[], int num_destinations) {
    int dist_matrix[MAX_CITIES][MAX_CITIES];

    // Step 1: Run Dijkstra's from each city
    for (int i = 0; i < num_destinations; i++) {
        int dist[MAX_CITIES];
        dijkstra(destinations[i], dist);
        for (int j = 0; j < num_destinations; j++) {
            dist_matrix[i][j] = dist[destinations[j]];
        }
    }

    // Step 2: Apply TSP heuristic to minimize total travel cost
    return tsp_nearest_neighbor(0, dist_matrix, destinations, num_destinations);
}

int main() {
    n = 5; // Number of cities

    // Example graph edges (city1, city2, weight)
    graph[0].edges[0] = (Edge){1, 9}; graph[0].size = 1;
    graph[0].edges[1] = (Edge){2, 20}; graph[0].size = 2;
    
    graph[1].edges[0] = (Edge){2, 5}; graph[1].size = 1;
    graph[1].edges[1] = (Edge){3, 1}; graph[1].size = 2;
    graph[1].edges[2] = (Edge){4, 5}; graph[1].size = 3;
    
    graph[2].edges[0] = (Edge){3, 30}; graph[2].size = 1;
    graph[2].edges[1] = (Edge){4, 25}; graph[2].size = 2;
    
    graph[3].edges[0] = (Edge){4, 15}; graph[3].size = 1;
    
    graph[4].edges[0] = (Edge){3, 7}; graph[4].size = 1;

    // Cities to visit
    int destinations[] = {0, 1, 3, 4};
    int num_destinations = sizeof(destinations) / sizeof(destinations[0]);

    // Run the algorithm
    int result = MDMSMD_TSP(0, destinations, num_destinations);
    printf("Minimum travel cost covering all cities: %d\n", result);

    return 0;
}
