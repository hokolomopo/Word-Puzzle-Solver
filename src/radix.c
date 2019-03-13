#include "radix.h"
#include <stdlib.h>
#include <stdio.h>

// /!\ GERER LE CAS OU ON CHERCHE ET QUE LE DICO EST VIDE !

static char NULL_SYMBOL = '\0';

struct RadixDic_t{
	Node* root;
	size_t size;
};

struct Node_t{
	Node* leftSon;
	Node* middleSon;
	Node* rightSon;
	void* data;
	char symbol;
};

void print_dic(RadixDic* dic){
	fprintf(stderr, "%c - %c - %c", dic -> root -> symbol, dic -> root -> middleSon -> symbol, dic -> root -> middleSon -> middleSon -> symbol);
}

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
Insert data with a given key in a dictionnary.

Parameters
----------
dic: The dictionnary in which to insert the data.
key: The key.
data: The data to insert;

Returns
-------
void
*/

// Problème: faut pas insérer le symbole
void insert(RadixDic* dic, char* key, void* data){
	char symbol;

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

	if(currNode)
		currNode -> data = data;

	else{
		Node* newNode = create_node(NULL_SYMBOL);
		newNode -> data = data;

		prevNode -> middleSon = newNode;
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
	return node -> data;
}