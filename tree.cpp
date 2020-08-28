#include <cstring>
#include "tree.hpp"

Root *getRoot(Forest *forest, const label n_vertex)
{
    label *forestIdx = forest->forestIdx;
    label Delta = forest->Delta;

    size_t bitmapCount = (n_vertex / 8 + 1) * sizeof(unsigned char);
    unsigned char *rowBitmap = static_malloc(unsigned char, bitmapCount);
    unsigned char *colBitmap = static_malloc(unsigned char, bitmapCount);

    Root *root = static_malloc(Root, 1);
    root->rootIdx = static_malloc(label, Delta + 1);
    root->rootIdx[0] = 0;
    root->Delta = Delta;
    for (int i = 0; i < Delta; i++) {
        root->rootIdx[i + 1] = root->rootIdx[i];

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
            root->rootIdx[i + 1] += rootCnt;
        }
    }

    root->root = static_malloc(label, root->rootIdx[Delta]);
    {
        int i = 0;
        for (int j = 0; j < n_vertex; j++) {
            if (colBitmap[j / 8] & (1 << (j % 8))) {
                root->root[i] = j;
                i++;
            }
        }
    }

    free(rowBitmap);
    free(colBitmap);

    return root;
}