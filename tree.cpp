#include <cstring>
#include "tree.hpp"

Root *getRoot(Forest *forest)
{
    label *forestIdx = forest->forestIdx;
    label Delta = forest->Delta;
    label n_edge = forest->n_edge;
    label n_vertex = forest->n_vertex;

    size_t bitmapCount = (n_vertex / 8 + 1) * sizeof(unsigned char);
    unsigned char *rowBitmap = static_malloc(unsigned char, bitmapCount);
    unsigned char *colBitmap = static_malloc(unsigned char, bitmapCount);

    label *rootIdx = static_malloc(label, Delta + 1);
    rootIdx[0] = 0;
    for (int i = 0; i < Delta; i++) {
        rootIdx[i + 1] = rootIdx[i];

        label n_leaf = forestIdx[i + 1] - forestIdx[i];
        if (n_leaf > 0) {
            memset(rowBitmap, 0, bitmapCount * sizeof(unsigned char));
            memset(colBitmap, 0, bitmapCount * sizeof(unsigned char));
            label *row = forest->row + forestIdx[i];
            label *col = forest->col + forestIdx[i];
            int rootCnt = 0;

            for (int j = 0; j < n_leaf; j++) {
                rowBitmap[row[j] / 8] |= 1 << (row[j] % 8);
                colBitmap[col[j] / 8] |= 1 << (col[j] % 8);
            }

            for (int j = 0; j < n_vertex / 8 + 1; j++) {
                rowBitmap[j] ^= colBitmap[j];
                colBitmap[j] &= rowBitmap[j];
                if (colBitmap[j]) {
                    unsigned char charCnt = colBitmap[j];
                    charCnt = ((charCnt >> 1) & 0x55) + (charCnt & 0x55);
                    charCnt = ((charCnt >> 2) & 0x33) + (charCnt & 0x33);
                    charCnt = (charCnt >> 4) + (charCnt & 0x0f);
                    rootCnt += (int) charCnt;
                }
            }
            rootIdx[i + 1] += rootCnt;
        }
    }

    label *rootVal = static_malloc(label, rootIdx[Delta]);
    {
        int i = 0;
        for (int j = 0; j < n_vertex; j++) {
            if (colBitmap[j / 8] & (1 << (j % 8))) {
                rootVal[i] = j;
                i++;
            }
        }
    }

    Root *root = static_malloc(Root, 1);
    root->root = rootVal;
    root->rootIdx = rootIdx;
    root->Delta = Delta;
    root->n_edge = n_edge;
    root->n_vertex = n_vertex;

    free(rowBitmap);
    free(colBitmap);

    return root;
}