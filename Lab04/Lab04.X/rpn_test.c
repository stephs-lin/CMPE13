// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "rpn.h"

int main() {
    BOARD_Init();
    int error;

    printf("\n###### Beginning slin41's rpn test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!

    char test0[] = " "; //white space
    double result0;
    double expected0 = 0;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test0);
    error = RPN_Evaluate(test0, &result0);
    if (error == RPN_ERROR_STACK_OVERFLOW) {
        printf("   Error: No more room on stack\n"); //
    } else if (error == RPN_ERROR_STACK_UNDERFLOW) { 
        printf("   Error: Not enough operands before operator\n"); //
    } else if (error == RPN_ERROR_INVALID_TOKEN) {
        printf("   Error: Invalid character in RPN string\n"); //
    } else if (error == RPN_ERROR_DIVIDE_BY_ZERO) {
        printf("   Error: Dividing by zero\n"); //
    } else if (error == RPN_ERROR_TOO_FEW_ITEMS_REMAIN) {
        printf("   Error: Invalid RPN calculation: more or less than one item in the stack\n");
    } else if (error == RPN_ERROR_TOO_MANY_ITEMS_REMAIN) {
        printf("   Error: Invalid RPN calculation: more or less than one item in the stack\n");
    } else if (result0 != expected0) {
        printf("   Failed, expected = %f , result = %f\n", expected0, result0);
    } else {
        printf("   Success!\n");
    }

    printf("Testing ProcessBackspaces:\n");
    char test_pb1[] = "123\b34";
    ProcessBackspaces(test_pb1);
    printf("result    : %s\n", test_pb1);
    printf("should be : 1234\n");

    while (1);
}


