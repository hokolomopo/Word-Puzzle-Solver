#include "radix.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(){
	RadixDic* dic = create_empty_dictionnary();
	char* word1 = "lol";
	char* word2 = "ptdr";
	char* word3 = "lolee";
	char* word4 = "lo";
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

	insert(dic, word3, word4);
	assert(strcmp(word4, (char*)get_data(dic, word3)) == 0);
	assert(get_dic_size(dic) == 4);
	assert(get_dic_max_length(dic) == 5);

}