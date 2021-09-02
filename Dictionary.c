// -------------------------------
// Dictionary ADT:
//
// -------------------------------

#include <stdlib.h>		// for built-in malloc that returns void *
#include "Dictionary.h"
#include <string.h>
#include<stdio.h>
// -------------------------------
// TODO: implement create
// create( *D, n ) -- creates a dictionary with n slots
// Initializes slots to n, and size to 0.
// -------------------------------

void create(dictionary *D, int n)
{   
    // initialize the 'slots' and 'size' field
    D -> slots = n;
    D -> size = 0;

    // allocate memory for 'n' node_t structs and assignt to 'hash_table' field
    D -> hash_table = (node_t*)malloc(n * sizeof(node_t));
   
    // loop over the 'n' elements
    // initialize 'value', 'next' , and 'prev' fields to 'NULL'
    // initialize the 'key' field of each node to a char string of size 5
    // HINT: can use something like this 'memcpy( D->hash_table[i].key, "", 5 );'
    for (int i = 0; i < n; i++) {
    	D -> hash_table[i].value = NULL;
    	D -> hash_table[i].next = NULL;
    	D -> hash_table[i].prev = NULL;
    	memcpy( D -> hash_table[i].key, "", 5 );
    }
    
}


// -------------------------------
// TODO: implement insert
// insert( *D, *e ) -- inserts element e->key:e->value into the dictionary.
// First, make sure that e->key does not exist in *D yet.
// If it does, return an error code of -1.
// Otherwise, update the size of *D, and return 0.
// -------------------------------

int insert(dictionary* D, element* e)
{
    // check if element's key already in dictionary
    node_t *ele = find(D, e -> key);
    if (ele != NULL) {
        return -1;
    }

    // if not:

        // instantiate a new node
    	node_t *new_node = (node_t*)malloc(sizeof(node_t));

        // set the 'key' and 'value' fields of the newly instantiated node
        // these come from 'key' and 'value' fields of element
        // HINT: can use memcpy to copy element's key to node's key field
        memcpy( new_node -> key, e -> key, 5 );
        new_node -> value = (e -> value);

        // get the index of element in the dictionary
        int index = hash(e -> key, D -> slots);

        // using the index, get the node/head of the list corresp. to that index
        // add the newly created node to the list
        // NOTE: this follows the same logic of adding a node to list
        // but do NOT use insertList from List.c
        node_t *head = &(D -> hash_table[index]);

        if (head == NULL) {
            *head = *new_node;
            (*head).next = NULL;
            (*head).prev = NULL;
        }
        else {
            new_node -> next = head;
            (*head).prev = new_node;
            new_node -> prev = NULL;
            (*head) = *new_node;
        }

        D -> size = (D -> size) + 1;

        for (int i = 0; i < (D -> size); i++) {
            head = (*head).next;
        }
        (*head).next = NULL;
    
        return 0;
}



// -------------------------------
// TODO: implement delete
// delete( *D, *key ) -- deletes element with key (*key) from the dictionary.
// If *key does not exist in *D, return an error code of -1.
// Update the size of *D, and return 0.
// -------------------------------

int delete(dictionary* D, char* key)
{


    node_t *N = find(D, key);
    if (N == NULL) {
        return -1;
    }

    int index = hash(key, D -> slots);
    node_t *head = &(D -> hash_table[index]);
    //node_t **head = &ptr;

    // if key exists, navigate to the node that has such key
    // remove the node from the list
    // NOTE: again, this is same logic as in deleting a node
    // but do NOT use delete method from List.c

    if (head == NULL || N == NULL) {
        return -1;
    }

    if (head == N) {
        head = N -> next;
    }

    if (N -> next != NULL) {
        N -> next -> prev = N -> prev;
    }

    if (N -> prev != NULL) {
        N -> prev -> next = N -> next;
    }

    //free(N);

    return 0;
}



// -------------------------------
// TODO: implement find
// find( D, k ) -- returns pointer to node whose key is *k from dictionary *D.
// If *k is not found in *D, then return NULL.
// -------------------------------

node_t* find(dictionary* D, char* k)
{
    // hash key to get index and then loop over the list at that index
    // to find node with same key
    // NOTE: you can use the findList method from List.c but need to
    // distinguish between the case when it returns the node with key
    // or just the sentinel head of the list at that index of the dict
    int index = hash( k, D -> slots);
    node_t *N = &(D -> hash_table[index]);
    node_t *retN = findList( N, k );    
    return retN;


}


// -------------------------------
// TODO: implement print
// print( dictionary *D ) -- prints the dictionary.
// Specifically, it looks at the non-empty slots of the hash table,
// and prints the key field of each node in the list.
// -------------------------------

void print(dictionary* D)
{
	for (int i = 0; i < (D -> slots); i++) {
        //printf("\n[%d] : ", i);
        node_t *ptr = &(D -> hash_table[i]);
        if (ptr -> value != NULL) {
            printList(ptr);
        }
	}
    printf("\n\n");
}