#include "radix.h"
#include <stdlib.h>

// /!\ GERER LE CAS OU ON CHERCHE ET QUE LE DICO EST VIDE !

struct RadixDic_t{
	Node* root;
};

struct Node_t{
	Node* leftSon;
	Node* middleSon;
	Node* rightSon;
	char* data;
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

	if(leftSon)
		delete_node_subtree(leftSon);
	
	if(middleSon)
		delete_node_subtree(middleSon);

	if(rightSon)
		delete_node_subtree(rightSon);
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
	RadixDic* dic = malloc(sizeof(RadixDic))

	// If malloc fails, it returns NULL
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
	if(dic -> root)
		delete_node_subtree(dic -> root);

	free(dic);
}

/**
Insert a given world in a given dictionnary.

Parameters
----------
dic: The dictionnary in which to insert the word.
word: The word to insert.
length: The length of the word.

Returns
-------
void
*/
void insert_word(RadixDic* dic, char* word, size_t length){

}

/**
Indicate if a given word is in a given dictionnary.

Parameters
----------
dic: The dictionnary in which to search the word.
word: The word to search.
length: The length of the word.

Returns
-------
A boolean indicating if the word is in the dictionnary.
*/
bool is_word_in_dic(RadixDic* dic, char* word, size_t length){
	currNode = dic -> root;

	for(size_t i = 0; i < length; i++){
		if(!currNode)
			return false;

		currNode = next_node(currNode, word[i]);
	}

	if(!currNode)
		return false;

	if(currNode -> data)
		return true;

	return false;
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
	while(true){

		if(symbol < currNode -> symbol){
			if(!currNode -> leftSon)
				return NULL;

			currNode = currNode -> leftSon;
		}

		else if(symbol > currNode -> symbol){
			if(!currNode -> rightSon)
				return NULL;

			currNode = currNode -> rightSon;
		}

		else{
			if(!currNode -> middleSon)
				return NULL;

			return currNode -> middleSon
		}
	}
	
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
	return node -> data != NULL;
}