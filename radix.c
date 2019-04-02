#include "radix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static char NULL_SYMBOL = '\0';

// Structure representing a dictionary organised with radix search.
struct RadixDic_t{
	Node* root;
	size_t size;
	size_t maxWordLength;
};

// Structure representing a node of a RadixDic_t.
struct Node_t{
	Node* leftSon;
	Node* middleSon;
	Node* rightSon;
	void* data;
	char symbol;
};

/**
 * Get the symbol of a given node.
 *
 * Parameters
 * ----------
 * node: The node from which to get the symbol.
 *
 * Returns
 * -------
 * The symbol associated to the node.
 */
char get_node_symbol(const Node* node){
	return node -> symbol;
}

/**
 * Create a node containing a given symbol.
 *
 * Parameters
 * ----------
 * symbol: The symbol associated to the node.
 *
 * Returns
 * -------
 * A node structure to which the symbol "symbol" is associated, or NULL is case
 * allocation failed.
 */
static Node* create_node(char symbol){

	// Allocate memory
	Node* node = malloc(sizeof(Node));
	
	if (!node)
		return NULL;

	// Initialise content
	node -> leftSon = NULL;
	node -> middleSon = NULL;
	node -> rightSon = NULL;
	node -> data = NULL;
	node -> symbol = symbol;

	return node;
}

/**
 * Deallocate a node and all its sons.
 *
 * Parameters
 * ----------
 * dic: The node to deallocate.
 *
 * Returns
 * -------
 * void
 */
static void delete_node_subtree(Node* node){

	// Free this node's data
	free(node -> data);

	// Free the rest if the tree recursively
	if(node -> leftSon)
		delete_node_subtree(node -> leftSon);
	
	if(node -> middleSon)
		delete_node_subtree(node -> middleSon);

	if(node -> rightSon)
		delete_node_subtree(node -> rightSon);

	// Free the current node
	free(node);
}

/**
 * Create an empty dictionary.
 * 
 * Parameters
 * ----------
 * void
 * 
 * Returns
 * -------
 * An empty dictionary, or NULL in case allocation failed.
 */
RadixDic* create_empty_dictionary(void){

	// Allocate memory
	RadixDic* dic = malloc(sizeof(RadixDic));

	if(!dic)
		return NULL;

	// Initialise content
	dic -> root = NULL;
	dic -> size = 0;
	dic -> maxWordLength = 0;

	return dic;
}

/**
 * Deallocate a dictionary.
 * 
 * Parameters
 * ----------
 * dic: The dictionary to deallocate.
 *
 * Returns
 * -------
 * void
 */
void delete_dictionary(RadixDic* dic){

	assert(dic);

	// Delete content
	if(dic -> root)
		delete_node_subtree(dic -> root);

	// Delete the dictionary
	free(dic);
}

/**
 * Insert data with a given key in a dictionary. If the key is already in the
 * dictionary, it erases previous content to put new data.
 * 
 * Parameters
 * ----------
 * dic: The dictionary in which to insert the data.
 * key: The key.
 * data: The data to insert, the user should insert a copy of the data as those
 *	  	 will be freed with dictionary.
 *
 * Returns
 * -------
 * A boolean indicating if all went well or not.
 */
bool insert(RadixDic* dic, const char* key, void* data){

	// Assert inputs
	if(!dic || !key || !data)
		return false;
	
	char symbol;

	// Check if the word is valid
	size_t wordLength = strlen(key);
	if(wordLength == 0)
		return false;

	// Update dictionary maxWordLengh component
	if(wordLength > dic -> maxWordLength)
		dic -> maxWordLength = wordLength;

	// If the dictionary is empty, initialize the root
	if(!dic -> root){
		Node* newNode = create_node(key[0]);

		if(!newNode)
			return false;

		dic -> root = newNode;
	}

	// Find the node insertion place
	Node* currNode = dic -> root;
	Node* prevNode = currNode;

	for(size_t i = 0; key[i] != '\0'; i++){

		symbol = key[i];

		/*
		If the current node is not valid, it means we found a node with the
		previous symbol at previous iteration and this node has no middle 
		child node, we thus insert a new node with the current symbol.
		*/
		if(!currNode){
			currNode = create_node(symbol);
			
			if(!currNode)
				return false;

			prevNode -> middleSon = currNode;
		}

		// Search for the current symbol
		while(true){

			// If searched symbol is higher than the symbol of the current node
			// search on the right subtree.
			if(symbol < currNode -> symbol){
				prevNode = currNode;
				currNode = currNode -> leftSon;

				// If there is no right subtree, then the searched node doesn't
				// exit, we create it
				if(!currNode){
					currNode = create_node(symbol);
					
					if(!currNode)
						return false;

					prevNode -> leftSon = currNode;
				}
			}

			// If searched symbol is lower than the symbol of the current node
			// search on the left subtree.
			else if (symbol > currNode -> symbol){
				prevNode = currNode;
				currNode = currNode -> rightSon;

				// If there is no left subtree, then the searched node doesn't
				// exit, we create it
				if(!currNode){
					currNode = create_node(symbol);
					
					if(!currNode)
						return false;

					prevNode -> rightSon = currNode;
				}
			}

			// Found the searched node, follow the middle subtree.
			else{
				prevNode = currNode;
				currNode = currNode -> middleSon;

				break;
			}
		}
	}

	// If the reached node exist, add the data to it.
	if(currNode){
		
		if(!currNode -> data)
			dic -> size ++;

		currNode -> data = data;
	}

	// If the reached node doesn't exist, create a NULL node holding the data
	else{
		Node* newNode = create_node(NULL_SYMBOL);

		if(!newNode)
			return false;

		newNode -> data = data;

		prevNode -> middleSon = newNode;

		dic -> size++;
	}

	return true;
}

/**
 * Get the node corresponding to a given key
 *
 * Parameters
 * ----------
 * dic: The dictionary in which to search the key.
 * key: The key to search.
 * 
 * Returns
 * -------
 * The node corresponding to the key, NULL if the key does not exist in the 
 * dictionary
 */
static Node* get_corresponding_node(const RadixDic* dic, const char* key){

	// Start from the root
	Node* currNode = dic -> root;

	// Follow the path corresponding to the key
	for(size_t i = 0; key[i] != '\0' && currNode; i++)
		currNode = next_node(currNode, key[i]);

	return currNode;
}

/**
 * Indicate if a given word key is in a given dictionary.
 *
 * Parameters
 * ----------
 * dic: The dictionary in which to search the key.
 * key: The key to search.
 *
 * Returns
 * -------
 * A boolean indicating if the word is in the dictionary.
 */
bool is_key_in_dic(RadixDic* dic, const char* key){

	assert(dic);
	assert(key);

	Node* keyNode = get_corresponding_node(dic, key);

	// If the node has not been found ,it is not in the dictionary.
	if(!keyNode)
		return false;

	// The node is a valid key if it is terminal
	return is_terminal(keyNode);
}

/**
 * Retrieve the data associated to a given key
 * 
 * Parameters
 * ----------
 * dic: The dictionary in which to search the data.
 * key: The key to search.
 * keyLength: The length of the key.
 *
 * Returns
 * -------
 * The data associated to the key, NULL of the key is not in the dictionary.
 */
void* get_data(const RadixDic* dic, const char* key){

	assert(dic);
	assert(key);

	Node* keyNode = get_corresponding_node(dic, key);

	// If the node has not been found ,it is not in the dictionary.
	if(!keyNode)
		return NULL;

	return keyNode -> data;
}

/**
 * Get the root node of a radix dictionary.
 *
 * Parameters
 * ----------
 * dic: The dictionary from which to get the root.
 *
 * Returns
 * -------
 * The root of the dictionary.
 */
Node* get_root(const RadixDic* dic){

	assert(dic);

	return dic -> root;
}

/**
 * Get the node correspoding to reading symbol "symbol" form the current node.
 *
 * Parameters
 * ----------
 * currNode: The current node.
 * symbol: The next symbol of the key.
 *
 * Returns
 * -------
 * The next node if there exist word in the dictionary with this symbol as next 
 * symbol, NULL otherwise. 
 */
Node* next_node(const Node* currNode, char symbol){

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
 * Indicate whether a node is terminal, i.e. represent the end of a word in the
 * dictionary.
 *
 * Parameters
 * ----------
 * node: The node to check.
 * 
 * Returns
 * -------
 * A boolean indicating whether the node is terminal.
 */
bool is_terminal(const Node* node){

	assert(node);

	return node -> data != NULL;
}

/**
 * Get the data associated to a node.
 *
 * Parameters
 * ----------
 * node: The node from which to retrieve data.
 *
 * Returns
 * -------
 * The data associated to the node.
 */
char* get_node_data(const Node* node){
	assert(node);

	return node -> data;
}

/**
 * Get the number of elements in the dictionary.
 * 
 * Parameters
 * ----------
 * dic: The dictionary from which to get the number of elements.
 *
 * Returns
 * -------
 * The number of elements in the dictionary.
 */
size_t get_dic_size(const RadixDic* dic){

	assert(dic);
	
	return dic -> size;
}

/**
 * Get length of the key of maximal length in the dictionary.
 * 
 * Parameters
 * ----------
 * dic: The dictionary from which to get the maximal key length.
 *
 * Returns
 * -------
 * The length of the key of maximal length.
 */
size_t get_dic_max_length(const RadixDic* dic){

	assert(dic);

	return dic -> maxWordLength;
}

/**
 * Recursive function retrieving all the data from a subtree of a radix 
 * dictionary and add those to datas.
 * 
 * Parameters
 * ----------
 * currNode: The node at the top of the subtree form which to retrieve data.
 * datas: An array of data in which to insert the retrieved data.
 * currIndex: A pointer to the current index to which insert the found data.
 *
 * Returns
 * -------
 * void
 */
static void add_data_recursively(const Node* currNode, void** datas, 
								 size_t* currIndex){

	if(!currNode)
		return;

	// If the node has data, add it to the found datas
	if(currNode -> data){
		datas[*currIndex] = currNode -> data;
		(*currIndex)++;
	}

	// Add the data contained in every subtree
	add_data_recursively(currNode -> leftSon, datas, currIndex);
	add_data_recursively(currNode -> middleSon, datas, currIndex);
	add_data_recursively(currNode -> rightSon, datas, currIndex);
}

/**
 * Get all the data in the dictionary.
 * 
 * Parameters
 * ----------
 * dic: The dictionary from which to get the data
 *
 * Returns
 * -------
 * An array of pointers to the data in the dictionary. Note that the data are 
 * not duplicated, NULL in case of allocation failure. The array returned is of 
 * size dic -> size.
 */
void** get_all_data(const RadixDic* dic){

	assert(dic);
	
	// Allocate memory
	void** datas = malloc(dic -> size * sizeof(void*));
	
	if(!datas)
		return NULL;

	// Retrieve the data the dic
	size_t currIndex = 0;
	add_data_recursively(dic -> root, datas, &currIndex);

	return datas;
}