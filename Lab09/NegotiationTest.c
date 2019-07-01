/*
 * File:   NegotiationTest.c
 * Author: Stephanie Lin
 *
 * Created on June 2, 2019, 5:29 PM
 */

#include <stdio.h>
#include "xc.h"
#include "Negotiation.h"
#include "BOARD.h"



int main(void) {
    BOARD_Init();
    
    /*HARD CODED TEST_VARIABLES*/
    int test_variables[5] = {3, 12345, 10, 4, 1};
    NegotiationData answer = test_variables[1];
    NegotiationData answer0 = test_variables[0];
    NegotiationData result1;
    NegotiationData result0;
    NegotiationData verification;
    NegotiationData flip_result;
    NegotiationData A;
    NegotiationData B;
    
    /*TEST1 FOR NEGOTIATION HASH*/
    /*INPUT IS 12345
     OUTPUT SHOULD BE 43183*/
    printf("NEGOTIATION TEST HARNESS\n\n");
    result1 = NegotiationHash(answer);
    printf("\nValue: %d", answer);
    printf("\nResult: %d", result1);
    printf("\nExpected Value: 43183");
    
    if (result1 == 43182) {
        printf("\nSUCCESS\n");
    } else {
        printf("\nFAIL\n");
    }
    
    result0 = NegotiationHash(answer0);
    printf("\nValue: %d", answer0);
    printf("\nResult: %d", result0);
    printf("\nExpected Value: 9");
    
    if (result0 == 9) {
        printf("\nSUCCESS\n");
    } else {
        printf("\nFAIL\n");
    }
    
    /*TEST2 FOR NEGOTIATION VERIFY*/
    /*INPUT IS 12345
     OUTPUT SHOULD BE SUCCESS*/
    printf("\nVERIFY TESTS:\n");
    verification = NegotiationVerify(test_variables[1], result1);
    if (verification == 1) {
        printf("VERIFY SUCCESS\n");
    } else {
        printf("FAIL\n");
    }
    verification = NegotiationVerify(test_variables[0], result0);
    if (verification == 1) {
        printf("VERIFY SUCCESS\n\n");
    } else {
        printf("FAIL\n\n");
    }
    
    /*TEST FOR NEGOTIATECOINFLIP*/
    /*INPUTS ARE A = 52, B = 18
     OUTPUT SHOULD BE HEADS*/
    printf("COIN FLIP RESULTS:\n");
    A = 52;
    B = 18;
    flip_result = NegotiateCoinFlip(A, B);
    printf("EXPECTED OUTPUT IS TAILS");
    printf("ACTUAL OUT IS: ");
    if (flip_result == 0) {
        printf("HEADS\n");
    } else {
        printf("TAILS\n");
    }
    
    A = 1;
    B = 2;
    flip_result = NegotiateCoinFlip(A, B);
    printf("EXPECTED OUTPUT IS HEADS");
    printf("ACTUAL OUT IS: ");
    if (flip_result == 0) {
        printf("HEADS\n");
    } else {
        printf("TAILS\n");
    }
    
    while(1);
}
