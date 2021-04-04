#include<stdlib.h>
#include<string.h>

#define MAXLINECHARS 256


typedef struct hashmapNode {
	char symbol[MAXLINECHARS];
	char mapping[MAXLINECHARS];
	struct hashmapNode *next;
	struct hashmapNode *prev;
} hashmapNode;

typedef struct hashmap { 
	hashmapNode *first;
	hashmapNode *last;
} hashmap;


hashmap *createHashMap(void)
{
	hashmap *hm = NULL;

	hm = (hashmap *)malloc(sizeof(hashmap));
	hm->first = NULL;
	hm->last = NULL;
	return hm;
}

int isEmpty(hashmap *hm)
{
	if (hm->first == NULL && hm->last == NULL)
		return  1;
	return 0;
}

int contains(hashmap *hm, char s[])
{
	hashmapNode *p = hm->first;

	while (p != NULL){
		if (strcmp(p->symbol,s) == 0)
			return 1;
		p = p->next;
	}
	return 0;
}

hashmapNode *get(hashmap *hm, char s[])
{
	if (hm == NULL)
		return NULL;

	hashmapNode *temp;
	
	temp = hm->first;
	while (strcmp(temp->symbol, s) != 0 && temp != NULL)
		temp=temp->next;
	
	return temp;
}

void put(hashmap *hm, char s[], char m[])
{
	if (hm == NULL)
		return;

	hashmapNode *node;

	node = (hashmapNode *)malloc(sizeof(hashmapNode));
	node->prev = NULL;
	node->next = NULL;
	memcpy(node->symbol, s, MAXLINECHARS);
	memcpy(node->mapping, m, MAXLINECHARS);

	if (isEmpty(hm)) {
		hm->first = node;
		hm->last = node;
	}else if (!(isEmpty(hm)) && !contains(hm,s)) {
		hashmapNode *temp;
		
		temp = hm->first;
		while (temp->next != NULL)
			temp = temp->next;
		node->prev = temp;
		temp->next = node;
		hm->last = node;
	}else if (!(isEmpty(hm)) && contains(hm,s)) {
		hashmapNode *temp;

		temp = hm->first;
		while (strcmp(temp->symbol, s) != 0) {
		       temp = temp->next;
		}
		memcpy(temp->mapping, m, MAXLINECHARS);
	}
}

void deleteOnce(hashmap *hm, char s[])
{
	if  (hm == NULL) 
		return;

	hashmapNode *temp;

	for (temp = hm->first;temp != NULL;temp = temp->next){
		if (strcmp(temp->symbol,s) == 0){
			if (temp->prev == NULL && temp->next == NULL){
				hm->first = NULL;
				hm->last  = NULL;
			}else if (temp->prev == NULL){
				hm->first = temp->next;
				temp->next->prev = NULL;
			}else if (temp->next == NULL){
				hm->last = temp->prev;
				temp->prev->next = NULL;
			}else{
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
			}
			free(temp);
			break;
		}
	}
}

void destroy(hashmap *hm) 
{
	if (hm == NULL)
		return;

	hashmapNode  *head;

	while (hm->first != hm->last){
		head = hm->first;
		hm->first = hm->first->next;
		free(head);
	}
	head = hm->first;
	free(head);
	free(hm);
}
