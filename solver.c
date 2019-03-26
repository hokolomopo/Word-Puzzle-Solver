#include "wordPuzzle.h"
#include "loader.h"
#include "radix.h"
#include <stdio.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		fprintf(stderr, "error");
		return 0;
	}

	RadixDic* wordDic = load_dic_from_file(argv[1], false);
	if(!wordDic){
		fprintf(stderr, "error");
		return 0;
	}
	/*
	char** words = (char**)(get_all_data(wordDic));
	for(size_t i = 0; i < get_dic_size(wordDic); i++){
		fprintf(stderr, "dumped = %s\n", words[i]);
	}
	*/

	size_t gridSize;
	char*** grid = load_grid_from_file(argv[2], &gridSize);
	if(!grid){
		fprintf(stderr, "error");
		return 0;
	}
	
	size_t returnLength;
	char** foundWords = solve(grid, gridSize, wordDic, &returnLength);
	if(!foundWords || returnLength == 0){
		fprintf(stderr, "error");
		return 0;
	}

	for(size_t i = 0; i < returnLength; i++){
		printf("%s\n", foundWords[i]);
	}
}