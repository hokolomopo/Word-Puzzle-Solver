#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "radix.h"
#include "loader.h"

static const char LINE_DELIM = '\n';
static const char WORD_DELIM = ' ';
static const char NOT_SPECIAL_CHAR = 'c';

static const size_t MAX_TILE_SIZE = 10;

static const unsigned int SEED = 11;

/**
 * Duplicate a string, allocating memory and copying the content.
 *
 * Parameters
 * ----------
 * src: The string to duplicate.
 *
 * Returns
 * -------
 * A pointer to the duplicate of that string.
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
 * Free a grid filled intil first index i and second index j.
 *
 * Parameters
 * ----------
 * grid: The grid to free.
 * gridSize: The size of the grid.
 * i: The first index bound until which to grid has been filled.
 * j: The first second bound until which to grid has been filled.
 *
 * Returns
 * -------
 * void
 */
static void free_grid(char*** grid, size_t gridSize, size_t i, size_t j){

	// Free filled content
	for(size_t k = 0; k <= i; k++){
		for(size_t l = 0; l < j; l ++)
			free(grid[k][l]);
	}

	// Free the grid
	for(size_t k = 0; k < gridSize; k++)
		free(grid[k]);

	free(grid);
}

/**
 * Load a grid from a file.
 *
 * Parameters
 * ----------
 * fileName: The name of the file containing the representation of the grid to
 *		     load.
 * gridSize: A pointer to a size_t that will be set to the size of the grid 
 *           loaded.
 *
 * Returns
 * -------
 * The loaded grid, a matrix of strings or NULL if bad arguments have been
 * passed, the file is ill formated or allocation failed.
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
		if(read == EOF){
			fclose(fp);
			return NULL;
		}

		if(read == WORD_DELIM)
			(*gridSize)++;
		read = fgetc(fp);
	}

	//Allocate grid
	char*** grid = malloc(*gridSize * sizeof(char**));
	
	if(!grid){
		fclose(fp);
		return NULL;
	}

	for(size_t i = 0; i < *gridSize; i++){
		grid[i] = malloc(*gridSize * sizeof(char*));
		
		if(!grid[i]){
			fclose(fp);

			for(size_t j = 0; j < i; j++)
				free(grid[i]);
			free(grid);

			return NULL;
		}
	}

	//Fill grid
	rewind(fp);

	char tmpData[MAX_TILE_SIZE + 1];

	for(size_t i = 0; i < *gridSize; i++){
		for(size_t j = 0; j < *gridSize; j++){

			// Fill the string
			size_t k;
			read = getc(fp);
			for(k = 0; read != WORD_DELIM && read != LINE_DELIM; k++){
				
				if(read == EOF){
					fclose(fp);
					free_grid(grid, *gridSize, i, j);
					return NULL;
				}

				tmpData[k] = toupper(read);

				read = getc(fp);
			}

			// Add terminal caracter at the end of the string
			tmpData[k] = '\0';

			// Use only the needed amount of memory
			grid[i][j] = strdup(tmpData);
			if(!grid[i][j]){
				fclose(fp);
				free_grid(grid, *gridSize, i, j);
				return NULL;
			}
		}

		if(read != LINE_DELIM){
			fclose(fp);
			free_grid(grid, *gridSize, i, *gridSize);
			return NULL;
		}
	}

	fclose(fp);

	return grid;
}

/**
 * Load a radix dictionary from a file.
 *
 * Parameters
 * ----------
 * fileName: The name of the file containing the representation of the 
 *           dictionary to load.
 * random: A boolean indicating if the words have to be inserted in the 
 *		   dictionary in a random order. If false, the words will be inserted 
 *		   in the same order as in the file.
 *
 * Returns
 * -------
 * The loaded grid, a matrix of strings or NULL if bad arguments have been
 * passed, the file is ill formated or allocation failed.
 */
RadixDic* load_dic_from_file(const char* fileName, bool random){
	
	//Open file
	FILE* fp = fopen(fileName, "r");
	if(!fp)
		return NULL;

	RadixDic* dic = create_empty_dictionary();

	if(!dic){
		fclose(fp);
		return NULL;
	}

	srand(SEED);

	// To not be EOF
	char read = NOT_SPECIAL_CHAR;

	size_t dicSize = 0;
	size_t nbReadLine = 0;
	size_t maxReadLine = 0;

	// Compute the number of line and the maximum line length
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

	// If we insert the word in random order insert them in a buffer.
	char** buffer = NULL;
	if(random){
		buffer = malloc(dicSize * sizeof(char*));
		
		if(!buffer){
			fclose(fp);
			delete_dictionary(dic);
			return NULL;
		}
	}

	char tmpData[maxReadLine + 1];

	// Read the dictionary
	size_t nbWords;
	for(nbWords = 0; read != EOF; nbWords++){
		
		read = fgetc(fp);
		
		if(read == EOF)
			break;

		// Read current word
		size_t i;
		for(i = 0; read != LINE_DELIM && read != EOF; i++){
			tmpData[i] = toupper(read);
			read = fgetc(fp);
		}

		tmpData[i] = '\0';

		// Use only the right amount of memory
		char* data = strdup(tmpData);

		if(!data){
			fclose(fp);
			delete_dictionary(dic);

			if(random)
				for(size_t i = 0; i < nbWords; i++)
					free(buffer[i]);

			free(buffer);

			return NULL;
		}

		// If we insert the words in random order insert the foudn word in a
		// buffer
		if(random){
			buffer[nbWords] = data;
		}

		// Otherwise insert the word directly in the dictionary.
		else{
			if(!insert(dic, data, data)){
				fclose(fp);
				delete_dictionary(dic);

				if(random)
					for(size_t i = 0; i <= nbWords; i++)
						free(buffer[i]);

				free(buffer);

				return NULL;
			}
		}
	}

	// If we insert the words in random order, insert the words stored in a
	// buffer in the dictionary.
	if(random){
		size_t nbWordsToInsert = nbWords;
		
		size_t insertIndex;
		for(size_t i = 0; i < nbWords; i++){
			
			// Chose a random index to insert
			insertIndex = rand()%nbWordsToInsert;

			// Insert the word at this random index
			if(!insert(dic, buffer[insertIndex], buffer[insertIndex])){
				
				for(size_t j = 0; j < nbWordsToInsert; j++)
					free(buffer[j]);
				
				free(buffer);
				delete_dictionary(dic);
				
				return NULL;
			}

			nbWordsToInsert--;

			// Swap last ununsed index with this random index to avoid 
			// repetitions
			char* tmp = buffer[nbWordsToInsert];
			buffer[nbWordsToInsert] = buffer[insertIndex];
			buffer[insertIndex] = tmp;
		}
	}

	fclose(fp);
	free(buffer);

	return dic;
}