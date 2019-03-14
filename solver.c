#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "radix.h"
#include "solver.h"

static const char LINE_DELIM = '\n';
static const char WORD_DELIM = ' ';
static const char STRING_WORD_DELIM[2] = " ";

static const size_t MAX_WORD_LENGTH = 30;

char* strdup(const char* src){
	size_t size;
	for(size = 0; src[size] != '\0'; size++);

	char* dest = malloc(size + 1 * sizeof(char));
	if(!dest)
		return NULL;

	strcpy(dest, src);

	return dest;
}

char*** load_grid_from_file(const char* fileName, size_t maxTileSize, 
								   size_t* gridSize){
	//Open file
	FILE* fp = fopen(fileName, "r");
	if(!fp)
		return NULL;

	//Check grid size
	*gridSize = 1;
	char read = fgetc(fp);
	
	while(read != LINE_DELIM){
		
		if(read == WORD_DELIM)
			(*gridSize)++;
		read = fgetc(fp);
	}

	fprintf(stderr, "size got\n");
	//Allocate grid
	char*** grid = malloc(sizeof(char**));
	if(!grid)
		return NULL;

	for(size_t i = 0; i < *gridSize; i++){
		grid[i] = malloc(sizeof(char*));
		if(!grid[i])
			return NULL;
	}

	fprintf(stderr, "allocated\n");
	//Fill grid
	rewind(fp);

	for(size_t i = 0; i < *gridSize; i++){
		char line[*gridSize * (maxTileSize + 1) + 1];
		
		if(!fgets(line, *gridSize * (maxTileSize + 1), fp))
			return NULL;

		char* token = strtok(line, STRING_WORD_DELIM);
		for(size_t j = 0; j < *gridSize; j++){
			if(!token)
				return NULL;

			grid[i][j] = strdup(token);

			token = strtok(NULL, STRING_WORD_DELIM);
		}
	}

	fprintf(stderr, "filled\n");
}

RadixDic* load_dic_from_file(const char* fileName){
	//Open file
	FILE* fp = fopen(fileName, "r");
	if(!fp)
		return NULL;

	RadixDic* dic = create_empty_dictionnary();

	char read[MAX_WORD_LENGTH];

	while(fgets(read, MAX_WORD_LENGTH, fp)){
		char* data = strdup(read);
		if(!data)
			return NULL;
		
		insert(dic, data, data);
	}

	return dic;
}

static size_t* size_t_memdup(size_t* src, size_t length){

	size_t* dest = malloc(length * sizeof(size_t));

	if(!dest)
		return NULL;

	return (size_t*)memcpy(dest, src, length);
}

static size_t index_1D(size_t firstIndex, size_t secondIndex, size_t length){

	return firstIndex * length + secondIndex;
}

bool solve_recursive(char*** grid, size_t gridLength, size_t i, size_t j, 
							Node* currNode, RadixDic* foundDic, 
							size_t* oldPassedThrough, size_t iteration, 
							size_t maxIteration, size_t maxWordLength){

	if(iteration > maxIteration)
		return true;

	size_t tile = index_1D(i, j, gridLength);
	for(size_t i = 0; i < maxIteration; i++){
		if(tile == oldPassedThrough[i])
			return true;
	}

	size_t* passedThrough = size_t_memdup(oldPassedThrough, maxWordLength);
	if(!passedThrough)
		return false;

	passedThrough[iteration] = index_1D(i, j, gridLength);

	Node* nextNode = currNode;
	for(size_t k = 0; grid[i][j][k] != '\0'; k++){
		nextNode = next_node(nextNode, grid[i][j][k]);
	}

	if(!nextNode)
		return true;

	if(is_terminal(nextNode)){
		char* word = (char*)get_node_data(nextNode);
		char* key = strdup(word);
		char* data = strdup(word);

		insert(foundDic, data, data);

		free(key);
	}

	if(i > 0)
		if(!solve_recursive(grid, gridLength, i - 1, j, nextNode, foundDic, 
						passedThrough, iteration + 1, maxIteration, 
						maxWordLength))
			return false;
	
	if(i < gridLength - 1)
		if(!solve_recursive(grid, gridLength, i + 1, j, nextNode, foundDic, 
						passedThrough, iteration + 1, maxIteration, 
						maxWordLength))
			return false;

	if(j > 0)
		if(!solve_recursive(grid, gridLength, i, j - 1, nextNode, foundDic, 
						passedThrough, iteration + 1, maxIteration, 
						maxWordLength))
			return false;
	
	if(j < gridLength - 1)
		if(!solve_recursive(grid, gridLength, i, j + 1, nextNode, foundDic, 
						passedThrough, iteration + 1, maxIteration, 
						maxWordLength))
			return false;

	if(i > 0 && j > 0)
		if(!solve_recursive(grid, gridLength, i - 1, j - 1, nextNode, foundDic, 
						passedThrough, iteration + 1, maxIteration, 
						maxWordLength))
			return false;

	if(i > 0 && j < gridLength - 1)
		if(!solve_recursive(grid, gridLength, i - 1, j + 1, nextNode, foundDic, 
						passedThrough, iteration + 1, maxIteration, 
						maxWordLength))
			return false;

	if(i < gridLength - 1 && j > 0)
		if(!solve_recursive(grid, gridLength, i + 1, j - 1, nextNode, foundDic, 
						passedThrough, iteration + 1, maxIteration, 
						maxWordLength))
			return false;

	if(i < gridLength - 1 && j < gridLength - 1)
		if(!solve_recursive(grid, gridLength, i + 1, j + 1, nextNode, foundDic, 
						passedThrough, iteration + 1, maxIteration, 
						maxWordLength))
			return false;

	free(passedThrough);

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

	assert(wordDic);
	assert(returnLength);

	RadixDic* foundDic = create_empty_dictionnary();

	if(!foundDic)
		return NULL;

	size_t* passedThrough = malloc(get_dic_max_length(wordDic) * sizeof(size_t));

	if(!passedThrough)
		return NULL;

	// Prevent passed through to be intialized with valid indices.
	for(size_t i = 0; i < get_dic_max_length(wordDic); i++){
		passedThrough[i] = gridLength * gridLength + 1;
	}

	fprintf(stderr, "initialized\n");

	for(size_t i = 0; i < gridLength; i++){
		for(size_t j = 0; j < gridLength; j++){
			fprintf(stderr, "iteration %ld %ld\n", i, j);
			if(!solve_recursive(grid, gridLength, i, j, get_root(wordDic), 
								foundDic, passedThrough, 0, 
								get_dic_size(wordDic),
								get_dic_max_length(wordDic))){

				delete_dictionnary(foundDic);
				free(passedThrough);
				return NULL;
			}
		}
	}

	char** words = (char**)(get_all_data(foundDic));
	size_t foundDicSize = get_dic_size(foundDic);
	for(size_t i = 0; i < foundDicSize; i++)
		words[i] = strdup(words[i]);

	delete_dictionnary(foundDic);
	free(passedThrough);

	return words;
}

/*
int main(){
   // attention si solve return NULL display error sur stderr.
}
*/
