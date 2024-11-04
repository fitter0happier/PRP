#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"

#define INTS_FOR_EDGE 3
#define BYTES_FOR_INT 4 
#define INIT_SIZE 10
#define STR_SIZE 11


// - function -----------------------------------------------------------------
graph_t* allocate_graph(void) {
    graph_t *graph = (graph_t*) malloc(sizeof(graph_t));

    if (graph == NULL) {
        fprintf(stderr, "Failed to create graph!\n");
        exit(-1);
    }

    graph->edges = (edge_t *) malloc(sizeof(edge_t) * INIT_SIZE);
    if (graph->edges == NULL) {
        fprintf(stderr, "Failed to create graph!\n");
        free(graph);
        exit(-1);
    }

    graph->num_edges = 0;
    graph->capacity = INIT_SIZE;
    return graph;
}

// - function -----------------------------------------------------------------
void free_graph(graph_t **graph) {
    if (graph == NULL || *graph == NULL) {
        return;
    }

    if ((*graph)->edges != NULL) {
        free((*graph)->edges);
    }

    free(*graph);
    *graph = NULL;
}

// - function -----------------------------------------------------------------
void load_txt(const char *fname, graph_t *graph) {
    FILE *file = fopen(fname, "r");

    if (file == NULL) {
        fprintf(stderr, "Failed to open file!\n");
        return;
    }

    char read_bytes[STR_SIZE];
    int curr_byte_count = 0;

    int read_ints[INTS_FOR_EDGE];
    int curr_int_count = 0;

    int c;

    while(true) {
        c = getc_unlocked(file);
        if (c == '\n' || c == ' ') {
            read_bytes[curr_byte_count] = '\0';
            curr_byte_count = 0;

            read_ints[curr_int_count++] = atoi(read_bytes);

            if (curr_int_count == INTS_FOR_EDGE) {
                if (graph->num_edges == graph->capacity) {
                    edge_t *larger_edges = realloc(graph->edges, sizeof(edge_t) * 2 * graph->capacity);

                    if (larger_edges == NULL) {
                        fprintf(stderr, "Failed to read file!\n");
                        free_graph(&graph);
                        exit(-1);
                    }

                    graph->edges = larger_edges;
                    graph->capacity *= 2;
                }

                graph->edges[graph->num_edges].from = read_ints[0];
                graph->edges[graph->num_edges].to = read_ints[1];
                graph->edges[graph->num_edges].cost = read_ints[2];
                graph->num_edges++;

                curr_int_count = 0;
            }

        } else if (c == EOF) {
            break;

        } else {
            read_bytes[curr_byte_count++] = (char) c;
        }
    }

    if (fclose(file) != EXIT_SUCCESS) {
        fprintf(stderr, "Failed to close file!\n");
    }
}

// - function -----------------------------------------------------------------
void load_bin(const char *fname, graph_t *graph) {
    FILE *file = fopen(fname, "rb");

    if (file == NULL) {
        fprintf(stderr, "Failed to open file!\n");
        return;
    }

    while (!feof(file)) {
        if (graph->num_edges == graph->capacity) {
            edge_t *larger_edges = realloc(graph->edges, sizeof(edge_t) * 2 * graph->capacity);

            if (larger_edges == NULL) {
                fprintf(stderr, "Failed to read file!\n");
                free_graph(&graph);
                exit(-1);
            }

            graph->edges = larger_edges;
            graph->capacity *= 2;
        }

        if (fread(&graph->edges[graph->num_edges], sizeof(edge_t), 1, file) == 1) {
            graph->num_edges++;
        }
    }

    if (fclose(file) != EXIT_SUCCESS) {
        fprintf(stderr, "Failed to close file!\n");
    }
}

// - function -----------------------------------------------------------------
void save_txt(const graph_t * const graph, const char *fname) {
    FILE *file = fopen(fname, "w");

    if (file == NULL) {
        fprintf(stderr, "Failed to open file!\n");
        return;
    }

    for (int i = 0; i < graph->num_edges; ++i) {
        fprintf(file, "%d %d %d\n", graph->edges[i].from, graph->edges[i].to, graph->edges[i].cost);
    }

    if (fclose(file) != EXIT_SUCCESS) {
        fprintf(stderr, "Failed to close file!\n");
    } 
}

// - function -----------------------------------------------------------------
void save_bin(const graph_t * const graph, const char *fname) {
    FILE *file = fopen(fname, "wb");

    if (file == NULL) {
        fprintf(stderr, "Failed to open file!\n");
        return;
    }

    fwrite(graph->edges, sizeof(edge_t), graph->num_edges, file);

    if (fclose(file) != EXIT_SUCCESS) {
        fprintf(stderr, "Failed to close file!\n");
    }
}
