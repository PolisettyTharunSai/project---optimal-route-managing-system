//Including all the Required Libraries.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>

#define INF INT_MAX
//Limiting the max allowed values.
#define MAX_CITIES 2000
#define MAX_EDGES  1000

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

//Co-ordinates for nodes.
typedef struct {
    int x, y;  
} CityLocation;

CityLocation cityLocations[MAX_CITIES];

typedef struct {
    int f_cost;  // f(x) = g(x) + h(x)
    int g_cost;  // Actual distance from the start node
    int city;
} AStarNode;

typedef struct {
    AStarNode nodes[MAX_CITIES];
    int size;
} PriorityQueue;
typedef enum { RED, BLACK } Color;
typedef struct RBTNode {
    int node1, node2;  // Edge representation
    int color;  // 0: Red, 1: Black
    struct RBTNode *left, *right, *parent;
} RBTNode;

RBTNode *rbt_root = NULL;

// Function Prototypes
void pq_push(PriorityQueue *pq, int f_cost, int g_cost, int city);
AStarNode pq_pop(PriorityQueue *pq);
int pq_empty(PriorityQueue *pq);
int heuristic(int city1, int city2);
void a_star(int source, int target, int dist[], int prev[]);
int graph_input(const char *filename);
void clear_graph();
void save_weights_to_file(const char *filename);
void print_path(int start, int end, int prev[]);
void remove_edge(int node1, int node2);
void update_file(const char *filename, int node1, int node2);
void tsp_a_star_based(int source, int destinations[], int num_destinations, const char *filename);
//RBtree functions
RBTNode *create_node(int node1, int node2) {
    RBTNode *new_node = (RBTNode *)malloc(sizeof(RBTNode));
    new_node->node1 = node1;
    new_node->node2 = node2;
    new_node->color = RED; // New nodes are always red
    new_node->left = new_node->right = new_node->parent = NULL;
    return new_node;
}

void left_rotate(RBTNode **root, RBTNode *x) {
    RBTNode *y = x->right;
    x->right = y->left;
    if (y->left != NULL) y->left->parent = x;
    y->parent = x->parent;

    if (x->parent == NULL) {
        *root = y; // Make y the new root
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void right_rotate(RBTNode **root, RBTNode *y) {
    RBTNode *x = y->left;
    y->left = x->right;
    if (x->right != NULL) x->right->parent = y;
    x->parent = y->parent;

    if (y->parent == NULL) {
        *root = x; // Make x the new root
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void fix_insert(RBTNode **root, RBTNode *z) {
    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBTNode *y = z->parent->parent->right;
            if (y != NULL && y->color == RED) {
                // Case 1: Uncle is RED
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // Case 2: Uncle is BLACK
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(root, z->parent->parent);
            }
        } else {
            RBTNode *y = z->parent->parent->left;
            if (y != NULL && y->color == RED) {
                // Case 1: Uncle is RED
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // Case 2: Uncle is BLACK
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK; // Ensure the root is always black
}

void rbt_insert(RBTNode **root, int node1, int node2) {
    RBTNode *z = create_node(node1, node2);
    RBTNode *y = NULL;
    RBTNode *x = *root;

    while (x != NULL) {
        y = x;
        if (z->node1 < x->node1 || (z->node1 == x->node1 && z->node2 < x->node2)) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;

    if (y == NULL) {
        *root = z; // Tree was empty
    } else if (z->node1 < y->node1 || (z->node1 == y->node1 && z->node2 < y->node2)) {
        y->left = z;
    } else {
        y->right = z;
    }

    fix_insert(root, z);
}

void transplant(RBTNode **root, RBTNode *u, RBTNode *v) {
    if (u->parent == NULL) {
        *root = v; // v is the new root
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

RBTNode *minimum(RBTNode *node) {
    while (node->left != NULL) node = node->left;
    return node;
}

void fix_delete(RBTNode **root, RBTNode *x) {
    while (x != *root && (x == NULL || x->color == BLACK)) {  // Ensure x is not NULL
        if (x == x->parent->left) {
            RBTNode *w = x->parent->right;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(root, x->parent);
                w = x->parent->right;
            }
            if ((w->left == NULL || w->left->color == BLACK) &&
                (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right == NULL || w->right->color == BLACK) {
                    if (w->left) w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right) w->right->color = BLACK;
                left_rotate(root, x->parent);
                x = *root;
            }
        } else {
            RBTNode *w = x->parent->left;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(root, x->parent);
                w = x->parent->left;
            }
            if ((w->right == NULL || w->right->color == BLACK) &&
                (w->left == NULL || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left == NULL || w->left->color == BLACK) {
                    if (w->right) w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left) w->left->color = BLACK;
                right_rotate(root, x->parent);
                x = *root;
            }
        }
    }
    if (x) x->color = BLACK;  // Ensure x is black
}

void rbt_delete(RBTNode **root, int node1, int node2) {
    RBTNode *z = *root;
    RBTNode *x = NULL, *y;

    while (z != NULL) {
        if (z->node1 == node1 && z->node2 == node2) {
            break;
        }
        if (z->node1 < node1 || (z->node1 == node1 && z->node2 < node2)) {
            z = z->right;
        } else {
            z = z->left;
        }
    }

    if (z == NULL) return;  // Node not found

    y = z;
    Color y_original_color = y->color;

    if (z->left == NULL) {
        x = z->right;
        transplant(root, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        transplant(root, z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (y_original_color == BLACK && x != NULL) {
        fix_delete(root, x);
    }
}

int rbt_search(RBTNode *root, int node1, int node2) {
    while (root != NULL) {
        if (node1 == root->node1 && node2 == root->node2) {
            return 1; // Found
        }
        if (node1 < root->node1 || (node1 == root->node1 && node2 < root->node2)) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return 0; // Not found
}

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
    pq->nodes[minIndex] = pq->nodes[--pq->size];  // Remove the last node
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
// Implements the A* algorithm to find the shortest path from the source to the target node in a graph, using distance and Euclidean heuristic values for optimal pathfinding.

void a_star(int source, int target, int dist[], int prev[]) {
//intialsing dist of all nodes to INF and prev to -1
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    //dist of source =0
    dist[source] = 0;

    PriorityQueue pq = {.size = 0};
    pq_push(&pq, heuristic(source, target), 0, source);
	// Processes nodes in the priority queue to explore potential paths towards the target using heuristic estimates for prioritization. 
	// Updates the shortest distance and predecessor information for each neighbor if a better path is found based on the heuristic.
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
//taking the graph input from the file  and return the max node value
int graph_input(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return -1;  // Return -1 on failure
    }

    for (int i = 0; i < MAX_CITIES; i++) {
        graph[i].size = 0;
    }

    int node1, node2, weight;
    int max_city = -1;  // Initialize max_city to -1 to handle cases where no valid node is read

    while (fscanf(file, "%d,%d,%d", &node1, &node2, &weight) == 3) {
        if (node1 >= MAX_CITIES || node2 >= MAX_CITIES) {
            printf("Warning: Node %d or %d exceeds max cities.\n", node1, node2);
            continue;
        }

        // Update max_city with the maximum of node1, node2, and the current max_city
        max_city = (node1 > node2 ? node1 : node2) > max_city ? (node1 > node2 ? node1 : node2) : max_city;

        if (graph[node1].size < MAX_EDGES) {
            graph[node1].edges[graph[node1].size++] = (Edge){node2, weight};
            rbt_insert(&rbt_root, node1, node2);  // Insert edge into RBT
        }
    }

    fclose(file);
    return max_city;  // Return the maximum node value encountered
}

//to clear the graph
void clear_graph() {
    for (int i = 0; i < MAX_CITIES; i++) {
        graph[i].size = 0;
    }
}
//to print the optimal path travelled
void print_path(int start, int end, int prev[]) {
    if (prev[end] != -1 && end != start) {
        print_path(start, prev[end], prev);  // Recursively print the path
    }
    if (end != start) {
        printf(" -> %d", end);  // Print the current node after the previous one
    }
}
//to remove an avoided edge from graph
void remove_edge(int node1, int node2) {
    for (int i = 0; i < graph[node1].size; i++) {
        if (graph[node1].edges[i].city == node2) {
            graph[node1].edges[i] = graph[node1].edges[--graph[node1].size];  // Remove edge
            return;
        }
    }
}
//to remove the avoided edges from the file 
void update_file(const char *filename, int node1, int node2) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (!temp) {
        printf("Error: Could not create temp file.\n");
        fclose(file);
        return;
    }

    int n1, n2, weight;
    while (fscanf(file, "%d,%d,%d", &n1, &n2, &weight) == 3) {
        if (!(n1 == node1 && n2 == node2)) {
            fprintf(temp, "%d,%d,%d\n", n1, n2, weight);
        }
    }

    fclose(file);
    fclose(temp);
    remove(filename);
    rename("temp.csv", filename);
}
//to update the weight of edge to new weight 
void update_edge_weight(int node1, int node2, int new_weight) {
    for (int i = 0; i < graph[node1].size; i++) {
        if (graph[node1].edges[i].city == node2) {
            graph[node1].edges[i].weight = new_weight;
            break;
        }
    }
}

// Function to save the current graph weights to a file
void save_weights_to_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < graph[i].size; j++) {
            fprintf(file, "%d %d %d\n", i, graph[i].edges[j].city, graph[i].edges[j].weight);
        }
    }

    fclose(file);
}
//generating random weights 
void randomize_edge_weights() {
    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < graph[i].size; j++) {
            // Set new random weight between 1 and 100 (or any range you choose)
            int new_weight = rand() % 100 + 1;
            graph[i].edges[j].weight = new_weight;
        }
    }
}

// Implements a modified Traveling Salesman Problem (TSP) solution using A* to find the nearest unvisited destination while allowing for dynamic weight updates after the first selection. 
// Tracks the total path cost and updates edge weights randomly to reflect changes in the graph's state, ensuring efficient route recalculations.
void tsp_a_star_based(int source, int destinations[], int num_destinations, const char *filename) {
    int total_cost = 0;
    int visited[MAX_CITIES] = {0};
    int current_city = source;

    visited[source] = 1;
    printf("Path: %d", current_city);

    // Flag to track if we have selected the first nearest neighbor
    int first_selection_done = 0;

    for (int count = 0; count < num_destinations; count++) {
        int nearest_city = -1;
        int min_distance = INF;
        int dist[MAX_CITIES], prev[MAX_CITIES];

        for (int i = 0; i < num_destinations; i++) {
            if (!visited[destinations[i]]) {
                a_star(current_city, destinations[i], dist, prev); // Implement A* here
                if (dist[destinations[i]] < min_distance) {
                    min_distance = dist[destinations[i]];
                    nearest_city = destinations[i];
                }
            }
        }

        if (nearest_city == -1) {
            printf("\nError: No unvisited destinations remaining.\n");
            return;
        }

        if (current_city != nearest_city) {
            print_path(current_city, nearest_city, prev); // Function to print the path
        }

        total_cost += min_distance;
        visited[nearest_city] = 1;

        // Update weights randomly only after the first selection
        if (!first_selection_done && num_destinations>1) {
            // Randomize edge weights after the first nearest neighbor selection
            randomize_edge_weights();

            // Save updated weights to the file
            save_weights_to_file(filename);

            // Mark the first selection as done
            first_selection_done = 1;
        }

        current_city = nearest_city;
    }

    printf("\nTotal minimal path cost to cover all destinations: %d\n", total_cost);
}

int main() {
    srand(time(0));  // Seed for random weight generation

    const char *filename = "graph_data1.csv";
    n = graph_input(filename)+1;    //0 based indexing give one less city.
    // User input for edges to exclude
    int num_excluded;
    printf("Enter the number of edges to exclude: ");
    scanf("%d", &num_excluded);

    for (int i = 0; i < num_excluded; i++) {
        int node1, node2;
        printf("Enter edge to exclude (node1,node2): ");
        scanf("%d,%d", &node1, &node2);
        remove_edge(node1, node2);
        rbt_delete(&rbt_root, node1, node2);  // Delete from the Red-Black Tree
        update_file(filename, node1, node2);
    }

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
	//calling the function to output the optimal path and optimal most
    tsp_a_star_based(source, destinations, num_destinations, filename);

    return EXIT_SUCCESS;
}
