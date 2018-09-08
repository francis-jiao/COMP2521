// testList.c - testing DLList data type
// Written by John Shepherd, March 2013
//z5142125 mingfang jiao
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"

typedef struct DLListNode {
	char   *value;  // value of this list item (string)
	struct DLListNode *prev;
	               // pointer previous node in list
	struct DLListNode *next;
	               // pointer to next node in list
} DLListNode;

typedef struct DLListRep {
	int  nitems;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *curr;  // current node in list
	DLListNode *last;  // last node in list
} DLListRep;

void printstate(DLList);

int main(int argc, char *argv[])
{
	DLList myList;
	myList = getDLList(stdin);
	assert(validDLList(myList));
    printstate(myList);
    printf("\n");
    //test DLListBefore function
    printf("test 1 :\ntest DLListBefore function by inserting an item: \"this is the insert for test 1\" \n");
    char *test1 = "this is the insert for test 1";
    DLListBefore(myList,test1);
    printf("test1: after inserting the item the state of text: \n");
    printf("\n");
    printstate(myList);
	assert(validDLList(myList));
    printf("\n");
	//test DLListAfter function
	printf("test 2 :\ntest DLListAfter function by inserting an item: \"this is the insert for test 2\" \n");
    char *test2 = "this is the insert for test 2";
    DLListAfter(myList,test2);
    printf("test2: after inserting the item the state of text: \n");
    printf("\n");
    printstate(myList);
	assert(validDLList(myList));
	//test DLListDelete function
    printf("\n");
	printf("test 3 :\ntest DLListDelete function by deleting the current item \n");
    printf("the current item should be \"this is the insert for test 2\" \n");
    DLListDelete(myList);
    printf("after the operation by test3 the state of text :\n");
    printf("\n");
    printstate(myList);
	assert(validDLList(myList));
	return 0;
}



void printstate(DLList myList) {
    if(myList->nitems == 0) {
        printf("the is an empty text\n");
    }else{
        printf("the number of text lines : %d\n",myList->nitems);
        printf("the current line is : %s\n",myList->curr->value);
        printf("the whole text :\n");
        putDLList(stdout, myList);
    }
}
    
