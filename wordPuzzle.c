#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "radix.h"
#include "wordPuzzle.h"
#include "loader.h"

/**
 * Returns a representation of a position in the grid.
 *
 * Parameters
 * ----------
 * firstIndex: The first index of the position.
 * secondIndex: The second index of the position.
 * length: The length of the grid.
 *
 * Returns
 * -------
 * A unique representation of that position.
 */
static size_t index_1D(size_t firstIndex, size_t secondIndex, size_t length){

	return firstIndex * length + secondIndex;
}

/**
 * Retrieve all the words contained in a given dictionary appearing in a given
 * grid and starting at a given position of that grid. The found words are
 * inserted in foundDic.
 *
 * Parameters
 * ----------
 * grid: The grid in which to find the words.
 * gridLength: The size of the grid.
 * i: The first index of the starting position.
 * j: The second index of the starting position.
 * currNode: argument used for recursion representing the current positon in the 
 * 			 dictionary radix tree, should be initialised with the root of the 
 *		     word dictionary.
 * foundDic: A dictionary in which to insert found words.
 * passedThrough: An array storing all the already visited positions. Should be
 *			      initialized empty.
 * notPassedValue: An integer not representing any valid position used as flag.
 * iteration: The current iteration, should be initialized at 0.
 * maxWordLength: The maximum length of a word in the word dictionary.
 *
 * Returns
 * -------
 * A boolean indicating if the executing went well.
 */
static bool solve_recursive(const char*** grid, size_t gridLength, size_t i, 
							size_t j, const Node* currNode, RadixDic* foundDic, 
							size_t* passedThrough, size_t notPassedValue, 
							size_t iteration, size_t maxWordLength){
	
	// We are searching for words longer that the longest word in the 
	// dictionary -> stop searching.
	if(iteration >= maxWordLength)
		return true;

	// Check if we already visited that position.
	size_t tile = index_1D(i, j, gridLength);
	for(size_t i = 0; i < maxWordLength; i++){
		if(tile == passedThrough[i])
			return true;
	}

	// Indicate we have visited that position
	passedThrough[iteration] = index_1D(i, j, gridLength);

	// Traverse graph according to symbols in the gird.
	const Node* nextNode = currNode;
	for(size_t k = 0; grid[i][j][k] != '\0'; k++){
		if(!nextNode)
			break;
		
		nextNode = next_node(nextNode, grid[i][j][k]);
	}

	// If nextNode is NULL, no words in the dictionary correspond to current 
	// word found in the grid
	if(!nextNode){
		passedThrough[iteration] = notPassedValue;
		return true;
	}

	// If nextNode is terminal, we have found a valid word.
	if(is_terminal(nextNode)){
		char* data = strdup((char*)get_node_data(nextNode));

		if(!data)
			return false;

		if(!insert(foundDic, data, data)){
			free(data);
			return false;
		}
	}

	//Try to extend the current word with all adjacent tiles.
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

	// Indicate we have finished visiting this position to let other recursive
	// calls pass through it.
	passedThrough[iteration] = notPassedValue;

	return true;
}

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
			 size_t* returnLength){

	// Check if the grid is valid
	assert(grid);
	for(size_t i = 0; i < gridLength; i++){
		assert(grid[i]);
		for(size_t j = 0; j < gridLength; j++)
			assert(grid[i][j]);
	}

	// Check if the dictionary and the returnLength pointers are valid
	assert(wordDic);
	assert(returnLength);

	// Create a dictionary in which to insert the found words.
	RadixDic* foundDic = create_empty_dictionary();

	//Check allocation
	if(!foundDic)
		return NULL;

	// Initialize the array of already passed through positions
	size_t maxWordLength = get_dic_max_length(wordDic);
	size_t* passedThrough = malloc(maxWordLength * sizeof(size_t));

	// Check allocation
	if(!passedThrough){
		delete_dictionary(foundDic);
		return NULL;
	}

	// notPassedValue is a position that is invalid w.r.t the grid.
	size_t notPassedValue = gridLength * gridLength + 1;
	
	// Prevent passed through to be intialized with valid indices.
	for(size_t i = 0; i < maxWordLength; i++)
		passedThrough[i] = notPassedValue;

	// Solve the grid
	for(size_t i = 0; i < gridLength; i++){
		for(size_t j = 0; j < gridLength; j++)
			if(!solve_recursive(grid, gridLength, i, j, get_root(wordDic), 
							foundDic, passedThrough, notPassedValue, 0, 
							maxWordLength)){

				delete_dictionary(foundDic);
				free(passedThrough);
				return NULL;
		}
	}

	// Retrieve the words from the dictionary of found words.
	char** words = (char**)(get_all_data(foundDic));
	
	if(!words){
		delete_dictionary(foundDic);
		free(passedThrough);
		return NULL;
	}

	*returnLength = get_dic_size(foundDic);
	
	for(size_t i = 0; i < *returnLength; i++){
		words[i] = strdup(words[i]);

		if(!words[i]){
			for(size_t j = 0; j < i; j++)
				free(words[i]);
			free(words);

			delete_dictionary(foundDic);
			free(passedThrough);

			return NULL;
		}
	}

	// Free memory
	delete_dictionary(foundDic);
	free(passedThrough);

	return words;
}
