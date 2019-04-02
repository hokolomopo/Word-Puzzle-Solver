#ifndef WORD_PUZZLE_H
#define WORD_PUZZLE_H

#include "radix.h"
#include <stddef.h>

/**
 * Solve a grid.
 *
 * Parameters
 * ----------
 * grid: The grid to solve. The grid must be squared a matrix of strings.
 * length: The size of the matrix.
 * wordDic: A dictionary containing all the existing words.
 * returnLength: A pointer to a size_t that will be modified by the function to
 *				 be the size of the returned array.
 *
 * Returns
 * -------
 * An array of strings containing the words found. NULL in case no word found or
 * allocation failure.
 */
char** solve(const char*** grid, size_t gridLength, const RadixDic* wordDic, 
			 size_t* returnLength);

#endif