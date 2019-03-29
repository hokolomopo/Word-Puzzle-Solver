#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "radix.h"
#include "wordPuzzle.h"
#include "loader.h"

/**
Returns a representation of a position in the grid.

Parameters
----------
firstIndex: The first index of the position.
secondIndex: The second index of the position.
length: The length of the grid.

Returns
-------
A unique representation of that position.
*/
static size_t index_1D(size_t firstIndex, size_t secondIndex, size_t length){

	return firstIndex * length + secondIndex;
}

/**
Retrieve all the words contained in a given dictionnary appearing in a given
grid and strating at a given position of that grid. The found words are
inserted in foundDic.

Parameters
----------
grid: The grid in which to find the words.
gridLength: The size of the grid.
i: The first index of the starting position.
j: The second index of the starting position.
currNode: argument used for recursion representing the current positon in the 
		  dictionnary radix tree, should be initialised with the root of the 
		  word dictionnary.
foundDic: A dictionnary in which to insert found words.
passedThrough: An array storing all the already visited positions. Should be
			   initialized empty.
notPassedValue: An integer not representing any valid position used as flag.
iteration: The current iteration, should be initialized at 0.
maxWordLength: The maximum length of a word in the word dictionnary.

Returns
-------
The data associated to the key, NULL of the key is not in the dictionnary.
*/
static void solve_recursive(char*** grid, size_t gridLength, size_t i, size_t j, 
					 		Node* currNode, RadixDic* foundDic, 
					 		size_t* passedThrough, size_t notPassedValue, 
					 		size_t iteration, size_t maxWordLength, bool debug){
	//fprintf(stderr, "i = %ld, j = %ld\n", i, j);

	// We are searching for words longer that the longest word in the 
	// dictionnary -> stop searching.
	if(iteration >= maxWordLength)
		return;

	if(debug)
		fprintf(stderr, "iteration = %ld\n", iteration);

	// Check if we already visited that position.
	size_t tile = index_1D(i, j, gridLength);
	for(size_t i = 0; i < maxWordLength; i++){
		if(tile == passedThrough[i])
			return;
	}

	// Indicate we have visited that position
	passedThrough[iteration] = index_1D(i, j, gridLength);

	// Traverse graph according to symbols in the gird.
	Node* nextNode = currNode;
	for(size_t k = 0; grid[i][j][k] != '\0'; k++){
		if(!nextNode)
			break;
		nextNode = next_node(nextNode, grid[i][j][k], debug);
		if(debug && nextNode)
			fprintf(stderr, "found %c\n", get_node_symbol(nextNode));
	}

	// If nextNode is NULL, no words in the dictionnary correspond to current 
	// word found in the grid
	if(!nextNode){
		passedThrough[iteration] = notPassedValue;
		return;
	}

	// If nextNode is terminal, we have found a valid word.
	if(is_terminal(nextNode)){
		char* data = strdup((char*)get_node_data(nextNode));

		if(debug)
			fprintf(stderr, "inserted %s, symbol = %c\n", data, get_node_symbol(nextNode));
		insert(foundDic, data, data);
	}

	//Try to extend the current word with all adjacent tiles.
	if(i > 0)
		solve_recursive(grid, gridLength, i - 1, j, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength, debug);
	
	if(i < gridLength - 1)
		solve_recursive(grid, gridLength, i + 1, j, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength, debug);

	if(j > 0)
		solve_recursive(grid, gridLength, i, j - 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength, debug);
	
	if(j < gridLength - 1)
		solve_recursive(grid, gridLength, i, j + 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength, debug);

	if(i > 0 && j > 0)
		solve_recursive(grid, gridLength, i - 1, j - 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength, debug);

	if(i > 0 && j < gridLength - 1)
		solve_recursive(grid, gridLength, i - 1, j + 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength, debug);

	if(i < gridLength - 1 && j > 0)
		solve_recursive(grid, gridLength, i + 1, j - 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength, debug);

	if(i < gridLength - 1 && j < gridLength - 1)
		solve_recursive(grid, gridLength, i + 1, j + 1, nextNode, foundDic, 
						passedThrough, notPassedValue, iteration + 1, 
						maxWordLength, debug);

	// Indicate we have finished visiting this position to let other recursive
	// calls pass thorugh it.
	passedThrough[iteration] = notPassedValue;
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
	/*
	if(is_key_in_dic(wordDic, "G"))
		fprintf(stderr, "G in wordDic\n");
		*/
	// Check if the grid is valid
	assert(grid);
	for(size_t i = 0; i < gridLength; i++){
		assert(grid[i]);
		for(size_t j = 0; j < gridLength; j++){
			assert(grid[i][j]);
		}
	}

	/*
	for(size_t i = 0;  i < gridLength; i++){
		for(size_t j = 0; j < gridLength; j++)
			fprintf(stderr, "grid[%ld][%ld] = %s\n", i, j, grid[i][j]);
	}
	*/

	// Check if the dictionnary and the returnLength pointers are valid
	assert(wordDic);
	assert(returnLength);

	// Create a dictionnary in which to insert the found words.
	RadixDic* foundDic = create_empty_dictionnary();

	//Check allocation
	if(!foundDic)
		return NULL;

	// Initialize the array of already passed through positions
	size_t maxWordLength = get_dic_max_length(wordDic);
	size_t* passedThrough = malloc(maxWordLength * sizeof(size_t));

	// Check allocation
	if(!passedThrough)
		return NULL;

	// notPassedValue is a position that is invalid w.r.t the grid.
	size_t notPassedValue = gridLength * gridLength + 1;
	
	// Prevent passed through to be intialized with valid indices.
	for(size_t i = 0; i < maxWordLength; i++){
		passedThrough[i] = notPassedValue;
	}

	// Solve the grid
	bool debug = false;
	for(size_t i = 0; i < gridLength; i++){
		for(size_t j = 0; j < gridLength; j++){
			debug = false;
			/*
			if(strcmp(grid[i][j], "G") == 0 && i == 2 && j == 0)
				debug = true;
				*/
			solve_recursive(grid, gridLength, i, j, get_root(wordDic), 
								foundDic, passedThrough, notPassedValue, 0, 
								maxWordLength, debug);
			if(debug && is_key_in_dic(foundDic, "G"))
				fprintf(stderr, "G in foundDic after solve\n");
		}
	}

	/*
	if(is_key_in_dic(foundDic, "G"))
		fprintf(stderr, "G in foundDic\n");
		*/
	// Retrieve the words from the dictionnary of found words.
	char** words = (char**)(get_all_data(foundDic));
	*returnLength = get_dic_size(foundDic);
	for(size_t i = 0; i < *returnLength; i++)
		words[i] = strdup(words[i]);

	// Free memory
	delete_dictionnary(foundDic);
	free(passedThrough);

	return words;
}
