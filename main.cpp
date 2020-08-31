#include "macro.h"
#include "readFile.hpp"
#include "forest.hpp"
#include "tree.hpp"
#include "edge.hpp"

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

    // Forest *forest = getForest(graph);

    // Root *root = getRoot(forest);

    // ColorTree *colorTree = getColorTree(forest, root);

    // ColorEdge *colorEdge = getColorEdge(forest, colorTree);

    Forest *forest = getForestT(graph);
    Root *root = getRootT(forest);
    ColorTree *colorTree = getColorTreeT(forest, root);

    for (int i = forest->forestIdx[0]; i < forest->forestIdx[1]; i++) {
        printf("%d %d %d\n", forest->row[i], forest->col[i], colorTree->colorTree[i]);
    }

    // label *colorUsed = static_calloc(label, n_vertex);
    // for (int i_edge = 0; i_edge < n_edge; i_edge++) {
    //     unsigned int colors = colorUsed[forest->row[i_edge]] | colorUsed[forest->col[i_edge]];
    //     if ((1 << colorEdge->colorEdge[i_edge]) & (colors)) {
    //         printf("NOOOOO!");
    //     }
    //     colorUsed[forest->row[i_edge]] |= (1 << colorEdge->colorEdge[i_edge]);
    //     colorUsed[forest->col[i_edge]] |= (1 << colorEdge->colorEdge[i_edge]);
    // }

    return 0;
}
