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
	char* word1 = strdup("lol");
	char* word2 = strdup("ptdr");
	char* word3 = strdup("lolee");
	char* word4 = strdup("lo");
	char* words[4] = {word1, word2, word3, word4};
	size_t WORDS_LENGTH = 4;
	char* notInDic1 = "mdr"; 
	char* notInDic2 = "lor";

	assert(get_dic_size(dic) == 0);
	assert(get_dic_max_length(dic) == 0);
	insert(dic, word1, word1);
	assert(get_dic_size(dic) == 1);
	assert(get_dic_max_length(dic) == 3);
	insert(dic, word2, word2);
	assert(get_dic_size(dic) == 2);
	assert(get_dic_max_length(dic) == 4);
	insert(dic, word3, word3);
	assert(get_dic_size(dic) == 3);
	assert(get_dic_max_length(dic) == 5);
	insert(dic, word4, word4);
	assert(get_dic_size(dic) == 4);
	assert(get_dic_max_length(dic) == 5);

	assert(is_key_in_dic(dic, word1));
	assert(is_key_in_dic(dic, word2));
	assert(is_key_in_dic(dic, word3));
	assert(is_key_in_dic(dic, word4));
	assert(!is_key_in_dic(dic, notInDic1));
	assert(!is_key_in_dic(dic, notInDic2));

	assert(strcmp(word1, (char*)get_data(dic, word1)) == 0);
	assert(strcmp(word2, (char*)get_data(dic, word2)) == 0);
	assert(strcmp(word3, (char*)get_data(dic, word3)) == 0);
	assert(strcmp(word4, (char*)get_data(dic, word4)) == 0);
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
	assert(get_dic_size(dic) == 4);
	assert(get_dic_max_length(dic) == 5);

	fprintf(stderr, "before free\n");

	delete_dictionnary(dic);
}