#include "forest.hpp"

Forest *getForest(label *row, label *col, const label n_edge, const label n_vertex)
{
    label *parentCnt = static_calloc(label, n_vertex);
    label *childCnt = static_calloc(label, n_vertex);

    for (int i_edge = 0; i_edge < n_edge; i_edge++) {
        parentCnt[row[i_edge]]++;
        childCnt[col[i_edge]]++;
    }

    int Delta = 0;
    label *degree = static_calloc(label, n_vertex);
    for (int i_vertex = 0; i_vertex < n_vertex; i_vertex++) {
        degree[i_vertex] = parentCnt[i_vertex] + childCnt[i_vertex];
        if (degree[i_vertex] > Delta)
            Delta = degree[i_vertex];
    }

    label *leafCnt = static_calloc(label, Delta);
    for (int i_vertex = 0; i_vertex < n_vertex; i_vertex++) {
        for (int i_parent = 0; i_parent < parentCnt[i_vertex]; i_parent++) {
            leafCnt[i_parent]++;
        }
    }

    label *forestIdx = static_malloc(label, Delta + 1);
    label *forestPos = static_malloc(label, Delta);
    forestIdx[0] = 0;
    forestIdx[Delta] = n_edge;
    forestPos[0] = 0;
    for (int i_forest = 1; i_forest < Delta; i_forest++) {
        forestIdx[i_forest] = forestIdx[i_forest - 1] + leafCnt[i_forest - 1];
        forestPos[i_forest] = forestIdx[i_forest];
    }

    label *rowNew = static_malloc(label, n_edge);
    label *colNew = static_malloc(label, n_edge);
    int i_edge = 0;
    for (int i_vertex = 0; i_vertex < n_vertex; i_vertex++) {
        for (int i_parent = 0; i_parent < parentCnt[i_vertex]; i_parent++) {
            rowNew[forestPos[i_parent]] = row[i_edge];
            colNew[forestPos[i_parent]] = col[i_edge];
            forestPos[i_parent]++;
            i_edge++;
        }
    }

    Forest *forest = static_malloc(Forest, 1);
    forest->row = rowNew;
    forest->col = colNew;
    forest->forestIdx = forestIdx;
    forest->Delta = Delta;

    free(parentCnt);
    free(childCnt);
    free(degree);
    free(leafCnt);
    free(forestPos);

    return forest;
}