#include "forest.hpp"

Forest *getForest(Graph *graph)
{
    label *row = graph->row;
    label *col = graph->col;
    label n_edge = graph->n_edge;
    label n_vertex = graph->n_vertex;

    label *parentCnt = static_calloc(label, n_vertex);
    label *childCnt = static_calloc(label, n_vertex);

    /***
     * 统计某个节点的亲子节点数量
     * 我们假定所有边都从高节点指向低节点
     ***/
    for (int i_edge = 0; i_edge < n_edge; i_edge++) {
        parentCnt[row[i_edge]]++;
        childCnt[col[i_edge]]++;
    }

    /***
     * 统计所有节点最大的度数存到Delta
     * 度数即为亲节点和子节点数量和
     ***/
    int Delta = 0;
    label *degree = static_calloc(label, n_vertex);
    for (int i_vertex = 0; i_vertex < n_vertex; i_vertex++) {
        degree[i_vertex] = parentCnt[i_vertex] + childCnt[i_vertex];
        if (degree[i_vertex] > Delta)
            Delta = degree[i_vertex];
    }

    /***
     * 一个点的亲节点一定分到不同的森林中
     * 将这些亲节点序号从低到高编号到对应的森林
     * 依此得到不同森林的叶子数
     ***/
    label *leafCnt = static_calloc(label, Delta);
    for (int i_vertex = 0; i_vertex < n_vertex; i_vertex++) {
        for (int i_parent = 0; i_parent < parentCnt[i_vertex]; i_parent++) {
            leafCnt[i_parent]++;
        }
    }

    /***
     * 统计不同森林的起始位置
     ***/
    label *forestIdx = static_malloc(label, Delta + 1);
    label *forestPos = static_malloc(label, Delta);
    forestIdx[0] = 0;
    forestIdx[Delta] = n_edge;
    forestPos[0] = 0;
    for (int i_forest = 1; i_forest < Delta; i_forest++) {
        forestIdx[i_forest] = forestIdx[i_forest - 1] + leafCnt[i_forest - 1];
        forestPos[i_forest] = forestIdx[i_forest];
    }

    /***
     * 重新排列`row`和`col`，使其边按照森林划分排列
     ***/
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

    /***
     * 输出
     ***/
    Forest *forest = static_malloc(Forest, 1);
    forest->row = rowNew;
    forest->col = colNew;
    forest->forestIdx = forestIdx;
    forest->Delta = Delta;
    forest->n_edge = n_edge;
    forest->n_vertex = n_vertex;

    free(parentCnt);
    free(childCnt);
    free(degree);
    free(leafCnt);
    free(forestPos);

    return forest;
}