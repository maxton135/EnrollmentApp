// -------------------------------
// Operations supporting hash tables
// -------------------------------

#include"HashTable.h"

// -------------------------------
// Converts a 5 letter key into an integer
// CC###: ascii equivalent of letters and digits are simply added together.
// #####: ascii equivalent of digits are simply added together.
// -------------------------------
int convert( char key[5] )
{

	int sum = 0;
	for (int i = 0; i < 5; i++) {
		sum = sum + key[i];
	}
	return sum;

}

// -------------------------------
// Hash function that returns an index into the hash table given a key
// use the module formula on page 263 of CLRS.
// -------------------------------
int hash( char key[5], int slots )
{
	int num = convert(key);
	int hashval = num % slots;
	return hashval;

}