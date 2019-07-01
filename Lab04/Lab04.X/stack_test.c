// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "stack.h"
#include <stdio.h>

int main() {
    int i;
    double *stackItem1;
    double *stackItem2;
    double *stackItem3;
    int a;
    int b;

    BOARD_Init();

    printf("\n###### Beginning slin41's stack test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!

    printf("Does StackInit() set the currentItemIndex appropriately?\n");
    struct Stack stack = {};
    StackInit(&stack);
    if (stack.currentItemIndex == -1) {
        printf("passed");
    } else {
        printf("failed");
    }

    //    test stackInit:
    StackPush(&stack, 0);
    StackPush(&stack, 0);
    StackPush(&stack, -3);
    StackPush(&stack, 4);
    StackPush(&stack, 5);
    StackPush(&stack, 6);
    StackPush(&stack, 7);
    StackPush(&stack, 8);
    StackPush(&stack, 9);
    StackPush(&stack, 10);
    StackPush(&stack, 11);
    StackPush(&stack, 12);
    StackPush(&stack, 13);
    StackPush(&stack, 14);
    StackPush(&stack, 15);
    StackPush(&stack, 16);
    StackPush(&stack, 17);
    StackPush(&stack, 18);
    StackPush(&stack, 19);
    StackPush(&stack, 20);
    
    for (i = STACK_SIZE - 1; i >= 0; i--) {
        printf("\n%f\n", stack.stackItems[i]);
    }
    a = StackIsFull(&stack);
    b = StackGetSize(&stack);
    printf("\n%d",a);
    printf("\nsize:%d",b);
//
    StackPop(&stack, &stackItem1);
    StackPop(&stack, &stackItem2);
    StackPop(&stack, &stackItem3);
    printf("printing second time");
    for (i = STACK_SIZE - 1 ; i >= 0; i--) {
        
        printf("\n%f\n", stack.stackItems[i]);
    }
    printf("%f",stackItem1);
    printf("%f",stackItem2);
    printf("%f",stackItem3);
    
    while (1);
    return 0;
}


