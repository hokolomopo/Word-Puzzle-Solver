#ifndef RADIX_H
#define RADIX_H
#include <stdbool.h>
#include <stddef.h>

// Structure representing a dictionary organised with radix search.
typedef struct RadixDic_t RadixDic;

// Structure representing a node of a RadixDic.
typedef struct Node_t Node;

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
char get_node_symbol(const Node* node);

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
RadixDic* create_empty_dictionary(void);

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
void delete_dictionary(RadixDic* dic);

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
bool insert(RadixDic* dic, const char* key, void* data);

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
bool is_key_in_dic(RadixDic* dic, const char* key);

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
void* get_data(const RadixDic* dic, const char* key);

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
Node* get_root(const RadixDic* dic);

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
Node* next_node(const Node* currNode, char symbol);

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
bool is_terminal(const Node* node);

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
char* get_node_data(const Node* node);

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
size_t get_dic_size(const RadixDic* dic);

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
size_t get_dic_max_length(const RadixDic* dic);

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
void** get_all_data(const RadixDic* dic);

#endif