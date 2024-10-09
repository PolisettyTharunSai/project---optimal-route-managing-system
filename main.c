#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

typedef struct Edge {
    int vertex;
    int weight;
    struct Edge *next;  // Pointer to the next edge (linked list)
} Edge;

typedef struct {
    Edge **edges;
    int num_nodes;
} Graph;

Graph *create_graph(int num_nodes) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->num_nodes = num_nodes;
    graph->edges = (Edge **)malloc(num_nodes * sizeof(Edge *));
    
    for (int i = 0; i < num_nodes; i++) {
        graph->edges[i] = NULL;
    }
    
    return graph;
}

void add_edge(Graph *graph, int u, int v, int weight) {
    Edge *edge = (Edge *)malloc(sizeof(Edge));
    edge->vertex = v;
    edge->weight = weight;
    edge->next = graph->edges[u];
    graph->edges[u] = edge;

    // Adding the reverse edge since it's an undirected graph
    Edge *reverse_edge = (Edge *)malloc(sizeof(Edge));
    reverse_edge->vertex = u;
    reverse_edge->weight = weight;
    reverse_edge->next = graph->edges[v];
    graph->edges[v] = reverse_edge;
}

void dijkstra(Graph *graph, int start, int *distances) {
    int *visited = (int *)calloc(graph->num_nodes, sizeof(int));
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INF;
    }
    distances[start] = 0;

    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min = INF, min_index;
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && distances[v] <= min) {
                min = distances[v];
                min_index = v;
            }
        }

        visited[min_index] = 1;

        Edge *edge = graph->edges[min_index];
        while (edge != NULL) {
            int v = edge->vertex;
            if (!visited[v] && distances[min_index] != INF && distances[min_index] + edge->weight < distances[v]) {
                distances[v] = distances[min_index] + edge->weight;
            }
            edge = edge->next;
        }
    }
    free(visited);
}

typedef struct {
    int u, v, weight;
} ReducedEdge;

ReducedEdge *build_reduced_graph(Graph *graph, int *vertices, int num_vertices, int *num_edges) {
    *num_edges = (num_vertices * (num_vertices - 1)) / 2;
    ReducedEdge *reduced_graph = (ReducedEdge *)malloc((*num_edges) * sizeof(ReducedEdge));
    int k = 0;
    
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            int u = vertices[i];
            int v = vertices[j];
            int *distances = (int *)malloc(graph->num_nodes * sizeof(int));
            dijkstra(graph, u, distances);
            reduced_graph[k++] = (ReducedEdge){u, v, distances[v]};
            free(distances);
        }
    }
    
    return reduced_graph;
}

typedef struct {
    int parent;
    int rank;
} UnionFind;

UnionFind *create_union_find(int n) {
    UnionFind *uf = (UnionFind *)malloc(n * sizeof(UnionFind));
    for (int i = 0; i < n; i++) {
        uf[i].parent = i;
        uf[i].rank = 0;
    }
    return uf;
}

int find(UnionFind *uf, int u) {
    if (uf[u].parent != u) {
        uf[u].parent = find(uf, uf[u].parent);
    }
    return uf[u].parent;
}

void union_sets(UnionFind *uf, int u, int v) {
    int root_u = find(uf, u);
    int root_v = find(uf, v);
    if (root_u != root_v) {
        if (uf[root_u].rank > uf[root_v].rank) {
            uf[root_v].parent = root_u;
        } else if (uf[root_u].rank < uf[root_v].rank) {
            uf[root_u].parent = root_v;
        } else {
            uf[root_v].parent = root_u;
            uf[root_u].rank++;
        }
    }
}

int compare_edges(const void *a, const void *b) {
    return ((ReducedEdge *)a)->weight - ((ReducedEdge *)b)->weight;
}

int kruskal_mst(ReducedEdge *edges, int num_edges, int num_vertices) {
    qsort(edges, num_edges, sizeof(ReducedEdge), compare_edges);

    UnionFind *uf = create_union_find(num_vertices);
    int mst_weight = 0, mst_edges = 0;

    for (int i = 0; i < num_edges && mst_edges < num_vertices - 1; i++) {
        if (find(uf, edges[i].u) != find(uf, edges[i].v)) {
            union_sets(uf, edges[i].u, edges[i].v);
            mst_weight += edges[i].weight;
            mst_edges++;
        }
    }

    free(uf);
    return mst_weight;
}

int minimum_weight_cover(Graph *graph, int *vertices, int num_vertices) {
    int num_edges;
    ReducedEdge *reduced_graph = build_reduced_graph(graph, vertices, num_vertices, &num_edges);
    int mst_weight = kruskal_mst(reduced_graph, num_edges, num_vertices);
    free(reduced_graph);
    return mst_weight;
}

int main() {
    Graph *graph = create_graph(6);
    
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 3);
    add_edge(graph, 1, 2, 1);
    add_edge(graph, 1, 3, 2);
    add_edge(graph, 2, 3, 4);
    add_edge(graph, 3, 4, 2);
    add_edge(graph, 4, 5, 6);
    add_edge(graph, 3, 5, 5);
    
    int vertices_to_cover[] = {0, 3, 4, 5};
    int num_vertices = 4;
    
    int result = minimum_weight_cover(graph, vertices_to_cover, num_vertices);
    printf("Minimum weight to cover vertices: %d\n", result);
    
    return 0;
}
