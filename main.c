#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>

#define INF INT_MAX
#define MAX_CITIES 2000
#define MAX_EDGES 10000

typedef struct {
    int city;
    int weight;
} Edge;

typedef struct {
    Edge edges[MAX_EDGES];
    int size;
} Graph;

Graph graph[MAX_CITIES];
int n;

typedef struct {
    int x, y;  // Coordinates for heuristic calculation
} CityLocation;

CityLocation cityLocations[MAX_CITIES];  // Array to store city coordinates

typedef struct {
    int f_cost;  // f(x) = g(x) + h(x)
    int g_cost;  // Actual distance from the start node
    int city;
} AStarNode;

typedef struct {
    AStarNode nodes[MAX_CITIES];
    int size;
} PriorityQueue;

void pq_push(PriorityQueue *pq, int f_cost, int g_cost, int city) {
    if (pq->size >= MAX_CITIES) {
        printf("Error: Priority Queue overflow\n");
        return;
    }
    pq->nodes[pq->size].f_cost = f_cost;
    pq->nodes[pq->size].g_cost = g_cost;
    pq->nodes[pq->size].city = city;
    pq->size++;
}

AStarNode pq_pop(PriorityQueue *pq) {
    if (pq->size == 0) {
        printf("Error: Priority Queue underflow\n");
        return (AStarNode){INF, INF, -1};
    }
    int minIndex = 0;
    for (int i = 1; i < pq->size; i++) {
        if (pq->nodes[i].f_cost < pq->nodes[minIndex].f_cost) {
            minIndex = i;
        }
    }
    AStarNode minNode = pq->nodes[minIndex];
    pq->nodes[minIndex] = pq->nodes[pq->size - 1];
    pq->size--;
    return minNode;
}

int pq_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

// Heuristic function using Euclidean distance between cities
int heuristic(int city1, int city2) {
    int dx = cityLocations[city1].x - cityLocations[city2].x;
    int dy = cityLocations[city1].y - cityLocations[city2].y;
    return (int)sqrt(dx * dx + dy * dy);
}

void a_star(int source, int target, int dist[], int prev[]) {
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[source] = 0;

    PriorityQueue pq = {.size = 0};
    pq_push(&pq, heuristic(source, target), 0, source);

    while (!pq_empty(&pq)) {
        AStarNode current = pq_pop(&pq);
        int u = current.city;

        if (u == target) {
            return;  // Early exit if target is reached
        }

        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].edges[i].city;
            int weight = graph[u].edges[i].weight;
            int tentative_g_cost = dist[u] + weight;

            if (tentative_g_cost < dist[v]) {
                dist[v] = tentative_g_cost;
                prev[v] = u;
                int f_cost = tentative_g_cost + heuristic(v, target);
                pq_push(&pq, f_cost, tentative_g_cost, v);
            }
        }
    }
}

void graph_input(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_CITIES; i++) {
        graph[i].size = 0;
    }

    int node1, node2, weight;
    while (fscanf(file, "%d,%d,%d", &node1, &node2, &weight) == 3) {
        if (node1 >= MAX_CITIES || node2 >= MAX_CITIES) {
            printf("Warning: Node %d or %d exceeds max cities.\n", node1, node2);
            continue;
        }

        if (graph[node1].size < MAX_EDGES) {
            graph[node1].edges[graph[node1].size++] = (Edge){node2, weight};
        }
    }

    fclose(file);
}

void clear_graph() {
    for (int i = 0; i < MAX_CITIES; i++) {
        graph[i].size = 0;
    }
}

void assign_random_weights() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < graph[i].size; j++) {
            graph[i].edges[j].weight = rand() % 100 + 1;  // Random weight between 1 and 100
        }
    }
}

void save_weights_to_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < graph[i].size; j++) {
            fprintf(file, "%d,%d,%d\n", i, graph[i].edges[j].city, graph[i].edges[j].weight);
        }
    }

    fclose(file);
}

void print_path(int start, int end, int prev[]) {
    if (prev[end] != -1 && end != start) {
        print_path(start, prev[end], prev);  // Recursively print the path
    }
    if (end != start) {
        printf(" -> %d", end);  // Print the current node after the previous one
    }
}

int tsp_a_star_based(int source, int destinations[], int num_destinations, const char *filename) {
    int total_cost = 0;
    int visited[MAX_CITIES] = {0};
    int current_city = source;

    visited[source] = 1;
    printf("Path: %d", current_city);  // Start the path from the source city

    for (int count = 0; count < num_destinations; count++) {
        int nearest_city = -1;
        int min_distance = INF;
        int dist[MAX_CITIES], prev[MAX_CITIES];

        // Randomize weights only after the first round
        if (count > 0) {
            assign_random_weights();  // Randomize weights
            save_weights_to_file(filename);  // Save to file
            clear_graph();  // Clear graph structure before reloading
            graph_input(filename);  // Reload the graph with updated weights
        }

        for (int i = 0; i < num_destinations; i++) {
            if (!visited[destinations[i]]) {
                a_star(current_city, destinations[i], dist, prev);  // Use A* for shortest path
                if (dist[destinations[i]] < min_distance) {
                    min_distance = dist[destinations[i]];
                    nearest_city = destinations[i];
                }
            }
        }

        if (nearest_city == -1) {
            printf("\nError: No unvisited destinations remaining.\n");
            return -1;
        }

        // Print the path from current_city to nearest_city, including all intermediate nodes
        if (current_city != nearest_city) {
            print_path(current_city, nearest_city, prev);
        }

        total_cost += min_distance;  // Update total cost
        visited[nearest_city] = 1;  // Mark this city as visited
        current_city = nearest_city;  // Move to the nearest city
    }

    printf("\nTotal minimal path cost to cover all destinations: %d\n", total_cost);

    return total_cost;
}

int main() {
    srand(time(0));  // Seed for random weight generation

    const char *filename = "graph_data0.csv";
    n = MAX_CITIES;
    graph_input(filename);

    int source;
    printf("Enter the source city (0 to %d): ", n - 1);
    scanf("%d", &source);
    if (source < 0 || source >= n) {
        printf("Error: Invalid source city %d. Must be between 0 and %d.\n", source, n - 1);
        return EXIT_FAILURE;
    }

    int num_destinations;
    printf("Enter the number of destinations: ");
    scanf("%d", &num_destinations);
    if (num_destinations <= 0 || num_destinations > MAX_CITIES) {
        printf("Error: Invalid number of destinations. Must be between 1 and %d.\n", MAX_CITIES);
        return EXIT_FAILURE;
    }

    int destinations[MAX_CITIES];
    printf("Enter the destination cities (0 to %d):\n", n - 1);
    for (int i = 0; i < num_destinations; i++) {
        printf("Destination %d: ", i + 1);
        scanf("%d", &destinations[i]);
        if (destinations[i] < 0 || destinations[i] >= n) {
            printf("Error: Invalid destination city %d. Must be between 0 and %d.\n", destinations[i], n - 1);
            return EXIT_FAILURE;
        }
    }

    tsp_a_star_based(source, destinations, num_destinations, filename);

    return EXIT_SUCCESS;
}
