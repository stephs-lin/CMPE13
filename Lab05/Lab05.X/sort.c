//>>> added by autochecker: sort.c mnlichte, Lab05, Lab05_submission_2
// Heap size 1024 required!

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "LinkedList.h"


// **** Declare any function prototypes here ***
ListItem *CreateUnsortedList(void); //this function has been written for you
ListItem *SelectionSort(ListItem* list);
ListItem *InsertionSort(ListItem* list);

/* Students should not need to modify main! */
int main(void) {
    BOARD_Init();

    printf("\n\nWelcome to slin41's sort.c, compiled on %s %s.\n", __DATE__, __TIME__);

    ListItem* listToSort;

    //Do a timing test 
    printf("\nStarting stopwatch for selection sort...\n");
    listToSort = CreateUnsortedList();
    __asm("nop"); //put a breakpoint on this line
    SelectionSort(listToSort);
    __asm("nop"); //put a breakpoint on this line, and record the stopwatch time
    LinkedListPrint(listToSort);



    printf("\nStarting stopwatch for insertion sort...\n");
    listToSort = CreateUnsortedList();
    __asm("nop"); //put a breakpoint on this line
    InsertionSort(listToSort);
    __asm("nop"); //put a breakpoint on this line, and record the stopwatch time
    LinkedListPrint(listToSort);


    while (1);
    return 0;
}

/**
 * SelectionSort() performs a selection sort on a passed to it.  The list is sorted alphabetically,
 * as determined by string.h's strcmp().  
 *
 * @param list Any element in a linked list to sort.  
 * @return a pointer to the head of the sorted list.
 * 
 * SelectionSort() assumes that no list items have a NULL data member.  It also assumes that 'list'
 * is a valid, non-NULL pointer to a linked list.
 * 
 * This function does not print.
 */
ListItem *SelectionSort(ListItem* list) {
    struct ListItem* firstSortedItem;
    struct ListItem* lastUnsortedItem;
    struct ListItem* insertAfter;
    struct ListItem* cmp;
    firstSortedItem = LinkedListGetFirst(list);
    while (firstSortedItem->nextItem != NULL) {
        cmp = firstSortedItem->nextItem; //skips this line
        while (cmp != NULL) {
            if ((strcmp(firstSortedItem->data, cmp->data)) > 0) { //firstsorted > cmp
                LinkedListSwapData(firstSortedItem, cmp);
            }
            cmp = cmp->nextItem;
        }
        firstSortedItem = firstSortedItem->nextItem;
    }
    //return LinkedListGetFirst(list);
}

/**
 * InsertionSort() performs an insertion sort on a passed to it.  The list is sorted alphabetically,
 * as determined by string.h's strcmp().  
 *
 * @param list Any element in a linked list to sort.  
 * @return a pointer to the head of the sorted list.
 * 
 * SelectionSort() assumes that no list items have a NULL data member.  It also assumes that 'list'
 * is a valid, non-NULL pointer to a linked list.
 * 
 * This function does not print.
 */
ListItem *InsertionSort(ListItem* list) {
    struct ListItem* firstSortedItem;
    struct ListItem* lastUnsortedItem;
    struct ListItem* insertAfter;
    char *savedData;
    firstSortedItem = LinkedListGetLast(list); //   First value already sorted
    while (firstSortedItem->previousItem != NULL) { //    While haven't gone through entire list
        lastUnsortedItem = firstSortedItem->previousItem; //    Make second to last item lastUnsortedItem
        if ((strcmp(firstSortedItem->data, lastUnsortedItem->data)) > 0) {//    If lastUnSortedItem is less than firstSortedItem         
            firstSortedItem = lastUnsortedItem; //    Then the lastUnsortedItem becomes firstSortedItem
        } else {//    If lastUnSortedItem is greater than firstSortedItem
            insertAfter = firstSortedItem; //    Make insertAfter to point to firstSortedItem;
            while (insertAfter->nextItem != NULL) {
                //    Keep moving right until insertAfter is less than lastUnsortedItem
                //    Or until insertAfter reaches the end of the list
                if ((strcmp(insertAfter->data, lastUnsortedItem->data)) > 0) {
                    break;
                } else {
                    insertAfter = insertAfter->nextItem;

                }

            }
            savedData = LinkedListRemove(lastUnsortedItem);
            LinkedListCreateAfter(insertAfter, savedData);
        }
    }
}

/* CreateUnsortedList() uses the functions in the LinkedList library to
 * generate an unsorted list.  
 * 
 * Students should not modify this function!
 * If an error occurs in this function, there is almost certainly 
 * a bug in your implementation of LinkedList.h.
 *   
 * @return A pointer to a ListItem which is the head of a new unsorted list. 
 */
ListItem *CreateUnsortedList(void)
/// <editor-fold defaultstate="collapsed" desc="CreateUnsortedList definition">
{
    //    char* wordList[] = {"decide", "toothpaste", "lowly", "robin", "reign", "crowd", "pies", "reduce", "tendency", "surround", "finger", "rake", "alleged", "hug", "nest", "punishment", "eggnog", "side", "beef", "exuberant", "sort", "scream", "zip", "hair", "ragged", "damage", "thought", "jump", "frequent", "substance", "head", "step", "faithful", "sidewalk", "pig", "raspy", "juggle", "shut", "maddening", "rock", "telephone", "selective", NULL};
    char* wordList[] = {"D", "A", "C", "E", "B", NULL};

    int i = 0;
    ListItem* head = LinkedListNew(wordList[i]);
    ListItem* tail = head;
    for (i = 1; wordList[i] != NULL; i++) {
        tail = LinkedListCreateAfter(tail, wordList[i]);
    }
    return head;
}
// </editor-fold>
