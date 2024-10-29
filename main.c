#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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
    int distance;
    int city;
} PQNode;

typedef struct {
    PQNode nodes[MAX_CITIES];
    int size;
} PriorityQueue;

void pq_push(PriorityQueue *pq, int distance, int city) {
    if (pq->size >= MAX_CITIES) {
        printf("Error: Priority Queue overflow\n");
        return;
    }
    pq->nodes[pq->size].distance = distance;
    pq->nodes[pq->size].city = city;
    pq->size++;
}

PQNode pq_pop(PriorityQueue *pq) {
    if (pq->size == 0) {
        printf("Error: Priority Queue underflow\n");
        return (PQNode){INF, -1};
    }
    int minIndex = 0;
    for (int i = 1; i < pq->size; i++) {
        if (pq->nodes[i].distance < pq->nodes[minIndex].distance) {
            minIndex = i;
        }
    }
    PQNode minNode = pq->nodes[minIndex];
    pq->nodes[minIndex] = pq->nodes[pq->size - 1];
    pq->size--;
    return minNode;
}

int pq_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

void dijkstra(int source, int dist[], int prev[]) {
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        prev[i] = -1;  // Initialize previous array
    }
    dist[source] = 0;

    PriorityQueue pq = {.size = 0};
    pq_push(&pq, 0, source);

    while (!pq_empty(&pq)) {
        PQNode current = pq_pop(&pq);
        int u = current.city;

        if (u < 0 || u >= n) {
            printf("Error: Invalid city index %d\n", u);
            continue;
        }

        for (int i = 0; i < graph[u].size; i++) {
            int v = graph[u].edges[i].city;
            int weight = graph[u].edges[i].weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;  // Store the previous city in the shortest path
                pq_push(&pq, dist[v], v);
            }
        }
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

int tsp_dijkstra_based(int source, int destinations[], int num_destinations, const char *filename) {
    int total_cost = 0;
    int visited[MAX_CITIES] = {0};
    int current_city = source;

    visited[source] = 1;
    printf("Path: %d", current_city);  // Start the path from the source city

    for (int count = 0; count < num_destinations; count++) {
        int nearest_city = -1;
        int min_distance = INF;
        int dist[MAX_CITIES], prev[MAX_CITIES];

        dijkstra(current_city, dist, prev);  // Call Dijkstra's algorithm

        for (int i = 0; i < num_destinations; i++) {
            int destination = destinations[i];
            if (!visited[destination] && dist[destination] < min_distance) {
                min_distance = dist[destination];
                nearest_city = destination;
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

    save_weights_to_file(filename);  // Save final weights back to the file
    return total_cost;
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

int main() {
    srand(time(0));  // Seed for random weight generation

    const char *filename = "graph_data.csv";
    n = MAX_CITIES;
    graph_input(filename);

    // Assign random weights to edges
    assign_random_weights();

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

    int total_cost = tsp_dijkstra_based(source, destinations, num_destinations, filename);
    if (total_cost == -1) {
        printf("No feasible path covering all destinations.\n");
    }

    return 0;
}
