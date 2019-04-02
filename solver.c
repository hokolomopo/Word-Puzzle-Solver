#include "wordPuzzle.h"
#include "loader.h"
#include "radix.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	// Verify the amount of arguments passed
	if(argc != 3){
		fprintf(stderr, "error");
		return 0;
	}

	// Load the dictionary
	RadixDic* wordDic = load_dic_from_file(argv[1], true);
	if(!wordDic){
		fprintf(stderr, "error");
		return 0;
	}

	// Load the grid
	size_t gridSize;
	char*** grid = load_grid_from_file(argv[2], &gridSize);
	if(!grid){
		fprintf(stderr, "error");
		return 0;
	}
	
	// Solve the grid
	size_t returnLength;
	char** foundWords = solve((const char***) grid, gridSize, wordDic, 
							  &returnLength);
	if(!foundWords || returnLength == 0){
		fprintf(stderr, "error");
		return 0;
	}

	// Print results
	for(size_t i = 0; i < returnLength; i++){
		printf("%s\n", foundWords[i]);
	}

	// Free memory
	delete_dictionary(wordDic);

	for(size_t i = 0; i < gridSize; i++){
		for(size_t j = 0; j < gridSize; j++){
			free(grid[i][j]);
		}
		free(grid[i]);
	}
	free(grid);

	for(size_t i = 0; i < returnLength; i++){
		free(foundWords[i]);
	}
	free(foundWords);
}