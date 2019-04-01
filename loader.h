#ifndef LOADER_H
#define LOADER_H

#include "radix.h"
#include <stddef.h>

/**
Duplicate a string, allocating memory and copying the content.

Parameters
----------
src: The string to duplicate.

Returns
-------
A pointer to the duplicate of that string.
*/
char* strdup(const char* src);

/**
Load a grid from a file.

Parameters
----------
fileName: The name of the file containing the representation of the grid to
		  load.
gridSize: A pointer to a size_t that will be set to the size of the grid loaded.

Returns
-------
The loaded grid, a matrix of strings.
*/
char*** load_grid_from_file(const char* fileName, size_t* gridSize);

RadixDic* load_dic_from_file(const char* fileName, bool random);

#endif