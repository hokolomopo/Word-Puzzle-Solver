#include "solver.h"
#include "radix.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main(){
	RadixDic* wordDic = load_dic_from_file("words.txt");

	fprintf(stderr, "dic loaded\n");
	
	size_t gridSize;
	char*** grid = load_grid_from_file("grid_2x2.txt", get_dic_max_length(wordDic), &gridSize);
	fprintf(stderr, "grid loaded\n");

	size_t foundLength;
	char** found = solve(grid, gridSize, wordDic, &foundLength);

	fprintf(stderr, "solved\n");
	char* shouldFind[3];
	shouldFind[0] = "AB";
	shouldFind[1] = "BA";
	shouldFind[2] = "CA";
	size_t nbWords = 3;

	assert(foundLength == nbWords);

	for(size_t i = 0; i < nbWords; i++){
		bool isFound = false;
		for(size_t j = 0; j < foundLength; j++){
			if(strcmp(shouldFind[i], found[j]) == 0)
				isFound = true;
		}
		assert(isFound);
	}
}