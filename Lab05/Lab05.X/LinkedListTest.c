/*
 * File:   LinkedListTest.c
 * Author: Stephanie Lin
 *
 * Created on May 3, 2019, 2:41 PM
 */

#include "BOARD.h"
#include "xc.h"
#include "LinkedList.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    BOARD_Init();
    struct ListItem* LinkedList;
    struct ListItem* LinkedList1;
    struct ListItem* LinkedList2;
    struct ListItem* LinkedList3;
    int z;
    char *a;
    //char a = 'b';

    LinkedList1 = LinkedListNew("a");
//    if ((strcmp(LinkedList->data, "a")) == 0) {//, then test passed
//        //        printf("\npassed1\n");
//    }

    LinkedList2 = LinkedListCreateAfter(LinkedList1, "b");
//    if ((strcmp(LinkedList->data, "b")) == 0) {//, then test passed
//        //        printf("\npassed3\n");
//    }

    LinkedList3 = LinkedListCreateAfter(LinkedList2, "c");
//    if ((strcmp(LinkedList->data, "c")) == 0) {//, then test passed
//        //        printf("\npassed6\n");
//    }

//    if (LinkedListSize(LinkedList) == 3) {//, then test passed
//                printf("\npassed4\n");
//    }
//    LinkedList = LinkedListGetFirst(LinkedList);
//    if ((strcmp(LinkedList->data, "aaaa")) == 0) {//, then test passed
//        //        printf("\npassedGetFirst\n");
//    }
//    LinkedList2 = LinkedListGetLast(LinkedList);
//    if ((strcmp(LinkedList2->data, "aaaa6")) == 0) {//, then test passed
//        //        printf("\npassedGetLast\n");
//    }
//    //    printf("\nLinkedListLast %s \n",LinkedList->data);
//
//
//    z = LinkedListSwapData(LinkedList, LinkedList2);
//    //      printf("\nLinkedList2 %s \n",LinkedList2->data); 
//    //      printf("\nLinkedList1 %s \n",LinkedList->data); 
//
//    //    printf("\n%d\n",z);
//     LinkedList = LinkedListGetFirst(LinkedList2);
    printf("Print Linked List\n");
    printf("%s: linkedlist1 data should be a\n",LinkedList1->data);
    LinkedListPrint(LinkedList1);
    printf("%s: linkedlist2 data should be b\n",LinkedList2->data);
    LinkedListPrint(LinkedList2);
    printf("we remove a item\n");
    a = LinkedListRemove(LinkedList2);
//    printf("Print the removed List item\n");
//    printf("%s: should be b\n",LinkedList2);
//    printf("Print the removed List item\n");
//    printf("%s",a);
//    LinkedListPrint(LinkedList1);
    printf("show removed linkedlist item");
    //LinkedListPrint(a);//
    printf("print linkedlist1\n");
    LinkedListPrint(LinkedList1);
    printf("print linkedlist2\n");
    LinkedListPrint(LinkedList2);
    //LinkedListPrint(LinkedList3);
//    if (LinkedList->nextItem == NULL) {
//        printf("\npassed2\n");
//        //        printf("%c", a);
//    } else {
//        printf("failed");
//    }
    while (1);
    return 0;
}
