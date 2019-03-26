#ifndef LOADER_H
#define LOADER_H

#include "radix.h"
#include <stddef.h>

char* strdup(const char* src);

char*** load_grid_from_file(const char* fileName, size_t* gridSize);

RadixDic* load_dic_from_file(const char* fileName, bool random);

#endif