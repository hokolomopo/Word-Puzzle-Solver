#include "solver.h"
#include "radix.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

int main(){
	RadixDic* wordDic = load_dic_from_file("words.txt", true);

	fprintf(stderr, "dic loaded\n");
	
	size_t gridSize;
	char*** grid = load_grid_from_file("grid_2x2.txt", &gridSize);
	fprintf(stderr, "grid loaded\n");

	if(!grid){
		fprintf(stderr, "grid NULL \n");
		return 0;
	}

	size_t foundLength;
	char** found = solve(grid, gridSize, wordDic, &foundLength);

	fprintf(stderr, "solved\n");
	char* shouldFind[4];
	shouldFind[0] = "AB";
	shouldFind[1] = "BA";
	shouldFind[2] = "CA";
	shouldFind[3] = "CAB";
	size_t nbWords = 4;

	char** words = (char**)(get_all_data(wordDic));

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