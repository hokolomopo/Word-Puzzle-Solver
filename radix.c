#include "radix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static char NULL_SYMBOL = '\0';

struct RadixDic_t{
	Node* root;
	size_t size;
	size_t maxWordLength;
};

struct Node_t{
	Node* leftSon;
	Node* middleSon;
	Node* rightSon;
	void* data;
	char symbol;
};

/**
Create a node containing a given symbol.

Parameters
----------
symbol: The symbol associated to the node.

Returns
-------
A node structure to which the symbol "symbol" is associated, or NULL is case
allocation failed.
*/
static Node* create_node(char symbol){

	Node* node = malloc(sizeof(Node));
	
	if (!node)
		return NULL;

	node -> leftSon = NULL;
	node -> middleSon = NULL;
	node -> rightSon = NULL;
	node -> data = NULL;
	node -> symbol = symbol;

	return node;
}

/**
Deallocate a node and all its sons.

Parameters
----------
dic: The node to deallocate.

Returns
-------
void
*/
static void delete_node_subtree(Node* node){

	free(node -> data);

	if(node -> leftSon)
		delete_node_subtree(node -> leftSon);
	
	if(node -> middleSon)
		delete_node_subtree(node -> middleSon);

	if(node -> rightSon)
		delete_node_subtree(node -> rightSon);

	free(node);
}

/**
Create an empty dictionnary.

Parameters
----------
void

Returns
-------
An empty dictionnary, or NULL in case allocation failed.
*/
RadixDic* create_empty_dictionnary(){

	RadixDic* dic = malloc(sizeof(RadixDic));

	if(!dic)
		return NULL;

	dic -> root = NULL;
	dic -> size = 0;
	dic -> maxWordLength = 0;

	return dic;
}

/**
Deallocate a dictionnary.

Parameters
----------
dic: The dictionnary to deallocate.

Returns
-------
void
*/
void delete_dictionnary(RadixDic* dic){

	assert(dic);

	if(dic -> root)
		delete_node_subtree(dic -> root);

	free(dic);
}

/**
Insert data with a given key in a dictionnary. If the key is already in the
dictionnary, it erases previous content to put new data.

Parameters
----------
dic: The dictionnary in which to insert the data.
key: The key.
data: The data to insert, the user should insert a copy of the data as those
	  will be freed with dictionnary.

Returns
-------
void
*/
void insert(RadixDic* dic, char* key, void* data){

	assert(dic);
	assert(key);
	assert(data);
	
	char symbol;

	size_t wordLength = strlen(key);

	if(wordLength > dic -> maxWordLength)
		dic -> maxWordLength = wordLength;

	if(!dic -> root){
		Node* newNode = create_node(key[0]);
		dic -> root = newNode;
	}

	Node* currNode = dic -> root;
	Node* prevNode = currNode;

	for(size_t i = 0; key[i] != '\0'; i++){
		symbol = key[i];

		if(!currNode){
			currNode = create_node(symbol);
			prevNode -> middleSon = currNode;
		}

		while(currNode){
			if(symbol < currNode -> symbol){
				prevNode = currNode;
				currNode = currNode -> leftSon;

				if(!currNode){
					currNode = create_node(symbol);
					prevNode -> leftSon = currNode;
				}
			}

			else if (symbol > currNode -> symbol){
				prevNode = currNode;
				currNode = currNode -> rightSon;

				if(!currNode){
					currNode = create_node(symbol);
					prevNode -> rightSon = currNode;
				}
			}

			else{
				prevNode = currNode;
				currNode = currNode -> middleSon;

				break;
			}
		}

	}

	if(currNode){
		if(!currNode -> data)
			dic -> size ++;

		currNode -> data = data;
	}

	else{
		Node* newNode = create_node(NULL_SYMBOL);
		newNode -> data = data;

		prevNode -> middleSon = newNode;

		dic -> size++;
	}
}

/**
Get the node corresponding to a given key

Parameters
----------
dic: The dictionnary in which to search the key.
key: The key to search.

Returns
-------
The node corresponding to the key, NULL if the key does not exist in the 
dictionnary
*/
static Node* get_corresponding_node(RadixDic* dic, char* key){

	Node* currNode = dic -> root;

	for(size_t i = 0; key[i] != '\0' && currNode; i++)
		currNode = next_node(currNode, key[i]);

	return currNode;
}

/**
Indicate if a given word key is in a given dictionnary.

Parameters
----------
dic: The dictionnary in which to search the key.
key: The key to search.

Returns
-------
A boolean indicating if the word is in the dictionnary.
*/
bool is_key_in_dic(RadixDic* dic, char* key){

	assert(dic);
	assert(key);

	Node* keyNode = get_corresponding_node(dic, key);

	if(!keyNode)
		return false;

	return keyNode -> data != NULL;
}

/**
Retrieve the data associated to a given key

Parameters
----------
dic: The dictionnary in which to search the data.
key: The key to search.
keyLength: The length of the key.

Returns
-------
The data associated to the key, NULL of the key is not in the dictionnary.
*/
void* get_data(RadixDic* dic, char* key){

	assert(dic);
	assert(key);

	Node* keyNode = get_corresponding_node(dic, key);

	if(!keyNode)
		return NULL;

	return keyNode -> data;
}

/**
Get the root node of a radix dictionnary.

Parameters
----------
dic: The dictionnary from which to get the root.

Returns
-------
The root of the dictionnary.
*/
Node* get_root(RadixDic* dic){

	assert(dic);

	return dic -> root;
}

/**
Get the node correspoding to reading symbol "symbol" form the current node.

Parameters
----------
currNode: The current node.
symbol: The next symbol of the key.

Returns
-------
The next node if there exist word in the dictionnary with this symbol as next 
symbol, NULL otherwise. 
*/
Node* next_node(Node* currNode, char symbol){

	assert(currNode);

	while(currNode){

		if(symbol < currNode -> symbol)
			currNode = currNode -> leftSon;

		else if(symbol > currNode -> symbol)
			currNode = currNode -> rightSon;

		else{
			return currNode -> middleSon;
		}
	}

	return NULL;
}

/**
Indicate whether a node is terminal, i.e. represent the end of a word in the
dictionnary.

Parameters
----------
node: The node to check.

Returns
-------
A boolean indicating whether the node is terminal.
*/
bool is_terminal(Node* node){

	assert(node);

	return node -> data != NULL;
}

/**
Get the data associated to a node.

Parameters
----------
node: The node from which to retrieve data.

Returns
-------
The data associated to the node.
*/
char* get_node_data(Node* node){
	assert(node);

	return node -> data;
}

/**
Get the number of elements in the dictionnary.

Parameters
----------
dic: The dictionnary from which to get the number of elements.

Returns
-------
The number of elements in the dictionnary.
*/
size_t get_dic_size(RadixDic* dic){

	assert(dic);
	
	return dic -> size;
}

/**
Get length of the key of maximal length in the dictionnary.

Parameters
----------
dic: The dictionnary from which to get the maximal key length.

Returns
-------
The length of the key of maximal length.
*/
size_t get_dic_max_length(RadixDic* dic){

	assert(dic);

	return dic -> maxWordLength;
}

static void add_data_recursively(Node* currNode, void** datas, 
								 size_t* currIndex){

	if(!currNode)
		return;

	if(currNode -> data){
		datas[*currIndex] = currNode -> data;
		(*currIndex)++;
	}

	add_data_recursively(currNode -> leftSon, datas, currIndex);
	add_data_recursively(currNode -> middleSon, datas, currIndex);
	add_data_recursively(currNode -> rightSon, datas, currIndex);
}

/**
Get all the data in the dictionnary.

Parameters
----------
dic: The dictionnary from which to get the data

Returns
-------
An array of pointers to the data in the dictionnary. Note that the data are not
duplicated, NULL in case of allocation failure. The array returned is of size
dic -> size.
*/
void** get_all_data(RadixDic* dic){

	assert(dic);
	
	void** datas = malloc(dic -> size * sizeof(void*));
	
	if(!datas)
		return NULL;

	size_t currIndex = 0;

	add_data_recursively(dic -> root, datas, &currIndex);

	return datas;
}