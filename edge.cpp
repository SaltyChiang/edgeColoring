#include "edge.hpp"

ColorEdge *getColorEdge(Forest *forestGraph, ColorTree *colorTreeGraph)
{
    label *forestIdx = forestGraph->forestIdx;

    label Delta = forestGraph->Delta;
    label n_edge = forestGraph->n_edge;
    label n_vertex = forestGraph->n_vertex;

    label n_edge_color = 2 * Delta - 1;

    label *colorEdge = static_malloc(label, n_edge);
    label *colorUsed = static_calloc(label, n_vertex);
    for (int i_forest = 0; i_forest < Delta; i_forest++) {
        label n_leaf = forestIdx[i_forest + 1] - forestIdx[i_forest];
        if (n_leaf > 0) {
            label *row = forestGraph->row + forestIdx[i_forest];
            label *col = forestGraph->col + forestIdx[i_forest];
            label *colorTree = colorTreeGraph->colorTree + forestIdx[i_forest];
            label *colorEdgeLocal = colorEdge + forestIdx[i_forest];
            for (int i_color = 0; i_color < 3; i_color++) {
                for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                    if (colorTree[i_leaf] == i_color) {
                        label colorUsedLocal = colorUsed[row[i_leaf]] | colorUsed[col[i_leaf]];
                        for (int i_edge_color = 0; i_edge_color < n_edge_color; i_edge_color++) {
                            if (!((colorUsedLocal >> i_edge_color) & 1)) {
                                colorUsed[row[i_leaf]] |= (1 << i_edge_color);
                                colorUsed[col[i_leaf]] |= (1 << i_edge_color);
                                colorEdgeLocal[i_leaf] = i_edge_color;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    ColorEdge *colorEdgeGraph = static_malloc(ColorEdge, 1);
    colorEdgeGraph->colorEdge = colorEdge;

    free(colorUsed);

    return colorEdgeGraph;
}
