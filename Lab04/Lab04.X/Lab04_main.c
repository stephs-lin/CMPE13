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

// **** Set macros and preprocessor directives ****
// The lab calls for 60 characters of user input
#define MAX_INPUT_LENGTH 60

int main() {
    BOARD_Init();

    //What follows is starter code.  You will need to modify it!
    int error;
    char rpn_sentence[MAX_INPUT_LENGTH + 2];
    //    char rpn_sentence; //[]= "-2 3 + 5 /\n";
    double *result;

    printf("Welcome to slin41'S RPN calculator.  Compiled on %s %s", __DATE__, __TIME__);
//    while (1) {

        printf("Enter floats and + - / * in RPN format:\n");

        fgets(rpn_sentence, sizeof (rpn_sentence), stdin);

//        ProcessBackspaces(rpn_sentence);

        error = RPN_Evaluate(rpn_sentence, result);
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
    } else {
        printf("   Success!\n");
    }

        printf("result = %d\n", *result);

//    }

        while (1);
}
