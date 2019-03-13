#include "radix.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(){
	RadixDic* dic = create_empty_dictionnary();
	char* word1 = "lol";
	char* word2 = "ptdr";
	char* word3 = "lole";

	insert(dic, word1, word1);
	insert(dic, word2, word2);
	insert(dic, word3, word3);

	assert(is_key_in_dic(dic, word1));
	assert(is_key_in_dic(dic, word2));
	assert(is_key_in_dic(dic, word3));

	assert(strcmp(word1, (char*)get_data(dic, word1)) == 0);
	assert(strcmp(word2, (char*)get_data(dic, word2)) == 0);
	assert(strcmp(word3, (char*)get_data(dic, word3)) == 0);
}