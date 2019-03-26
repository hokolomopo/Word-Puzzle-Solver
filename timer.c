#include "radix.h"
#include "loader.h"
#include "wordPuzzle.h"
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

static const char WORD_DELIM = ' ';
static const char LINE_DELIM = '\n';

double test(char* dicFileName, char* gridFileName){
	printf("dicFileName = %s\n", dicFileName);
	printf("gridFileName = %s\n", gridFileName);

	size_t gridSize;
	RadixDic* wordDic = load_dic_from_file(dicFileName, true);
	char*** grid = load_grid_from_file(gridFileName, &gridSize);

	clock_t start = clock();
	size_t returnLength;
	char** result = solve(grid, gridSize, wordDic, &returnLength);
	clock_t end = clock();

	return (end - start)/(double)CLOCKS_PER_SEC;
}

int main(){
	FILE* toTest = fopen("plots/toTest.txt", "r");
	if(!toTest){
		fprintf(stderr, "toTest failed");
		return 0;
	}

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
	while(read != EOF){
		size_t i;
		for(i = 0; read != WORD_DELIM; i++){
			gridName[i] = read;
			read = fgetc(toTest);
		}
		gridName[i] = '\0';

		read = fgetc(toTest);

		for(i = 0; read != LINE_DELIM; i++){
			dicName[i] = read;
			read = fgetc(toTest);
		}
		read = fgetc(toTest);

		dicName[i] = '\0';
		
		result = test(dicName, gridName);
		fprintf(resultFile, "%lf\n", result);
	}
	
	fclose(resultFile);
}