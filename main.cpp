#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "macro.h"
#include "readFile.hpp"
#include "forest.hpp"
#include "tree.hpp"

// #define NONZERONUM 19800

int main(int argc, char *argv[])
{
    // 读取图/拓扑信息
    label n_edge, n_vertex;
    label *row = readFile("./data/row_10KV", &n_edge);
    label *col = readFile("./data/col_10KV", &n_edge);

    n_vertex = getVertexNum(row, col, n_edge);
    printf("n_edge: %d, n_vertex: %d\n", n_edge, n_vertex);

    Graph *graph = static_malloc(Graph, 1);
    graph->row = row;
    graph->col = col;
    graph->n_edge = n_edge;
    graph->n_vertex = n_vertex;

    Forest *forest = getForest(graph);

    Root *root = getRoot(forest);

    return 0;
}
