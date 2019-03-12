#ifndef RADIX_H
#define RADIX_H
#include <stdbool.h>
#include <stddef.h>

typedef struct RadixDic_t RadixDic;
typedef struct Node_t Node;

/**
Create an empty dictionnary.

Parameters
----------
void

Returns
-------
An empty dictionnary
*/
RadixDic* create_empty_dictionnary();

/**
Deallocate a dictionnary.

Parameters
----------
dic: The dictionnary to deallocate

Returns
-------
void
*/
void delete_dictionnary(RadixDic* dic);

/**
Insert a given world in a given dictionnary.

Parameters
----------
dic: The dictionnary in which to insert the word
word: The word to insert
length: The length of the word.

Returns
-------
void
*/
void insert_word(RadixDic* dic, char* word, size_t length);

/**
Indicate if a given word is in a given dictionnary.

Parameters
----------
dic: The dictionnary in which to search the word
word: The word to search
length: The length of the word.

Returns
-------
A boolean indicating if the word is in the dictionnary.
*/
bool is_word_in_dic(RadixDic* dic, char* word, size_t length);

/**
Get the root node of a radix dictionnary.

Parameters
----------
dic: The dictionnary from which to get the root

Returns
-------
The root of the dictionnary 
*/
Node* get_root(RadixDic* dic);

/**
Get the node following the current node when the next symbol of the key is
symbol.

Parameters
----------
dic: The dictionnary containing the node
currNode: The current node
symbol: The next symbol of the key

Returns
-------
The next node if there exists word in the dictionnary with the current read
symbol of the key as prefix. NULL otherwise. 
*/
Node* next_node(RadixDic* dic, Node* currNode, char symbol);

/**
Indicate whether a node is terminal, i.e. represent the end of a word in the
dictionnary.

Parameters
----------
node: The node to check

Returns
-------
A boolean indicating whether the node is terminal
*/
bool is_terminal(Node* node);

#endif