#include "radix.h"
#include "loader.h"
#include "wordPuzzle.h"
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

static const char WORD_DELIM = ' ';
static const char LINE_DELIM = '\n';

/**
 * Compute the time needed to solve a given grid w.r.t a given dictionary
 * 
 * Parameters
 * ----------
 * dicFileName: The name of the file containing the dictionary.
 * gridFileName: The name of the file containing the grid.
 *
 * Returns
 * -------
 * The time needed to solve the grid.
 */
static double computeTime(char* dicFileName, char* gridFileName){
	printf("dicFileName = %s\n", dicFileName);
	printf("gridFileName = %s\n", gridFileName);

	// Load dictionary and grid
	size_t gridSize;

	// Compute time needed
	clock_t start = clock();
	RadixDic* wordDic = load_dic_from_file(dicFileName, true);

	char*** grid = load_grid_from_file(gridFileName, &gridSize);

	size_t returnLength;
	char** result = solve((const char***) grid, gridSize, wordDic, &returnLength);
	clock_t end = clock();

	return (end - start)/(double)CLOCKS_PER_SEC;
}

int main(){
	// Open the file containing the name of the grids and dictionaries to test.
	FILE* toTest = fopen("plots/toTest.txt", "r");
	if(!toTest){
		fprintf(stderr, "toTest failed");
		return 0;
	}

	// Open the file in which to write the results.
	FILE* resultFile = fopen("plots/results.txt", "w");
	if(!resultFile){
		fprintf(stderr, "result file failed");
		return 0;
	}

	char dicName[50];
	char gridName[50];

	char read = fgetc(toTest);
	double result;
	size_t line = 0;

	// Read the toTest file.
	while(read != EOF){
		size_t i;

		// Read the name of the grid
		for(i = 0; read != WORD_DELIM; i++){
			gridName[i] = read;
			read = fgetc(toTest);
		}
		gridName[i] = '\0';

		// Read the name of the dictionary
		read = fgetc(toTest);

		for(i = 0; read != LINE_DELIM; i++){
			dicName[i] = read;
			read = fgetc(toTest);
		}
		read = fgetc(toTest);

		dicName[i] = '\0';

		// Compute the time
		result = computeTime(dicName, gridName);
		fprintf(resultFile, "%lf\n", result);
	}
	
	fclose(resultFile);
}