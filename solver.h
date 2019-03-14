#ifndef SOLVER_H
#define SOLVER_H

#include "radix.h"
#include <stddef.h>

char* strdup(const char* src);

char*** load_grid_from_file(const char* fileName, size_t maxTileSize, 
								   size_t* gridSize);

RadixDic* load_dic_from_file(const char* fileName);

static size_t* size_t_memdup(size_t* src, size_t length);

bool solve_recursive(char*** grid, size_t gridLength, size_t i, size_t j, 
							Node* currNode, RadixDic* foundDic, 
							size_t* oldPassedThrough, size_t iteration, 
							size_t maxIteration, size_t maxWordLength);

char** solve(char*** grid, size_t gridLength, RadixDic* wordDic, 
			 size_t* returnLength);

#endif