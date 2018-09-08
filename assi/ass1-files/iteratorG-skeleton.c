/* iteratorG.c
   Generic Iterator implementation, using doubly linked list

   Written by: ....
   Date: ....

    You need to submit ONLY this file.... 

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "iteratorG.h"

typedef struct Node {
    int value;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct IteratorGRep {
    int nitems;
    Node *befcur;
    Node *aftcur;
    Node *first;
    Node *last;
    ElmCompareFp cmpFp;
    ElmNewFp newFp;
    ElmFreeFp freeFp;
} IteratorGRep;


/*
  Your  functions below .... 
 */

static Node *newNode(void *vp){
    Node *new = malloc(sizeof(Node));
    new->value = ElmNewFp(vp);
    new->prev = new->next = NULL;
}
    

IteratorG newIterator(ElmCompareFp cmpFp, ElmNewFp newFp, ElmFreeFp freeFp){
	struct IteratorGRep *newlist;
	newlist = malloc(sizeof(struct IteratorGRep));
	newlist->nitems = 0;
	newlist->befcur = NULL;
	newlist->aftcur = NULL;
	newlist->first = NULL;
    newlist->last = NULL;
    newlist->cmpFp = cmpFp;
    newlist->newFp = newFp;
    newlist->freeFp = freeFp;
    
	return newlist;
}

int  add(IteratorG it, void *vp){

    Node *insert = newNode(vp);
    if(it->nitems == 0){
        it->aftcur = it->last = it->first = insert;
        it->nitems++;
    }else if(it->curr->prev == NULL && it->curr->next != NULL){
        insert->next = it->aftcur;
        it->aftcur->prev = insert;
        it->aftcur = insert;
        it->first = insert;
        insert->prev = NULL;
        it->nitems++;
    }else if(it->curr->prev != NULL && it->curr->next != NULL){
        it->befcur->next = insert;
        insert->prev = it->befcur;
        insert->next = it->aftcur;
        it->aftcur->prev = insert;
        it->aftcur = insert;
        it->nitems++;
    }else if(it->curr->prev != NULL && it->curr->next == NULL) {
        it->befcur->next = insert;
        insert->prev = it->befcur;
        insert->next = NULL;
        it->aftcur = insert;
        it->last = insert;
        it->nitems++;
    }
	return 1;
}
int  hasNext(IteratorG it){
    if(it->aftcur != NULL) {
        return 1;
    }
    return 0;

}
int  hasPrevious(IteratorG it){
	if(it->befcur != NULL) {
	    return 1;
    } 
	return 0;
}
void *next(IteratorG it){
    if(it->aftcur == NULL) {
        return NULL;
    }
    it->befcur = it->aftcur;
    it->aftcur = it->aftcur->next;
    return it->befcur->value;
}
void *previous(IteratorG it){
    if(it->befcur == NULL) {
        return NULL;
    }
    it->aftcur = it->befcur;
    it->befcur = it->befcur->prev;
    return it->aftcur->value;
}

int  del(IteratorG it){
    if(it->befcur == NULL) {
        return 0;
    }
    temp = it->befcur;
    temp->prev->next = it->aftcur;
    it->aftcur->prev = temp->prev;
    it->befcur = temp->prev;
    free(temp);
    it->nitems--;
}

int  set(IteratorG it, void *vp){       //???
	if(it->befcur == NULL) {
	    return 0;
	}
	it->curr->prev->value = *vp;
	return 1;
}

IteratorG advance(IteratorG it, int n){
    int i = 0;
    Node *temp;
    if(n>0){
        temp = it->befcur;
        while( i < n ){
            if(temp->next != NULL) {
                temp = temp->next;
            }else{
                return NULL;
            }
            i++;
        }
    }else{
        i = 0;
        temp = it->aftcur;
        while(i > n){
            if(temp->prev != NULL) {
                temp = temp->prev;
            }else{
                return NULL;
            }
            i--;
        }
    }
    
    IteratorG new = newIterator(ElmCompareFp cmpFp, ElmNewFp newFp, ElmFreeFp freeFp);
    if(n>0) {
        i = 0;
        while(i < n) {
            it->befcur = it->aftcur;
            it->aftcur = it->aftcur->next;
            Node *ins = newNode(it->befcur->value);
            if(new->nitems == 0) {
                new->first = new->last = new->aftcur = ins;
                new->nitems++;
            }else{
                new->aftcur->prev = ins;
                ins->next = new->aftcur;
                ins->prev = NULL;
                new->first = ins;
                new->nitems++;
            }
            i++;
        }
    }else {
        i = 0;
        while(i > n) {
            it->aftcur = it->befcur;
            it->befcur = it->befcur->prev;
            Node *ins = newNode(it->aftcur->value);
            if(new->nitems == 0) {
                new->first = new->last = new->aftcur = ins; 
                new->nitems++;
            }else{
                new->aftcur->prev = ins;
                ins->next = new->aftcur;
                ins->prev = NULL;
                new->first = ins;
                new->nitems++;
            }
            i++;
        }
    }
                
    return new;
}
void reverse(IteratorG it){
	Node *current = it->first;
	Node *temp = current = current->next;
	while(current != NULL) {
	    temp = current->next;
	    current->next = current->prev;
	    current->prev = temp;
	    current = current->next;
    }
    temp = it->head;
    it->first = it->last;
    it->last = temp;
}
IteratorG find(IteratorG it, int (*fp) (void *vp) ){
    fp = it-> 
	return NULL;
}
int distanceFromStart(IteratorG it){
    int i = 0;
    Node *temp = it->aftcur;
    while(temp->prev != NULL) {
        i++;
        temp = temp->prev;
    }
	return i;
}
int distanceToEnd(IteratorG it){
    int i = 0;
    Node *temp = it->befcur;
    while(temp->next != NULL) {
        i++;
        temp = temp->next;
    }
	return i;
}
void reset(IteratorG it){
	while(it->befcur != NULL) {
	    it->aftcur = it->befcur;
	    it->befcur = ir->befcur->prev;
    }  
}
void freeIt(IteratorG it){
    Node temp = it->first;
    while(temp != NULL) {
        Node todelete = temp;
        temp = temp->next;        
        it->freeFp(todelete->value);
        free(todelete);
    }
    free(it);
}



// --------------------------------------





