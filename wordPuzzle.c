#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "radix.h"
#include "wordPuzzle.h"

static const char LINE_DELIM = '\n';
static const char WORD_DELIM = ' ';
static const char NOT_SPECIAL_CHAR = 'c';

static const size_t MAX_WORD_LENGTH = 30;
static const size_t SIZE_STEP_DIC = 700;

static const size_t MAX_TILE_SIZE = 10;

static const unsigned int SEED = 11;

char* strdup(const char* src){
	size_t size;
	for(size = 0; src[size] != '\0'; size++);

	char* dest = malloc(size + 1 * sizeof(char));
	if(!dest)
		return NULL;

	strcpy(dest, src);

	return dest;
}

char*** load_grid_from_file(const char* fileName, size_t* gridSize){
	//Open file
	FILE* fp = fopen(fileName, "r");
	if(!fp)
		return NULL;

	//Check grid size
	*gridSize = 1;
	char read = fgetc(fp);
	
	while(read != LINE_DELIM){
		if(read == EOF)
			return NULL;

		if(read == WORD_DELIM)
			(*gridSize)++;
		read = fgetc(fp);
	}

	//Allocate grid
	char*** grid = malloc(sizeof(char**));
	if(!grid)
		return NULL;

	for(size_t i = 0; i < *gridSize; i++){
		grid[i] = malloc(sizeof(char*));
		if(!grid[i])
			return NULL;
	}

	//Fill grid
	rewind(fp);

	//To not be a special char
	read = NOT_SPECIAL_CHAR;
	for(size_t i = 0; i < *gridSize; i++){
		for(size_t j = 0; j < *gridSize; j++){
			grid[i][j] = malloc(MAX_TILE_SIZE * sizeof(char));
			if(!grid[i][j])
				return NULL;
			
			size_t k ;
			read = getc(fp);
			for(k = 0; read != WORD_DELIM && read != LINE_DELIM; k++){
				if(read == EOF)
					return NULL;

				grid[i][j][k] = read;

				read = getc(fp);
			}
			grid[i][j][k] = '\0';
		}

		if(read != LINE_DELIM)
			return NULL;

	}

	return grid;
}

RadixDic* load_dic_from_file(const char* fileName, bool random){
	//Open file
	FILE* fp = fopen(fileName, "r");
	if(!fp)
		return NULL;

	RadixDic* dic = create_empty_dictionnary();

	srand(SEED);

	size_t bufferSize = SIZE_STEP_DIC;
	char** buffer = malloc(bufferSize * sizeof(char*));
	if(!buffer)
		return NULL;

	// To not be EOF
	char read = NOT_SPECIAL_CHAR;

	size_t nbWords;
	for(nbWords = 0; read != EOF; nbWords++){
		char* data = malloc(MAX_WORD_LENGTH + 1 * sizeof(char));
		if(!data)
			return NULL;

		read = fgetc(fp);
		size_t i;
		for(i = 0; read != LINE_DELIM && read != EOF; i++){
			data[i] = read;
			read = fgetc(fp);
		}
		data[i] = '\0';

		if(random){
			if(nbWords >= bufferSize){
				bufferSize += SIZE_STEP_DIC;
				if(!realloc(buffer, bufferSize * sizeof(char*)))
					return NULL;

			}
			buffer[nbWords] = data;
		}
		else
			insert(dic, data, data);
	}

	if(random){
		size_t nbWordsToInsert = nbWords;
		
		size_t indicesToInsert[nbWords];

		for(size_t i = 0; i < nbWords; i++){
			indicesToInsert[i] = i;
		}

		size_t insertIndex;
		for(size_t i = 0; i < nbWords; i++){
			insertIndex = rand()%nbWordsToInsert;
			insert(dic, buffer[indicesToInsert[insertIndex]],
				   buffer[indicesToInsert[insertIndex]]);
			indicesToInsert[insertIndex] = indicesToInsert[--nbWordsToInsert];
		}
	}

	return dic;
}

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

bool solve_recursive(char*** grid, size_t gridLength, size_t i, size_t j, 
					 Node* currNode, RadixDic* foundDic, size_t* passedThrough, 
					 size_t notPassedValue, size_t iteration, 
					 size_t maxWordLength){

	if(iteration > maxWordLength)
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

	assert(wordDic);
	assert(returnLength);

	RadixDic* foundDic = create_empty_dictionnary();

	if(!foundDic)
		return NULL;

	size_t* passedThrough = malloc(get_dic_max_length(wordDic) * sizeof(size_t));

	if(!passedThrough)
		return NULL;

	size_t notPassedValue = gridLength * gridLength + 1;
	
	// Prevent passed through to be intialized with valid indices.
	for(size_t i = 0; i < get_dic_max_length(wordDic); i++){
		passedThrough[i] = notPassedValue;
	}

	for(size_t i = 0; i < gridLength; i++){
		for(size_t j = 0; j < gridLength; j++){
			if(!solve_recursive(grid, gridLength, i, j, get_root(wordDic), 
								foundDic, passedThrough, notPassedValue, 0, 
								get_dic_max_length(wordDic))){

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
