#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "radix.h"
#include "loader.h"

static const char LINE_DELIM = '\n';
static const char WORD_DELIM = ' ';
static const char NOT_SPECIAL_CHAR = 'c';

static const size_t MAX_TILE_SIZE = 10;

static const unsigned int SEED = 11;

/**
Duplicate a string, allocating memory and copying the content.

Parameters
----------
src: The string to duplicate.

Returns
-------
A pointer to the duplicate of that string.
*/
char* strdup(const char* src){
	// Compute the size the string to duplicate
	size_t size;
	for(size = 0; src[size] != '\0'; size++);

	// Allocate memory
	char* dest = malloc(size + 1 * sizeof(char));
	if(!dest)
		return NULL;

	// Copy content
	strcpy(dest, src);

	return dest;
}

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
char*** load_grid_from_file(const char* fileName, size_t* gridSize){
	//Open file
	FILE* fp = fopen(fileName, "r");
	if(!fp)
		return NULL;

	// Set grid size
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
	char*** grid = malloc(*gridSize * sizeof(char**));
	if(!grid)
		return NULL;

	for(size_t i = 0; i < *gridSize; i++){
		grid[i] = malloc(*gridSize * sizeof(char*));
		if(!grid[i])
			return NULL;
	}

	//Fill grid
	rewind(fp);

	for(size_t i = 0; i < *gridSize; i++){
		for(size_t j = 0; j < *gridSize; j++){
			// Allocate string
			char* tmpData = malloc(MAX_TILE_SIZE * sizeof(char));
			if(!tmpData)
				return NULL;
			
			// Fill the string
			size_t k;
			read = getc(fp);
			for(k = 0; read != WORD_DELIM && read != LINE_DELIM; k++){
				if(read == EOF)
					return NULL;

				//fprintf(stderr, "grid[%ld][%ld][%ld] = %c\n", i, j, k, read);
				tmpData[k] = read;

				read = getc(fp);
			}

			// Add terminal caracter at the end of the string
			tmpData[k] = '\0';

			// Use only the right amount of memory
			grid[i][j] = strdup(tmpData);
			free(tmpData);

			if(!grid[i][j])
				return NULL;
		}

		if(read != LINE_DELIM)
			return NULL;

	}

	fclose(fp);

	return grid;
}

/**
Load a radix dictionary from a file.

Parameters
----------
fileName: The name of the file containing the representation of the dictionary 
		  to load.
random: A boolean indicating if the words have to be inserted in the dictionary
		in a random order. If false, the words will be inserted in the same
		order as in the file.

Returns
-------
The loaded grid, a matrix of strings.
*/
RadixDic* load_dic_from_file(const char* fileName, bool random){
	//Open file
	FILE* fp = fopen(fileName, "r");
	if(!fp){
		fprintf(stderr, "fail open dic\n");
		return NULL;
	}

	RadixDic* dic = create_empty_dictionnary();

	srand(SEED);

	// To not be EOF
	char read = NOT_SPECIAL_CHAR;

	size_t dicSize = 0;
	size_t nbReadLine = 0;
	size_t maxReadLine = 0;
	while(read != EOF){
		nbReadLine++;
		if(read == '\n'){
			if(nbReadLine > maxReadLine)
				maxReadLine = nbReadLine;
			nbReadLine = 0;
			dicSize++;
		}
		read = fgetc(fp);
	}
	
	dicSize++;
	rewind(fp);
	read = NOT_SPECIAL_CHAR;

	char** buffer;
	if(random){
		buffer = malloc(dicSize * sizeof(char*));
		if(!buffer)
			return NULL;
	}

	size_t nbWords;
	for(nbWords = 0; read != EOF; nbWords++){
		char* tmpData = malloc(maxReadLine + 1 * sizeof(char));
		if(!tmpData)
			return NULL;

		read = fgetc(fp);
		if(read == EOF){
			free(tmpData);
			break;
		}

		size_t i;
		for(i = 0; read != LINE_DELIM && read != EOF; i++){
			tmpData[i] = read;
			read = fgetc(fp);
		}
		tmpData[i] = '\0';

		// Use only the right amount of memory
		char* data = strdup(tmpData);
		free(tmpData);

		if(!data)
			return NULL;

		if(random){
			buffer[nbWords] = data;
		}

		else{
			//fprintf(stderr, "inserted = %s\n", data);
			/*
			if(strcmp(data, "G") == 0)
				fprintf(stderr, "insert G\n");
				*/

			insert(dic, data, data);
		}
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

	fclose(fp);
	free(buffer);

	return dic;
}