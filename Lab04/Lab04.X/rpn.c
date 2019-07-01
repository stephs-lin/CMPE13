/*
 * File:   rpn.c
 * Author: Stephanie Lin
 *
 * Created on April 26, 2019, 2:45 PM
 */


#include "xc.h"
#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "rpn.h"

/* RPN_Evaluate() parses and evaluates a string that contains 
 * a valid Reverse Polish Notation string (no newlines!)  
 * @param:  rpn_string - a string in polish notation.  Tokens must be either 
 *          arithmetic operators or numbers.
 * @param:  result - a pointer to a double that will be modified to contain
 *          the return value of the rpn expression.
 * @return: error - if the rpn expression is invalid, 
 *          an appropriate error value is returned.
 * 
 * RPN_Evaluate supports the following basic arithmetic operations:
 *   + : addition
 *   - : subtraction
 *   * : multiplication
 *   / : division
 * Numeric tokens can be positive or negative, and can be integers or 
 * decimal floats.  RPN_Evaluate should be able to handle strings of 
 * at least 255 length.
 * */
static void Clear_Stack(struct Stack *stack, int tokens) {
    int i;
    double operator;
    for (i = 0; i <= tokens; i++) {
        StackPop(&stack, &operator);
    }
}

int RPN_Evaluate(char * rpn_string, double * result) {

    int string_length;
    int i = 0;
    int invalid;
    int token_count;
    int stackfull = 0;
    int div_by_zero = 0;
    int too_few = 0;
    double operand1;
    double operand2;
    double answer;
    double atof_result;
    char *token;

    struct Stack stack = {};
    StackInit(&stack);

    invalid = 0;

    string_length = strlen(rpn_string);
    rpn_string[string_length] = 0;
    for (i = 0; i < string_length; i++) {
        if (rpn_string[i] == '\n') {
            rpn_string[i] = '\0';
        }
    }
    token_count = 0;
    token = strtok(rpn_string, " ");

    while (token != NULL) {
        atof_result = atof(token);
        if (atof_result == 0.0 && ((0 == strcmp(token, "+")) ||
                0 == strcmp(token, "-") || 0 == strcmp(token, "*")
                || 0 == strcmp(token, "/"))) {
            StackPop(&stack, &operand1);
            StackPop(&stack, &operand2);
            if ((StackIsEmpty != 1)) {
                too_few = 1;
                break;
            }
            if (0 == (strcmp(token, "+"))) {
                answer = operand2 + operand1;
                StackPush(&stack, answer);
            } else if (0 == (strcmp(token, "-"))) {
                answer = operand2 - operand1;
                StackPush(&stack, answer);
            } else if (0 == (strcmp(token, "*"))) {
                answer = operand2 * operand1;
                StackPush(&stack, answer);
            } else if (0 == (strcmp(token, "/"))) {
                if (operand1 == 0) {
                    answer = operand2 / operand1;
                    StackPush(&stack, answer);
                } else {
                    div_by_zero = 1;
                }
            } else {
                invalid = 1;
            }
        } else if (atof_result != 0.0) {
            if (StackIsFull(&stack) != 1) {

                StackPush(&stack, atof_result);
            } else {
                stackfull = 1;
            }
        } else if (atof_result == 0.0) {
            if (StackIsFull(&stack) != 1) {

                StackPush(&stack, atof_result);
            } else {
                stackfull = 1;
            }
        } else {
            invalid = 1;
        }
        token = strtok(NULL, " ");
        token_count++;
    }

    Clear_Stack(&stack, token_count);

    *result = answer;
    printf("result = %d\n", answer);

    if (stackfull == 1) {
        return 1;
    } else if (too_few == 1) {
        return 2;
    } else if (invalid == 1) {
        return 3;
    } else if (div_by_zero == 1) {
        return 4;
    } else if ((StackGetSize(&stack) != 1) || (StackIsEmpty == 1)) {
        return 5;
    } else
        return 0;

}

/**
 * This function should read through an array of characters, checking for backspace characters. 
 * When it encounters a backspace character, it eliminates the backspace, the preceeding character
 * (if such a character exists), and shifts all subsequent characters as appropriate.
 * @param string_to_modify The string that will be processed for backspaces. 
 *        This string is modified "in place", so it is both an input and an output to the function.
 * @return Returns the size of the resulting string in "string_to_modify".
 *
 * ProcessBackspaces() should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings of at least 255 length.
 * 
// * */
int ProcessBackspaces(char *rpn_sentence) {
    int i;
    int j;
    char *dest;

    strcpy(dest, rpn_sentence);
    for (i = 0; rpn_sentence[i] != '\0'; i++) {
        if (rpn_sentence[i] == '\b') {
            i++;
            j--;
        }
        dest[j] = rpn_sentence[i];
        j++;
    }
    rpn_sentence[i] = dest[j];
}

