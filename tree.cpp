#include "tree.hpp"

Root *getRoot(Forest *forestGraph)
{
    label *forestIdx = forestGraph->forestIdx;

    label Delta = forestGraph->Delta;
    label n_edge = forestGraph->n_edge;
    label n_vertex = forestGraph->n_vertex;

    size_t bitmapCount = (n_vertex / 8 + 1) * sizeof(unsigned char);
    unsigned char *rowBitmap = static_malloc(unsigned char, bitmapCount);
    unsigned char *colBitmap = static_malloc(unsigned char, bitmapCount);

    label **rootForest = static_malloc(label *, Delta);
    label *rootIdx = static_malloc(label, Delta + 1);
    rootIdx[0] = 0;

    /***
     * 按照森林循环
     ***/
    for (int i_forest = 0; i_forest < Delta; i_forest++) {
        rootIdx[i_forest + 1] = rootIdx[i_forest];

        /***
         * 按照森林中的叶子循环
         * 使用Bitmap统计在`col`队列但不在`row`队列中的节点
         * 这些节点就是森林中的根
         * 每个根对应一棵树
         ***/
        label n_leaf = forestIdx[i_forest + 1] - forestIdx[i_forest];
        if (n_leaf > 0) {
            memset(rowBitmap, 0, bitmapCount * sizeof(unsigned char));
            memset(colBitmap, 0, bitmapCount * sizeof(unsigned char));
            label *row = forestGraph->row + forestIdx[i_forest];
            label *col = forestGraph->col + forestIdx[i_forest];
            int rootCnt = 0;

            for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                rowBitmap[row[i_leaf] / 8] |= 1 << (row[i_leaf] % 8);
                colBitmap[col[i_leaf] / 8] |= 1 << (col[i_leaf] % 8);
            }

            for (int i_char_vertex = 0; i_char_vertex < n_vertex / 8 + 1; i_char_vertex++) {
                rowBitmap[i_char_vertex] ^= colBitmap[i_char_vertex];
                colBitmap[i_char_vertex] &= rowBitmap[i_char_vertex];
                if (colBitmap[i_char_vertex]) {
                    unsigned char charCnt = colBitmap[i_char_vertex];
                    charCnt = ((charCnt >> 1) & 0x55) + (charCnt & 0x55);
                    charCnt = ((charCnt >> 2) & 0x33) + (charCnt & 0x33);
                    charCnt = (charCnt >> 4) + (charCnt & 0x0f);
                    rootCnt += (int) charCnt;
                }
            }
            rootIdx[i_forest + 1] += rootCnt;

            rootForest[i_forest] = static_malloc(label, rootCnt);
            int i_root = 0;
            for (int i_vertex = 0; i_vertex < n_vertex; i_vertex++) {
                if (colBitmap[i_vertex / 8] & (1 << (i_vertex % 8))) {
                    rootForest[i_forest][i_root] = i_vertex;
                    i_root++;
                }
            }
        }
    }

    /***
     * 按照森林循环
     * 将之前得到的根存储到一起，按照森林的顺序排列
     ***/
    label *root = static_malloc(label, rootIdx[Delta]);
    for (int i_forest = 0; i_forest < Delta; i_forest++) {
        int n_leaf = forestIdx[i_forest + 1] - forestIdx[i_forest];
        if (n_leaf > 0) {
            int n_root = rootIdx[i_forest + 1] - rootIdx[i_forest];
            memcpy(root + rootIdx[i_forest], rootForest[i_forest], n_root * sizeof(label));
            free(rootForest[i_forest]);
        }
    }

    /***
     * 输出
     ***/
    Root *rootGraph = static_malloc(Root, 1);
    rootGraph->root = root;
    rootGraph->rootIdx = rootIdx;

    free(rowBitmap);
    free(colBitmap);

    return rootGraph;
}

Root *getRootT(Forest *forestGraph)
{
    label *forestIdx = forestGraph->forestIdx;

    label Delta = forestGraph->Delta;
    label n_edge = forestGraph->n_edge;
    label n_vertex = forestGraph->n_vertex;

    size_t bitmapCount = (n_vertex / 8 + 1) * sizeof(unsigned char);
    unsigned char *rowBitmap = static_malloc(unsigned char, bitmapCount);
    unsigned char *colBitmap = static_malloc(unsigned char, bitmapCount);

    label **rootForest = static_malloc(label *, Delta);
    label *rootIdx = static_malloc(label, Delta + 1);
    rootIdx[0] = 0;

    for (int i_forest = 0; i_forest < Delta; i_forest++) {
        rootIdx[i_forest + 1] = rootIdx[i_forest];

        label n_leaf = forestIdx[i_forest + 1] - forestIdx[i_forest];
        if (n_leaf > 0) {
            memset(rowBitmap, 0, bitmapCount * sizeof(unsigned char));
            memset(colBitmap, 0, bitmapCount * sizeof(unsigned char));
            label *row = forestGraph->row + forestIdx[i_forest];
            label *col = forestGraph->col + forestIdx[i_forest];
            int rootCnt = 0;

            for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                rowBitmap[row[i_leaf] / 8] |= 1 << (row[i_leaf] % 8);
                colBitmap[col[i_leaf] / 8] |= 1 << (col[i_leaf] % 8);
            }

            for (int i_char_vertex = 0; i_char_vertex < n_vertex / 8 + 1; i_char_vertex++) {
                colBitmap[i_char_vertex] ^= rowBitmap[i_char_vertex];
                rowBitmap[i_char_vertex] &= colBitmap[i_char_vertex];
                if (rowBitmap[i_char_vertex]) {
                    unsigned char charCnt = rowBitmap[i_char_vertex];
                    charCnt = ((charCnt >> 1) & 0x55) + (charCnt & 0x55);
                    charCnt = ((charCnt >> 2) & 0x33) + (charCnt & 0x33);
                    charCnt = (charCnt >> 4) + (charCnt & 0x0f);
                    rootCnt += (int) charCnt;
                }
            }
            rootIdx[i_forest + 1] += rootCnt;

            rootForest[i_forest] = static_malloc(label, rootCnt);
            int i_root = 0;
            for (int i_vertex = 0; i_vertex < n_vertex; i_vertex++) {
                if (rowBitmap[i_vertex / 8] & (1 << (i_vertex % 8))) {
                    rootForest[i_forest][i_root] = i_vertex;
                    i_root++;
                }
            }
        }
    }

    label *root = static_malloc(label, rootIdx[Delta]);
    for (int i_forest = 0; i_forest < Delta; i_forest++) {
        int n_leaf = forestIdx[i_forest + 1] - forestIdx[i_forest];
        if (n_leaf > 0) {
            int n_root = rootIdx[i_forest + 1] - rootIdx[i_forest];
            memcpy(root + rootIdx[i_forest], rootForest[i_forest], n_root * sizeof(label));
            free(rootForest[i_forest]);
        }
    }

    /***
     * 输出
     ***/
    Root *rootGraph = static_malloc(Root, 1);
    rootGraph->root = root;
    rootGraph->rootIdx = rootIdx;

    free(rowBitmap);
    free(colBitmap);

    return rootGraph;
}

ColorTree *getColorTree(Forest *forestGraph, Root *rootGraph)
{
    label *forestIdx = forestGraph->forestIdx;
    label *rootIdx = rootGraph->rootIdx;

    label Delta = forestGraph->Delta;
    label n_edge = forestGraph->n_edge;
    label n_vertex = forestGraph->n_vertex;

    label *color = static_calloc(label, n_vertex);
    label *colorOld = static_calloc(label, n_vertex);
    label *colorTree = static_malloc(label, n_edge);

    /***
     * 按照森林循环
     ***/
    for (int i_forest = 0; i_forest < Delta; i_forest++) {

        /***
         * 按照森林中的叶子循环
         ***/
        label n_leaf = forestIdx[i_forest + 1] - forestIdx[i_forest];
        if (n_leaf > 0) {
            label n_root = rootIdx[i_forest + 1] - rootIdx[i_forest];
            label *row = forestGraph->row + forestIdx[i_forest];
            label *col = forestGraph->col + forestIdx[i_forest];
            label *root = rootGraph->root + rootIdx[i_forest];

            for (int i_vertex = 0; i_vertex < n_vertex; i_vertex++) {
                colorOld[i_vertex] = i_vertex;
            }
            for (int i_root = 0; i_root < n_root; i_root++) {
                color[root[i_root]] = 0;
                colorOld[root[i_root]] = 0;
            }

            /***
             * 按照论文给出的算法将树填上6种颜色
             ***/
            bool sixColor = false;
            while (!sixColor) {
                sixColor = true;
                for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                    unsigned int colorIn = colorOld[col[i_leaf]];
                    unsigned int colorOut = colorOld[row[i_leaf]];
                    colorIn ^= colorOut;
                    colorIn ^= colorIn - 1;
                    unsigned int i_bit;
                    for (i_bit = 1; i_bit <= sizeof(unsigned int) * 8; i_bit++) {
                        if (!(colorIn >> i_bit)) {
                            break;
                        }
                    }
                    i_bit--;
                    colorOut = (i_bit << 1) | ((colorOut >> i_bit) & 1);
                    color[row[i_leaf]] = colorOut;
                    if (sixColor) {
                        if (colorOut > 5) {
                            sixColor = false;
                        }
                    }
                }
                label *colorTemp = colorOld;
                colorOld = color;
                color = colorTemp;
            }

            /***
             * 按照论文给出的算法将6种颜色缩减到3种颜色
             ***/
            for (int i_over_color = 5; i_over_color >= 3; i_over_color--) {
                for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                    color[row[i_leaf]] = colorOld[col[i_leaf]];
                }
                for (int i_root = 0; i_root < n_root; i_root++) {
                    color[root[i_root]] = 1 - colorOld[root[i_root]];
                }

                for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                    if (color[row[i_leaf]] == i_over_color) {
                        for (int i_color = 0; i_color < 3; i_color++) {
                            if ((color[col[i_leaf]] != i_color) &&
                                (colorOld[row[i_leaf]] != i_color)) {
                                color[row[i_leaf]] = i_color;
                                break;
                            }
                        }
                    }
                }
                label *colorTemp = colorOld;
                colorOld = color;
                color = colorTemp;
            }

            /***
             * 将树填色的结果存储
             * 注意，这里按边存储
             * 存储的是相应亲节点的颜色
             ***/
            label *colorTreeLocal = colorTree + forestIdx[i_forest];
            for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                colorTreeLocal[i_leaf] = colorOld[col[i_leaf]];
            }
        }
    }

    /***
     * 输出
     ***/
    ColorTree *colorTreeGraph = static_malloc(ColorTree, 1);
    colorTreeGraph->colorTree = colorTree;

    free(color);
    free(colorOld);

    return colorTreeGraph;
}

ColorTree *getColorTreeT(Forest *forestGraph, Root *rootGraph)
{
    label *forestIdx = forestGraph->forestIdx;
    label *rootIdx = rootGraph->rootIdx;

    label Delta = forestGraph->Delta;
    label n_edge = forestGraph->n_edge;
    label n_vertex = forestGraph->n_vertex;

    label *color = static_calloc(label, n_vertex);
    label *colorOld = static_calloc(label, n_vertex);
    label *colorTree = static_malloc(label, n_edge);

    /***
     * 按照森林循环
     ***/
    for (int i_forest = 0; i_forest < Delta; i_forest++) {

        /***
         * 按照森林中的叶子循环
         ***/
        label n_leaf = forestIdx[i_forest + 1] - forestIdx[i_forest];
        if (n_leaf > 0) {
            label n_root = rootIdx[i_forest + 1] - rootIdx[i_forest];
            label *row = forestGraph->row + forestIdx[i_forest];
            label *col = forestGraph->col + forestIdx[i_forest];
            label *root = rootGraph->root + rootIdx[i_forest];

            for (int i_vertex = 0; i_vertex < n_vertex; i_vertex++) {
                colorOld[i_vertex] = i_vertex;
            }
            for (int i_root = 0; i_root < n_root; i_root++) {
                color[root[i_root]] = 0;
                colorOld[root[i_root]] = 0;
            }

            /***
             * 按照论文给出的算法将树填上6种颜色
             ***/
            bool sixColor = false;
            while (!sixColor) {
                sixColor = true;
                for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                    unsigned int colorIn = colorOld[row[i_leaf]];
                    unsigned int colorOut = colorOld[col[i_leaf]];
                    colorIn ^= colorOut;
                    colorIn ^= colorIn - 1;
                    unsigned int i_bit;
                    for (i_bit = 1; i_bit <= sizeof(unsigned int) * 8; i_bit++) {
                        if (!(colorIn >> i_bit)) {
                            break;
                        }
                    }
                    i_bit--;
                    colorOut = (i_bit << 1) | ((colorOut >> i_bit) & 1);
                    color[col[i_leaf]] = colorOut;
                    if (sixColor) {
                        if (colorOut > 5) {
                            sixColor = false;
                        }
                    }
                }
                label *colorTemp = colorOld;
                colorOld = color;
                color = colorTemp;
            }

            /***
             * 按照论文给出的算法将6种颜色缩减到3种颜色
             ***/
            for (int i_over_color = 5; i_over_color >= 3; i_over_color--) {
                for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                    color[col[i_leaf]] = colorOld[row[i_leaf]];
                }
                for (int i_root = 0; i_root < n_root; i_root++) {
                    color[root[i_root]] = 1 - colorOld[root[i_root]];
                }

                for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                    if (color[col[i_leaf]] == i_over_color) {
                        for (int i_color = 0; i_color < 3; i_color++) {
                            if ((color[row[i_leaf]] != i_color) &&
                                (colorOld[col[i_leaf]] != i_color)) {
                                color[col[i_leaf]] = i_color;
                                break;
                            }
                        }
                    }
                }
                label *colorTemp = colorOld;
                colorOld = color;
                color = colorTemp;
            }

            /***
             * 将树填色的结果存储
             * 注意，这里按边存储
             * 存储的是相应亲节点的颜色
             ***/
            label *colorTreeLocal = colorTree + forestIdx[i_forest];
            for (int i_leaf = 0; i_leaf < n_leaf; i_leaf++) {
                colorTreeLocal[i_leaf] = colorOld[row[i_leaf]];
            }
        }
    }

    /***
     * 输出
     ***/
    ColorTree *colorTreeGraph = static_malloc(ColorTree, 1);
    colorTreeGraph->colorTree = colorTree;

    free(color);
    free(colorOld);

    return colorTreeGraph;
}
