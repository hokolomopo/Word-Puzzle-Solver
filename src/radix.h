#ifndef RADIX_H
#define RADIX_H
#include <stdbool.h>
#include <stddef.h>

typedef struct RadixDic_t RadixDic;
typedef struct Node_t Node;

void print_dic(RadixDic* dic);

/**
Create an empty dictionnary.

Parameters
----------
void

Returns
-------
An empty dictionnary, or NULL in case allocation failed.
*/
RadixDic* create_empty_dictionnary();

/**
Deallocate a dictionnary.

Parameters
----------
dic: The dictionnary to deallocate.

Returns
-------
void
*/
void delete_dictionnary(RadixDic* dic);

/**
Insert data with a given key in a dictionnary. If the key is already in the
dictionnary, it erases previous content to put new data.

Parameters
----------
dic: The dictionnary in which to insert the data.
key: The key.
data: The data to insert;

Returns
-------
void
*/
void insert(RadixDic* dic, char* key, void* data);

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
bool is_key_in_dic(RadixDic* dic, char* key);

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
void* get_data(RadixDic* dic, char* key);

/**
Get the root node of a radix dictionnary.

Parameters
----------
dic: The dictionnary from which to get the root.

Returns
-------
The root of the dictionnary.
*/
Node* get_root(RadixDic* dic);

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
Node* next_node(Node* currNode, char symbol);

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
bool is_terminal(Node* node);

/**
Get the data associated to a node.

Parameters
----------
node: The node from which to retrieve data.

Returns
-------
The data associated to the node.
*/
char* get_node_data(Node* node);

/**
Get the number of elements in the dictionnary.

Parameters
----------
dic: The dictionnary from which to get the number of elements.

Returns
-------
The number of elements in the dictionnary.
*/
size_t get_dic_size(RadixDic* dic);

/**
Get length of the key of maximal length in the dictionnary.

Parameters
----------
dic: The dictionnary from which to get the maximal key length.

Returns
-------
The length of the key of maximal length.
*/
size_t get_dic_max_length(RadixDic* dic);

#endif