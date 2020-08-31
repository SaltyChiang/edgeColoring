#ifndef __TREE_HPP__
#define __TREE_HPP__

#include "macro.h"

Root *getRoot(Forest *forestGraph);
Root *getRootT(Forest *forestGraph);
ColorTree *getColorTree(Forest *forestGraph, Root *rootGraph);
ColorTree *getColorTreeT(Forest *forestGraph, Root *rootGraph);

#endif