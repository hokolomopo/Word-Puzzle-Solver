#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "radix.h"
#include "wordPuzzle.h"
#include "loader.h"

/*
static size_t* size_t_memdup(size_t* src, size_t length){

	size_t* dest = malloc(length * sizeof(size_t));

	if(!dest)
		return NULL;

	return (size_t*)memcpy(dest, src, length);
}
*/

static size_t index_1D(size_t firstIndex, size_t secondIndex, size_t length){

	return firstIndex * length + secondIndex;
}

// TODO: enlever les return true car plus de false
bool solve_recursive(char*** grid, size_t gridLength, size_t i, size_t j, 
					 Node* currNode, RadixDic* foundDic, size_t* passedThrough, 
					 size_t notPassedValue, size_t iteration, 
					 size_t maxWordLength){
	//fprintf(stderr, "i = %ld, j = %ld\n", i, j);

	if(iteration >= maxWordLength)
		return true;

	size_t tile = index_1D(i, j, gridLength);
	for(size_t i = 0; i < maxWordLength; i++){
		if(tile == passedThrough[i])
			return true;
	}

	passedThrough[iteration] = index_1D(i, j, gridLength);

	Node* nextNode = currNode;
	for(size_t k = 0; grid[i][j][k] != '\0'; k++){
		if(!nextNode)
			break;
		nextNode = next_node(nextNode, grid[i][j][k]);
	}

	if(!nextNode){
		passedThrough[iteration] = notPassedValue;
		return true;
	}

	if(is_terminal(nextNode)){
		char* word = (char*)get_node_data(nextNode);
		char* key = strdup(word);
		char* data = strdup(word);

		//fprintf(stderr, "inserted %s, symbol = %c\n", data, get_node_symbol(nextNode));
		insert(foundDic, data, data);

		free(key);
	}

	if(i > 0)
		if(!solve_recursive(grid, gridLength, i - 1, j, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength))
			return false;
	
	if(i < gridLength - 1)
		if(!solve_recursive(grid, gridLength, i + 1, j, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength))
			return false;

	if(j > 0)
		if(!solve_recursive(grid, gridLength, i, j - 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength))
			return false;
	
	if(j < gridLength - 1)
		if(!solve_recursive(grid, gridLength, i, j + 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength))
			return false;

	if(i > 0 && j > 0)
		if(!solve_recursive(grid, gridLength, i - 1, j - 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength))
			return false;

	if(i > 0 && j < gridLength - 1)
		if(!solve_recursive(grid, gridLength, i - 1, j + 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength))
			return false;

	if(i < gridLength - 1 && j > 0)
		if(!solve_recursive(grid, gridLength, i + 1, j - 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength))
			return false;

	if(i < gridLength - 1 && j < gridLength - 1)
		if(!solve_recursive(grid, gridLength, i + 1, j + 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength))
			return false;

	passedThrough[iteration] = notPassedValue;

	return true;
}

/**
Solve a grid.

Parameters
----------
grid: The grid to solve. The grid must be squared a matrix of strings.
length: The size of the matrix.
wordDic: A dictionnary containing all the existing words.
returnLength: A pointer to a size_t that will be modified by the function to be
			  the size of the returned array.

Returns
-------
An array of strings containing the words found. NULL in case no word found or
allocation failure.
*/
char** solve(char*** grid, size_t gridLength, RadixDic* wordDic, 
			 size_t* returnLength){

	assert(grid);
	for(size_t i; i < gridLength; i++){
		assert(grid[i]);
		for(size_t j; j < gridLength; j++){
			assert(grid[i][j]);
		}
	}

	/*
	for(size_t i = 0;  i < gridLength; i++){
		for(size_t j = 0; j < gridLength; j++)
			fprintf(stderr, "grid[%ld][%ld] = %s\n", i, j, grid[i][j]);
	}
	*/

	assert(wordDic);
	assert(returnLength);

	RadixDic* foundDic = create_empty_dictionnary();

	if(!foundDic)
		return NULL;

	size_t maxWordLength = get_dic_max_length(wordDic);
	size_t* passedThrough = malloc(maxWordLength * sizeof(size_t));

	if(!passedThrough)
		return NULL;

	size_t notPassedValue = gridLength * gridLength + 1;
	
	// Prevent passed through to be intialized with valid indices.
	for(size_t i = 0; i < maxWordLength; i++){
		passedThrough[i] = notPassedValue;
	}

	for(size_t i = 0; i < gridLength; i++){
		for(size_t j = 0; j < gridLength; j++){
			//fprintf(stderr, "%s\n", grid[i][j]);
			if(!solve_recursive(grid, gridLength, i, j, get_root(wordDic), 
								foundDic, passedThrough, notPassedValue, 0, 
								maxWordLength)){

				delete_dictionnary(foundDic);
				free(passedThrough);
				return NULL;
			}
		}
	}

	char** words = (char**)(get_all_data(foundDic));
	*returnLength = get_dic_size(foundDic);
	for(size_t i = 0; i < *returnLength; i++)
		words[i] = strdup(words[i]);

	delete_dictionnary(foundDic);
	free(passedThrough);

	return words;
}
