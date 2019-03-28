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

static const size_t MAX_WORD_LENGTH = 30;

static const size_t SIZE_STEP_DIC = 500;

static const size_t MAX_TILE_SIZE = 11;

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

	//fprintf(stderr, "gridSize = %ld\n", *gridSize);

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
			grid[i][j] = malloc(MAX_TILE_SIZE * sizeof(char));
			if(!grid[i][j])
				return NULL;
			
			size_t k;
			read = getc(fp);
			for(k = 0; read != WORD_DELIM && read != LINE_DELIM; k++){
				if(read == EOF)
					return NULL;

				//fprintf(stderr, "grid[%ld][%ld][%ld] = %c\n", i, j, k, read);
				grid[i][j][k] = read;

				read = getc(fp);
			}
			grid[i][j][k] = '\0';
		}

		if(read != LINE_DELIM)
			return NULL;

	}

	fclose(fp);

	/*
	for(size_t i = 0; i < *gridSize; i++){	
		for(size_t j = 0; j < *gridSize; j++){
			for(size_t k = 0; grid[i][j][k] != '\0'; k++)
				fprintf(stderr, "grid[%ld][%ld][%ld] = %c\n", i, j, k, grid[i][j][k]);
		}
	}

	for(size_t i = 0; i < *gridSize; i++){
		for(size_t j = 0; j < *gridSize; j++){
			fprintf(stderr, "grid[%ld][%ld] = %s\n", i, j, grid[i][j]);
		}
	}
	*/

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

	// Initialize it with NULL to avoid failures on free for unused slots.
	char** buffer = calloc(bufferSize, sizeof(char*));
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
		if(read == EOF){
			free(data);
			break;
		}

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
		else{
			//fprintf(stderr, "inserted = %s\n", data);
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

	for(size_t i = 0; i < bufferSize; i++)
		free(buffer[i]);

	fclose(fp);
	free(buffer);

	return dic;
}