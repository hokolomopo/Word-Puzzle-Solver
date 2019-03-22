#include "radix.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "solver.h"

int main(){
	RadixDic* dic = create_empty_dictionnary();
	/*
	char* word1 = strdup("lol");
	char* word2 = strdup("ptdr");
	char* word3 = strdup("lolee");
	char* word4 = strdup("lo");
	*/
	char* word1 = strdup("AACHEN");
	char* word2 = strdup("AARDVARK");
	char* word3 = strdup("AARDVARKS");
	char* word4 = strdup("AARON");
	char* word5 = strdup("AB");
	char* word6 = strdup("ABA");
	char* words[6] = {word1, word2, word3, word4, word5, word6};
	size_t WORDS_LENGTH = 6;
	char* notInDic1 = "mdr"; 
	char* notInDic2 = "lor";

	assert(get_dic_size(dic) == 0);
	assert(get_dic_max_length(dic) == 0);
	insert(dic, word1, word1);
	assert(get_dic_size(dic) == 1);
	assert(get_dic_max_length(dic) == 6);
	insert(dic, word2, word2);
	assert(get_dic_size(dic) == 2);
	assert(get_dic_max_length(dic) == 8);
	insert(dic, word3, word3);
	assert(get_dic_size(dic) == 3);
	assert(get_dic_max_length(dic) == 9);
	insert(dic, word4, word4);
	assert(get_dic_size(dic) == 4);
	assert(get_dic_max_length(dic) == 9);
	insert(dic, word5, word5);
	assert(get_dic_size(dic) == 5);
	assert(get_dic_max_length(dic) == 9);
	insert(dic, word6, word6);
	assert(get_dic_size(dic) == 6);
	assert(get_dic_max_length(dic) == 9);

	assert(is_key_in_dic(dic, word1));
	assert(is_key_in_dic(dic, word2));
	assert(is_key_in_dic(dic, word3));
	assert(is_key_in_dic(dic, word4));
	assert(is_key_in_dic(dic, word5));
	assert(is_key_in_dic(dic, word6));
	assert(!is_key_in_dic(dic, notInDic1));
	assert(!is_key_in_dic(dic, notInDic2));

	assert(strcmp(word1, (char*)get_data(dic, word1)) == 0);
	assert(strcmp(word2, (char*)get_data(dic, word2)) == 0);
	assert(strcmp(word3, (char*)get_data(dic, word3)) == 0);
	assert(strcmp(word4, (char*)get_data(dic, word4)) == 0);
	assert(strcmp(word5, (char*)get_data(dic, word5)) == 0);
	assert(strcmp(word6, (char*)get_data(dic, word6)) == 0);
	assert(get_data(dic, notInDic1) == NULL);
	assert(get_data(dic, notInDic2) == NULL);

	fprintf(stderr, "before\n");
	void** datas = get_all_data(dic);
	fprintf(stderr, "after\n");
	
	for(size_t i = 0; i < WORDS_LENGTH; i++){
		fprintf(stderr, "iteration %ld\n", i);
		
		bool isInDic = false;
		for(size_t j = 0; j < get_dic_size(dic); j++){
			if(strcmp(words[i], datas[j]) == 0){
				isInDic = true;
				break;
			}
		}

		assert(isInDic);
	}

	insert(dic, word3, word4);
	assert(strcmp(word4, (char*)get_data(dic, word3)) == 0);
	assert(get_dic_size(dic) == 6);
	assert(get_dic_max_length(dic) == 9);

	fprintf(stderr, "before free\n");

	delete_dictionnary(dic);
}