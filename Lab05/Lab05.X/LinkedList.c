/*
 * File:   LinkedListTest.c
 * Author: Stephanie Lin
 *
 * Created on May 3, 2019, 2:41 PM
 */
#define lower -1
#define greater 1
#define equal 0

#include "xc.h"
#include "LinkedList.h"

static int CompareStrings(ListItem *firstItem, ListItem *secondItem) {
    if (strcmp(firstItem->data, secondItem->data) > 0) {
        return greater;
    } else if (strcmp(firstItem->data, secondItem->data) < 0) {
        return lower;
    } else
        return equal;
}

/**
 * This function starts a new linked list. Given an allocated pointer to data it will return a
 * pointer for a malloc()ed ListItem struct. If malloc() fails for any reason, then this function
 * returns NULL otherwise it should return a pointer to this new list item. data can be NULL.
 *
 * @param data The data to be stored in the first ListItem in this new list. Can be any valid 
 *       (struct ListItem*)      pointer value.
 * @return A pointer to the malloc()'d ListItem. May be NULL if an error occured.
 */
ListItem *LinkedListNew(char *data) {
    struct ListItem *list = malloc(sizeof (struct ListItem));
    if (list == NULL) {
        return NULL;
    } else {
        list->data = data;
        list->previousItem = NULL;
        list->nextItem = NULL;
        return list;
    }
}

/**
 * This function allocates a new ListItem containing data and inserts it into the list directly
 * after item. It rearranges the pointers of other elements in the list to make this happen. If
 * passed a NULL item, CreateAfter() should still create a new ListItem, just with no previousItem.
 * It returns NULL if it can't malloc() a new ListItem, otherwise it returns a pointer to the new
 * item. The data parameter is also allowed to be NULL.
 *
 * @param item The ListItem that will be before the newly-created ListItem.
 * @param data The data the new ListItem will point to.
 * @return A pointer to the newly-malloc()'d ListItem.
 */
ListItem *LinkedListCreateAfter(ListItem *item, char *data) {
    //    ListItem *newNode = LinkedListNew(data); //CREATING A NEW POINTER?
    ListItem *newNode = malloc(sizeof (struct ListItem));
    if (newNode != NULL) {
        //        printf("\nnewNode is not NULL\n");
        if (item->nextItem == NULL) {
            //            printf("FIRST 1");
            //            printf("\nitem->nextItem == NULL\n");
            newNode->previousItem = item;
            //            printf("\nprevious item's data %s\n",item->data);
            newNode->nextItem = NULL;
            item->nextItem = newNode;
            
        } 
        else if (item == NULL){
            newNode->previousItem = NULL;
        }
        else {
            //            printf("FIRST 2");
            //            printf("\nitem->nextItem != NULL\n");
            newNode->previousItem = item;
            newNode->nextItem = item->nextItem;
            item->nextItem->previousItem = newNode;
            item->nextItem = newNode;
        }
        newNode->data = data;
        //        printf("\ndata:%s\n", newNode->data);
        //        printf("\nreturn NewNode\n");
        return newNode;
    } else {
        //        printf("\nnewNode is NULL\n");
        return NULL;
    }
}
//check if returns null pointer

/**
 * This function will remove a list item from the linked list and free() the memory that the
 * ListItem struct was using. It doesn't, however, free() the data pointer and instead return it
 * so that the calling code can manage it.  If passed a pointer to NULL, LinkedListRemove() should
 * return NULL to signal an error.
 *
 * @param item The ListItem to remove from the list.
 * @return The data pointer from the removed item. May be NULL.
 */
char *LinkedListRemove(ListItem *item) {
//    printf("entring list remove");
    //    ListItem *newNode = malloc(sizeof (struct ListItem));
    if (item->previousItem != NULL && item->nextItem != NULL) { 
        item->previousItem->nextItem = item->nextItem;
        item->nextItem->previousItem = item->previousItem;
    } else if (item->previousItem == NULL) {
        item->nextItem->previousItem = NULL;
    } else if (item->nextItem == NULL) {
        item->previousItem->nextItem = NULL;
    } else if (item->previousItem == NULL && item->nextItem == NULL) {
        return NULL;
    }
    char *temp = item->data;
    item->nextItem = NULL;
    item->previousItem = NULL;
    if (item == NULL) {
        return NULL;
    } else
//        printf("freeing item");
        free(item);
    return temp; //set a new char data pointer rearrange then free. 
}
/*four situations that you need to be able to handle:  1) removing the head of a list, 2) removing the tail, 
 * 3) removing the only item, and 4) removing a middle item*/

/**
 * This function returns the total size of the linked list. This means that even if it is passed a
 * ListItem that is not at the head of the list, it should still return the total number of
 * ListItems in the list. A NULL argument will result in 0 being returned.
 *
 * @param list An item in the list to be sized.
 * @return The number of ListItems in the list (0 if `list` was NULL).
 */
int LinkedListSize(ListItem *list) {
    int i;
    i = 1;
    //    printf("\nDeclaration: %d\n", i);
    ListItem *newNode = malloc(sizeof (struct ListItem));
    newNode = list;
    for (; newNode->previousItem != NULL;) { //IS THIS CORRECT?
        newNode = newNode->previousItem;
//        list = newNode;
        i++;
    }
    //    printf("\nNumber above: %d\n", i);
    for (; list->nextItem != NULL;) { //IS THIS CORRECT?
        list = list->nextItem;
        i++;
    }
    //    printf("\nNumber below: %d\n", i);

    if (i >= 0) {
//                printf("\nSum: %d\n", i);
        return i;
    } else
        //        printf("i < 0");
        return 0;
}

/**
 * This function returns the head of a list given some element in the list. If it is passed NULL,
 * it will just return NULL. If given the head of the list it will just return the pointer to the
 * head anyways for consistency.
 *
 * @param list An element in a list.
 * @return A pointer to the first element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetFirst(ListItem *list) {
    //    printf("\nentering get first\n");
    ListItem *newNode = malloc(sizeof (struct ListItem));
    for (; list->previousItem != NULL;) {
        //        printf("\n%s\n", list->data);
        newNode = list->previousItem;
        list = newNode;
    }
    //    newNode = list->previousItem;
    //    list = newNode;
    //    printf("\n%s\n", list->data);
    return list;
}

/**
 * This function operates identically to LinkedListGetFirst(), but returns
 * the tail of a linked list.
 * 
 * @param list An element in a list.
 * @return A pointer to the last element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetLast(ListItem *list) {
    //    printf("\nentering get last\n");
    //    printf("\n%s\n", list->data);
    //    ListItem *newNode = malloc(sizeof (struct ListItem));
    for (; list->nextItem != NULL;) {
        //        printf("\ndata:%s\n", list->data);
        list = list->nextItem;

        //        list = newNode;
    }
    //    printf("\n%s\n", list->data);
    return list;
}

/**
 * LinkedListSwapData() switches the data pointers of the two provided ListItems. This is most
 * useful when trying to reorder ListItems but when you want to preserve their location. This
 * function should return STANDARD_ERROR if either arguments are NULL, otherwise it should return
 * SUCCESS. If one or both of the data pointers are NULL in the given ListItems, it still does
 * perform the swap and returns SUCCESS.
 *
 * @param firstItem One of the items whose data will be swapped.
 * @param secondItem Another item whose data will be swapped.
 * @return SUCCESS if the swap worked or STANDARD_ERROR if it failed.
 */
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem) {
    char *temp = secondItem->data;
    secondItem->data = firstItem->data;
    firstItem->data = temp;

    if ((firstItem == NULL) | (secondItem == NULL)) {
//        printf("enrror");
        return STANDARD_ERROR;
    } else {
//        printf("success");
        return SUCCESS;
    }
}

/**
 * LinkedListPrint() prints out the complete list to stdout. This function prints out the given
 * list, starting at the head if the provided pointer is not the head of the list, like "[STRING1,
 * STRING2, ... ]" If LinkedListPrint() is called with a NULL list it does nothing, returning
 * STANDARD_ERROR. If passed a valid pointer, prints the list and returns SUCCESS.
 *
 * @param list Any element in the list to print.
 * @return SUCCESS or STANDARD_ERROR if passed NULL pointers.
 */
int LinkedListPrint(ListItem *list) {
        printf("\nentering print\n");
    LinkedListGetFirst(list);
    if (list == NULL) {
        return STANDARD_ERROR;
    } else {

        for (; list->nextItem != NULL;) {
            printf("\n%s\n", list->data);
            list = list->nextItem;
        }
        printf("\n%s\n", list->data);
        return SUCCESS;
    }
}
