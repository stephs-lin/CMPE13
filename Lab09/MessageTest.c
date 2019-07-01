/*
 * File:   MessageTest.c
 * Author: Stephanie Lin
 *
 * Created on June 2, 2019, 6:37 PM
 */

#include <stdio.h>
#include "xc.h"
#include "Message.h"
#include "BOARD.h"

static const char *PARSE_TEST_1 = "RES,1,2,3";
static const char *PARSE_TEST_2 = "SHO,73,5";
static const char *CHECKSUM_TEST_1 = "$RES,1,2,3";
static const char *CHECKSUM_TEST_2 = "$CHA,5";
static Message ENCODE_TEST_1 = {MESSAGE_RES, 1, 2, 3};
static Message ENCODE_TEST_2 = {MESSAGE_CHA, 1, 5};
static uint8_t CHECKSUM_RESULT_1;
static uint8_t CHECKSUM_RESULT_2;
static int PARSE_RESULT_1;
static int PARSE_RESULT_2;
static int ENCODE_RESULT_1;
static int ENCODE_RESULT_2;
//static BB_Event ship = {BB_EVENT_RES_RECEIVED, 0, 0, 0};
BB_Event cruise;
BB_Event message_event;
//BB_Event *message_string;

int main(void)
{
    BOARD_Init();
    int result;
    char message_string;

    /* START OF TEST HARNESS FOR MESSAGE_CALCULATECHECKSUM*/
    CHECKSUM_RESULT_1 = Message_CalculateChecksum(CHECKSUM_TEST_1);
    printf("\nMessage_CalculateChecksum Test 1 OUTPUT: 0x%x", CHECKSUM_RESULT_1);
    printf("\nMessage_CalculateChecksum Test 1 EXPECTED OUTPUT: 0x74");

    CHECKSUM_RESULT_2 = Message_CalculateChecksum(CHECKSUM_TEST_2);
    printf("\nMessage_CalculateChecksum Test 2 OUTPUT: 0x%x", CHECKSUM_RESULT_2);
    printf("\nMessage_CalculateChecksum Test 2 EXPECTED OUTPUT: 0x7F");

    /* END OF TEST HARNESS FOR MESSAGE_CALCULATECHECKSUM*/



    /* START OF TEST HARNESS FOR MESSAGE_PARSEMESSAGE*/

    PARSE_RESULT_1 = Message_ParseMessage(PARSE_TEST_1, "74\0", &message_event);
    printf("\nMessage_ParseMessage Test 1 EXPECTED OUTPUT: successful");
    if (PARSE_RESULT_1 != 1) {
        printf("\nMessage_ParseMessage Test 1 successful");
    } else {
        printf("\nMessage_ParseMessage Test 1 failed");
    }

    PARSE_RESULT_2 = Message_ParseMessage(PARSE_TEST_2, "65\0", &message_event);
    printf("\nMessage_ParseMessage Test 1 EXPECTED OUTPUT: successful");
    if (PARSE_RESULT_2 != 0) {
        printf("\nMessage_ParseMessage Test 2 successful");
    } else {
        printf("\nMessage_ParseMessage Test 2 failed");
    }

    /* END OF TEST HARNESS FOR MESSAGE_PARSEMESSAGE*/



    /* START OF TEST HARNESS FOR MESSAGE_ENCODE*/

    ENCODE_RESULT_1 = Message_Encode(&message_string, ENCODE_TEST_1);
    printf("\nOUTPUT: %d", ENCODE_RESULT_1);
    printf("\nEXPECTED OUTPUT: 14");
    printf("\nMESSAGE_STRING: %s", &message_string);

    if (ENCODE_RESULT_1 == 14) {
        printf("\nMessage_Encode Test 1 successful");
    } else {
        printf("\nMessage_Encode Test 1 failed");
    }

    ENCODE_RESULT_2 = Message_Encode(&message_string, ENCODE_TEST_2);
    printf("\nOUTPUT: %d", ENCODE_RESULT_2);
    printf("\nEXPECTED OUTPUT: 10");
    printf("\nMESSAGE_STRING: %s", &message_string);

    if (ENCODE_RESULT_2 == 10) {
        printf("\nMessage_Encode Test 2 successful");
    } else {
        printf("\nMessage_Encode Test 2 failed");
    }

    /* END OF TEST HARNESS FOR MESSAGE_ENCODE*/


    /* FIRST TEST FOR MESSAGE_DECODE
     SHOULD RETURN SUCCESSFUL
     */

    Message_Decode('$', &cruise);
    Message_Decode('R', &cruise);
    Message_Decode('E', &cruise);
    Message_Decode('V', &cruise);
    Message_Decode(',', &cruise);
    Message_Decode('1', &cruise);
    Message_Decode('*', &cruise);
    Message_Decode('7', &cruise);
    Message_Decode('0', &cruise);
    result = Message_Decode('\n', &cruise);
    if (result != 1) {
        printf("\nMessage_Decode Test 1 Successful");
    } else {
        printf("\nMessage_Decode Test 1 Failed");
    }

    /* SECOND TEST FOR MESSAGE_DECODE
     SHOULD RETURN UNSUCCESSFUL
     */
    Message_Decode('$', &cruise);
    Message_Decode('$', &cruise);
    Message_Decode('R', &cruise);
    Message_Decode('E', &cruise);
    Message_Decode('S', &cruise);
    Message_Decode(',', &cruise);
    Message_Decode('1', &cruise);
    Message_Decode(',', &cruise);
    Message_Decode('2', &cruise);
    Message_Decode(',', &cruise);
    Message_Decode('3', &cruise);
    Message_Decode('*', &cruise);
    Message_Decode('7', &cruise);
    Message_Decode('0', &cruise);
    result = Message_Decode('\n', &cruise);
    if (result != 0) {
        printf("\nMessage_Decode Test 2 Successful");
    } else {
        printf("\nMessage_Decode Test 2 Failed");
    }

    while (1);

}
