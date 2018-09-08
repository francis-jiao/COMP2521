/* iteratorG.c
   Generic Iterator implementation, using doubly linked list

   Written by: MINGFANG JIAO z5142125
   Date: 21.04.2018

    You need to submit ONLY this file.... 

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "iteratorG.h"

typedef struct Node {
    void  *value;               // the element in the node
    struct Node *prev;       // the pointer of previous node
    struct Node *next;       // the pointer of next node
} Node;

typedef struct IteratorGRep {
    int nitems;              // the number of items in the node
    Node *befcur;            // the cursor is locating 
    Node *aftcur;            // between the befcur node and aftcur node
    Node *first;             // the first node in the list
    Node *last;              // the last node in the list
    ElmCompareFp cmpFp;      
    ElmNewFp newFp;
    ElmFreeFp freeFp;
} IteratorGRep;


/*
  Your  functions below .... 
 */
 
// creat a new node 
static Node *newNode(void *vp, IteratorG it){  
    assert(it != NULL);
    
    Node *new;
    new = malloc(sizeof(Node));              // allocate memory for new node
    if(new == NULL) {
        return NULL;
    }else{
        new->value = it->newFp(vp);          // set value as on the basis of pointer to value
        new->next = new->prev = NULL;        // set previous node and next node to null
        return new;
    }
}

    

IteratorG newIterator(ElmCompareFp cmpFp, ElmNewFp newFp, ElmFreeFp freeFp){
	struct IteratorGRep *newlist;                   // creat a new iterator
	newlist = malloc(sizeof(struct IteratorGRep));  // allocate memory for list iterator
	newlist->nitems = 0;                            // set nitems to 0
	newlist->befcur = NULL;                         // set the follwoing node to NULL
	newlist->aftcur = NULL;                         // befcur aftcur first last 
	newlist->first = NULL;
    newlist->last = NULL;
    newlist->cmpFp = cmpFp;
    newlist->newFp = newFp;
    newlist->freeFp = freeFp;
    
	return newlist;
}

int  add(IteratorG it, void *vp){
    Node *insert = newNode(vp,it);           // put vp in to a new node
    if(it->nitems == 0){                     
        it->aftcur = insert;                 // if it is an empty list
        it->last = insert;
        it->first = insert;
        it->nitems++;
    }else if(it->befcur == NULL && it->aftcur != NULL){ 
        insert->next = it->aftcur;
        it->aftcur->prev = insert;          // if the cursor is at the beginning of the list
        it->aftcur = insert;
        it->first = insert;
        insert->prev = NULL;
        it->nitems++;
    }else if(it->befcur != NULL && it->aftcur != NULL){
        it->befcur->next = insert;           // if the cursor is at the body of the list
        insert->prev = it->befcur;
        insert->next = it->aftcur;
        it->aftcur->prev = insert;
        it->aftcur = insert;
        it->nitems++;
    }else if(it->befcur != NULL && it->aftcur == NULL) {
        it->befcur->next = insert;
        insert->prev = it->befcur;           // if the cursor is at the end of the list
        insert->next = NULL;
        it->aftcur = insert;
        it->last = insert;
        it->nitems++;
    }
	return 1;
}

int  hasNext(IteratorG it){
    if(it->aftcur != NULL) {
        return 1;                      // if the list has next node after cursor, return 1
    }                                  
    return 0;                          // else return 0
}
int  hasPrevious(IteratorG it){
	if(it->befcur != NULL) {           // if the list has node before cursor, return 1
	    return 1;
    } 
	return 0;                          // else return 0
} 
void *next(IteratorG it){
    if(it->aftcur == NULL) {           // if there is no next node
        return NULL;                   // return NULL
    }                                 
    it->befcur = it->aftcur;           // else move the cursor forward for one node  
    it->aftcur = it->aftcur->next; 
    return it->befcur->value;          // return the pointer to the value
}
void *previous(IteratorG it){          
    if(it->befcur == NULL) {           // if there is no previous node
        return NULL;                   // return NULL
    }
    it->aftcur = it->befcur;           // else move the cursor inverted for one node 
    it->befcur = it->befcur->prev;
    return it->aftcur->value;          // return the pointer to the value
}

int  del(IteratorG it){
    if(it->befcur == NULL) {                 // if there is no node before cursor
        return 0;                            // return 0
    }else if(it->befcur->prev == NULL){      // if there is only one node before cursor
        if(it->aftcur != NULL){              // if there are nodes after cursor
            it->freeFp(it->befcur->value);   
            free(it->befcur);            
            it->befcur = NULL;
            it->aftcur->prev = NULL;         
            it->first = it->aftcur;
        }else{                               // if there are no nodes after cursor
            it->freeFp(it->befcur->value);
            free(it->befcur);
            it->befcur = NULL;
            it->first = it->last = NULL;
        }                       
        it->nitems--;
    }else {                                    // if there are more than one node before cursor
        Node *temp = it->befcur;
        if(it->aftcur != NULL){                // if there are nodes after cursor
            temp->prev->next = it->aftcur;
            it->aftcur->prev = temp->prev;
            it->befcur = temp->prev;
            it->freeFp(temp->value);
            free(temp);                        //  free befcur after connect the aftcur and befcur->prev
        }else{                                 // if there are no nodes after cursor
            temp->prev->next = NULL;
            it->last = temp->prev;
            it->befcur = temp->prev;
            it->freeFp(temp->value);
            free(temp);
        }        
        it->nitems--;  // number of items -1
    }                                     
    return 1;
}

int  set(IteratorG it, void *vp){       
	if(it->befcur == NULL)  return 0;   // if there in no node before cursor return 0
	it->befcur->value = it->newFp(vp);  // else set the new value in the node
	return 1;
}

IteratorG advance(IteratorG it, int n){
    IteratorG new = newIterator(it->cmpFp, it->newFp, it->freeFp); // creat a new iterator
    int i = 0;                                                     
    if (n > 0) {                                                   // if n>0
        if(distanceToEnd(it) < n){                                 // if n exceed the distancetoend
            return NULL;                                           // return NULL
        }else{
            while (i < n) {                                        
                it->befcur = it->aftcur;                           // move the cursor forward for one node
                it->aftcur = it->aftcur->next;
                Node *ins = newNode(it->befcur->value,it);         // create a new node called ins
                if(new->nitems == 0) {
                    new->first = new->last = new->aftcur = ins;    // if the iterator is empty set the inserted node as first, last, aftcur node
                    new->nitems++;                                 // nitems +1
                }else{
                    new->last->next = ins;                         // if the iterator is not empty add the node at the end of the list
                    ins->prev = new->last;
                    ins->next = NULL;
                    new->last = ins;
                    new->nitems++;
                }
                i++;
            }
        }
    }else{
        n = n * -1;                                                    // if n < 0
        if(n > distanceFromStart(it)){                                 // if -n exceed the distancetoend 
            return NULL;                                               // return NULL
         }else{
            while (i < n) {
                it->aftcur = it->befcur;                               // move the cursor inverted for one node
                it->befcur = it->befcur->prev;
                Node *ins = newNode(it->aftcur->value,it);             // create a new node called ins
                if(new->nitems == 0) {
                    new->first = new->last = new->aftcur = ins;        // if the iterator is empty set the inserted node as first, last, aftcur node
                    new->nitems++;                                     // nitems +1
                }else{
                    new->last->next = ins;                             
                    ins->prev = new->last;
                    ins->next = NULL;                                  // if the iterator is not empty add the node at the end of the list
                    new->last = ins;
                    new->nitems++;
                }
                i++;
            }
        }
    }
    return new;
}

void reverse(IteratorG it){
    int n = 0;
    int cursor = distanceToEnd(it);       // record the position of cursor
    reset(it);                            // move the cursor to the beginning of the list
    int nelements = distanceToEnd(it);    // record the number of elements in the list
    void *newlist[nelements-1];          // create a array with distancetoend -1 elements
    while(hasNext(it)){                  
        newlist[n] = next(it);           // copy all the elements in the list to the array
        n++;
    }
    n = 0;
    while(hasPrevious(it)){             // set the value of every element in the list from the end to beginning 
        set(it,newlist[n]); 
        previous(it);          
        n++;
    }
    reset(it);                          // recover the cursor to it's previous position
    advance(it,cursor);
}
IteratorG find(IteratorG it, int (*fp) (void *vp) ){
    IteratorG findlist = newIterator(it->cmpFp,it->newFp,it->freeFp);           // create a new iterator called findlist
    Node *temp = it->aftcur;
    while(temp != NULL) {
        if(fp(temp->value) == 1) {                                              // if this node is wanted (fp() return 1)
            Node *ins = newNode(temp->value,it);                                
            if(findlist->nitems == 0) {                                         // put the node in the findlist
                findlist->first = findlist->last = findlist->aftcur = ins;      // if the iterator is empty set the inserted node as first, last, aftcur node
                findlist->nitems++;                                             // nitems +1
            }else{
                findlist->last->next = ins;                                     
                ins->prev = findlist->last;                                     // if the iterator is not empty add the node at the end of the list
                findlist->last = ins;
                findlist->nitems++;
            }
        }
        temp=temp->next;
    }
	return findlist;
}
int distanceFromStart(IteratorG it){
    int i = 1;                                  // aftcur as a after cursor node so i start from 1
    if(it->befcur == NULL )  return 0;          // if there is no node before cursor return 0
    Node *temp = it->befcur;
    while(temp->prev != NULL) {                 //use a while loop to calculate the distance from start 
        i++;                                    // stop untill at the beginning of list
        temp = temp->prev;
    }
	return i;
}
int distanceToEnd(IteratorG it){
    int i = 1;                                  // befcur as a after cursor node so i start from 1
    if(it->aftcur == NULL)  return 0;           // if there is no node after cursor return 0
    Node *temp = it->aftcur;                    
    while(temp->next != NULL) {                 //use a while loop to calculate the distance to end
        i++;                                    // stop untill at the end of list
        temp = temp->next;
    }
	return i;
}
void reset(IteratorG it){
	while(it->befcur != NULL) {                 // use a while loop to move the cursor inverted once for one node
	    it->aftcur = it->befcur;                // stop untill at the beginning of the list
	    it->befcur = it->befcur->prev;
    }  
}
void freeIt(IteratorG it){
    Node *temp = it->first;
    while(temp != NULL) {                       // use a while loop to free every elements in the list from head
        Node *todelete = temp;                  // stop untill at the end of the list
        temp = temp->next;        
        it->freeFp(todelete->value);
        free(todelete);                         
    }
    free(it);                                   // free the strcut it
}
// --------------------------------------




