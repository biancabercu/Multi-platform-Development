#ifndef _HASH_H_
#define _HASH_H_
#include<stdlib.h>
#include<string.h>

#define MAXLINECHARS 256


//typedef struct hashmapNode {
//	char symbol[MAXLINECHARS];
//	char mapping[MAXLINECHARS];
//	struct hashmapNode *next;
//	struct hashmapNode *prev;
//} hashmapNode;

//typedef struct hashmap { 
//	hashmapNode *first;
//	hashmapNode *last;
//} hashmap;


hashmap *createHashMap(void);

int isEmpty(hashmap *hm);

int contains(hashmap *hm, char s[]);

hashmapNode *get(hashmap *hm, char s[]);

void put(hashmap *hm, char s[], char m[]);

void deleteOnce(hashmap *hm, char s[]);

void destroy(hashmap *hm);

#endif 
