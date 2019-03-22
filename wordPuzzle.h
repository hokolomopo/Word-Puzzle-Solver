#ifndef WORD_PUZZLE_H
#define WORD_PUZZLE_H

#include "radix.h"
#include <stddef.h>

char* strdup(const char* src);

char*** load_grid_from_file(const char* fileName, size_t* gridSize);

RadixDic* load_dic_from_file(const char* fileName, bool random);

bool solve_recursive(char*** grid, size_t gridLength, size_t i, size_t j, 
							Node* currNode, RadixDic* foundDic, 
							size_t* passedThrough, size_t notPassedValue, 
							size_t iteration, size_t maxWordLength);

char** solve(char*** grid, size_t gridLength, RadixDic* wordDic, 
			 size_t* returnLength);

#endif