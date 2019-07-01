/*
 * File:   Message.c
 * Author: Stephanie Lin
 *
 * Created on June 2, 2019, 4:20 PM
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Message.h"
#include "xc.h"
#include "BOARD.h"
#include "BattleBoats.h"

#define Checksum_string_length 2
#define BITMASK8 0x08
#define BITMASK4 0x04
#define BITMASK2 0x02
#define BITMASK1 0x01
#define ASCII_A 65
#define ASCII_F 70
#define ASCII_Z 90
#define ASCII_0 48
#define ASCII_9 57

typedef enum {
    waiting_for_delimiter = 1,
    recording_payload = 2,
    recording_checksum = 3,
} DecodeStates;

struct DecodeData {
    DecodeStates state;
    uint8_t checksum_value;
};

static struct DecodeData decode = {waiting_for_delimiter, 15};
static BB_Event boat = {BB_EVENT_NO_EVENT, 0, 0, 0};
static BB_Event vessel;
static const char payload[] = "$%s,%d,%d,%d*%d\n";
static char payload_array[] = {};
static int payload_pointer;
static char checksum_array[2] = {};
static int checksum_pointer;
static char string_l[50];
static char string_2[50];
static char checksum_string[100];
static char message_string[MESSAGE_MAX_LEN];
static char temp[MESSAGE_MAX_PAYLOAD_LEN + 1];
static char temp2[MESSAGE_MAX_PAYLOAD_LEN + 1];
static char temp3[MESSAGE_MAX_PAYLOAD_LEN + 1];
static int parse_result;
static uint8_t checksum_result;
static uint8_t string_length;

/**
 * Given a payload string, calculate its checksum
 * 
 * @param payload       //the string whose checksum we wish to calculate
 * @return   //The resulting 8-bit checksum 
 */

uint8_t Message_CalculateChecksum(const char* payload) {
    /* calculate string length of payload */
    string_length = strlen(payload);
    uint8_t result = 0;
    int i = 0;
    /* go through each string and XOR each char */
    for (i = 0; i < string_length; i++) {
        if (payload[i] == '\n') {
        } else if (payload[i] == '$') {
        } else if (payload[i] == ',') {
        } else {
            if (i == 0) {
                result = payload[i];
            } else {
                result = payload[i]^result;
            }
        }
    }
    return result;
}

/**
 * Given a payload and its checksum, verify that the payload matches the checksum
 * 
 * @param payload       //the payload of a message
 * @param checksum      //the checksum (in string form) of  a message,
 *                          should be exactly 2 chars long, plus a null char
 * @param message_event //An event corresponding to the parsed message.
 *                      //If the message could be parsed successfully,
 *                          message_event's type will correspond to the message type and 
 *                          its parameters will match the message's data fields.
 *                      //If the message could not be parsed,
 *                          message_events type will be BB_EVENT_ERROR
 * 
 * @return STANDARD_ERROR if:
 *              the payload does not match the checksum
 *              the checksum string is not two characters long
 *              the message does not match any message template
 *          SUCCESS otherwise
 * 
 * Please note!  sscanf() has a couple compiler bugs that make it a very
 * unreliable tool for implementing this function. * 
 */
int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event) {
    uint8_t checksum_result;
    uint8_t checksum_var2;
    uint8_t calculated_checksum;
    int i;
    int j = 0;
    int comma_flag = 0;
    /* call function Message_CalculateCheck to calculate checksum of payload */
    calculated_checksum = Message_CalculateChecksum(payload);
    /* check for event type by checking first three chars of payload */
    if ((payload[0] == 'C') && (payload[1] == 'H') && (payload[2] == 'A')) {
        message_event->type = BB_EVENT_CHA_RECEIVED;
        //        boat.type = BB_EVENT_CHA_RECEIVED;
        /* go through rest of the chars in payload */
        for (i = 4; i < MESSAGE_MAX_PAYLOAD_LEN; i++) {
            /* check if char in payload is a number */
            if ((payload[i] >= ASCII_0) && (payload[i] <= ASCII_9)) {
                /* store char in a temp array */
                temp[j] = payload[i];
                /* move pointer to next space */
                j++;
            } else {
                /* insert null character at the end */
                temp[j] = '\0';
                /* set i to the end to exit out of for loop */
                i = MESSAGE_MAX_PAYLOAD_LEN;
            }
        }
        /* convert string into integer and store integer into param */
        message_event->param0 = atoi(temp);
        //        boat.param0 = atoi(temp);
    } else if ((payload[0] == 'A') && (payload[1] == 'C') && (payload[2] == 'C')) {
        message_event->type = BB_EVENT_ACC_RECEIVED;
        //        boat.type = BB_EVENT_CHA_RECEIVED;
        for (i = 4; i < MESSAGE_MAX_PAYLOAD_LEN; i++) {
            if ((payload[i] >= ASCII_0) && (payload[i] <= ASCII_9)) {
                temp[j] = payload[i];
                j++;
            } else {
                temp[j] = '\0';
                i = MESSAGE_MAX_PAYLOAD_LEN;
            }
        }
        message_event->param0 = atoi(temp);
        //        boat.param0 = atoi(temp);
    } else if ((payload[0] == 'R') && (payload[1] == 'E') && (payload[2] == 'V')) {
        message_event->type = BB_EVENT_REV_RECEIVED;
        //        boat.type = BB_EVENT_CHA_RECEIVED;
        for (i = 3; i < MESSAGE_MAX_PAYLOAD_LEN; i++) {
            if ((payload[i] >= ASCII_0) && (payload[i] <= ASCII_9)) {
                temp[j] = payload[i];
                j++;
            } else {
                temp[j] = '\0';
                i = MESSAGE_MAX_PAYLOAD_LEN;
            }
        }
        message_event->param0 = atoi(temp);
        //        boat.param0 = atoi(temp);
    } else if ((payload[0] == 'S') && (payload[1] == 'H') && (payload[2] == 'O')) {
        message_event->type = BB_EVENT_SHO_RECEIVED;
        //        boat.type = BB_EVENT_CHA_RECEIVED;
        for (i = 4; i < MESSAGE_MAX_PAYLOAD_LEN; i++) {
            /* check for commas, and skip */
            if (payload[i] == ',') {
                comma_flag++;
                i++;
            }
            /* if no comma, continue store into temp array */
            if (comma_flag == 0) {
                if ((payload[i] >= ASCII_0) && (payload[i] <= ASCII_9)) {
                    temp[j] = payload[i];
                    j++;
                } else {
                    temp[j] = '\0';
                    i = MESSAGE_MAX_PAYLOAD_LEN;
                }
                /* if comma exists, store into second temp array */
            } else if (comma_flag == 1) {
                if ((payload[i] >= ASCII_0) && (payload[i] <= ASCII_9)) {
                    temp2[j] = payload[i];
                    j++;
                } else {
                    temp2[j] = '\0';
                    i = MESSAGE_MAX_PAYLOAD_LEN;
                }
            }
        }
        message_event->param0 = atoi(temp);
        message_event->param1 = atoi(temp2);
        //        boat.param0 = atoi(temp);
        //        boat.param1 = atoi(temp2);
    } else if ((payload[0] == 'R') && (payload[1] == 'E') && (payload[2] == 'S')) {
        message_event->type = BB_EVENT_RES_RECEIVED;
        //        boat.type = BB_EVENT_CHA_RECEIVED;
        for (i = 4; i < MESSAGE_MAX_PAYLOAD_LEN; i++) {
            if (payload[i] == ',') {
                comma_flag++;
                i++;
            }
            if (comma_flag == 0) {
                if ((payload[i] >= ASCII_0) && (payload[i] <= ASCII_9)) {
                    temp[j] = payload[i];
                    j++;
                } else {
                    temp[j] = '\0';
                    i = MESSAGE_MAX_PAYLOAD_LEN;
                }
            } else if (comma_flag == 1) {
                if ((payload[i] >= 48) && (payload[i] <= 57)) {
                    temp2[j] = payload[i];
                    j++;
                } else {
                    temp2[j] = '\0';
                    i = MESSAGE_MAX_PAYLOAD_LEN;
                }
                /* store in third temp array */
            } else if (comma_flag == 2) {
                if ((payload[i] >= 48) && (payload[i] <= 57)) {
                    temp3[j] = payload[i];
                    j++;
                } else {
                    temp3[j] = '\0';
                    i = MESSAGE_MAX_PAYLOAD_LEN;
                }
            }
        }

        message_event->param0 = atoi(temp);
        message_event->param1 = atoi(temp2);
        message_event->param2 = atoi(temp3);
        //        boat.param0 = atoi(temp);
        //        boat.param1 = atoi(temp2);
        //        boat.param2 = atoi(temp3);
    } else {
        //                boat.type = BB_EVENT_CHA_RECEIVED;
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    /* check if first char of checksum_string is a hex value and store into a variable */
    if (checksum_string[0] == 'F') {
        checksum_result = 15;
    } else if (checksum_string[0] == 'E') {
        checksum_result = 14;
    } else if (checksum_string[0] == 'D') {
        checksum_result = 13;
    } else if (checksum_string[0] == 'C') {
        checksum_result = 12;
    } else if (checksum_string[0] == 'B') {
        checksum_result = 11;
    } else if (checksum_string[0] == 'A') {
        checksum_result = 10;
    } else if (checksum_string[0] == '9') {
        checksum_result = 9;
    } else if (checksum_string[0] == '8') {
        checksum_result = 8;
    } else if (checksum_string[0] == '7') {
        checksum_result = 7;
    } else if (checksum_string[0] == '6') {
        checksum_result = 6;
    } else if (checksum_string[0] == '5') {
        checksum_result = 5;
    } else if (checksum_string[0] == '4') {
        checksum_result = 4;
    } else if (checksum_string[0] == '3') {
        checksum_result = 3;
    } else if (checksum_string[0] == '2') {
        checksum_result = 2;
    } else if (checksum_string[0] == '1') {
        checksum_result = 1;
    } else if (checksum_string[0] == '0') {
        checksum_result = 0;
    }
    /* check if second char of checksum_string is a hex value and store into a variable */
    if (checksum_string[1] == 'F') {
        checksum_var2 = 15;
    } else if (checksum_string[1] == 'E') {
        checksum_var2 = 14;
    } else if (checksum_string[1] == 'D') {
        checksum_var2 = 13;
    } else if (checksum_string[1] == 'C') {
        checksum_var2 = 12;
    } else if (checksum_string[1] == 'B') {
        checksum_var2 = 11;
    } else if (checksum_string[1] == 'A') {
        checksum_var2 = 10;
    } else if (checksum_string[1] == '9') {
        checksum_var2 = 9;
    } else if (checksum_string[1] == '8') {
        checksum_var2 = 8;
    } else if (checksum_string[1] == '7') {
        checksum_var2 = 7;
    } else if (checksum_string[1] == '6') {
        checksum_var2 = 6;
    } else if (checksum_string[1] == '5') {
        checksum_var2 = 5;
    } else if (checksum_string[1] == '4') {
        checksum_var2 = 4;
    } else if (checksum_string[1] == '3') {
        checksum_var2 = 3;
    } else if (checksum_string[1] == '2') {
        checksum_var2 = 2;
    } else if (checksum_string[1] == '1') {
        checksum_var2 = 1;
    } else if (checksum_string[1] == '0') {
        checksum_var2 = 0;
    }
    if (checksum_string[2] != '\0') {
        return STANDARD_ERROR;
    }
    /*bit shift left checksum by 1 */
    checksum_result = checksum_result << 1;
    /* check if second check sum variable's last bit is a 1 */
    if (checksum_var2 & BITMASK8) {
        /* or 1 to the last bit of checksum_result  */
        checksum_result = checksum_result | BITMASK1;
    }
    checksum_result = checksum_result << 1;
    if (checksum_var2 & BITMASK4) {
        checksum_result = checksum_result | BITMASK1;
    }
    checksum_result = checksum_result << 1;
    if (checksum_var2 & BITMASK2) {
        checksum_result = checksum_result | BITMASK1;
    }
    checksum_result = checksum_result << 1;
    if (checksum_var2 & BITMASK1) {
        checksum_result = checksum_result | BITMASK1;
    }
    /* storing check_sum result */
    decode.checksum_value = checksum_result;
    if (checksum_result == calculated_checksum) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. 
 * 
 * The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * 
 * @param message            The character array used for storing the output. 
 *                              Must be long enough to store the entire string,
 *                              see MESSAGE_MAX_LEN.
 * @param message_to_encode  A message to encode
 * @return                   The length of the string stored into 'message_string'.
                             Return 0 if message type is MESSAGE_NONE.
 */
int Message_Encode(char *message_string, Message message_to_encode) {
    message_string[MESSAGE_MAX_LEN];
    /* check for message_to_encode type */
    if (MESSAGE_CHA == message_to_encode.type) {
        /* wrap payload template with message template */
        sprintf(string_l, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
        sprintf(string_2, MESSAGE_TEMPLATE, string_l, decode.checksum_value);
        strcpy(message_string, string_2);
        /* return the length of message */
        return strlen(string_2);
    } else if (MESSAGE_ACC == message_to_encode.type) {
        sprintf(string_l, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
        sprintf(string_2, MESSAGE_TEMPLATE, string_l, decode.checksum_value);
        strcpy(message_string, string_2);
        return strlen(string_2);
    } else if (MESSAGE_REV == message_to_encode.type) {
        sprintf(string_l, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
        sprintf(string_2, MESSAGE_TEMPLATE, string_l, decode.checksum_value);
        strcpy(message_string, string_2);
        return strlen(string_2);
    } else if (MESSAGE_SHO == message_to_encode.type) {
        sprintf(string_l, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0,
                message_to_encode.param1);
        sprintf(string_2, MESSAGE_TEMPLATE, string_l, decode.checksum_value);
        strcpy(message_string, string_2);
        return strlen(string_2);
    } else if (MESSAGE_RES == message_to_encode.type) {
        sprintf(string_l, PAYLOAD_TEMPLATE_RES, message_to_encode.param0,
                message_to_encode.param1, message_to_encode.param2);
        sprintf(string_2, MESSAGE_TEMPLATE, string_l, decode.checksum_value);
        strcpy(message_string, string_2);
        return strlen(string_2);
    } else {
        message_to_encode.type = MESSAGE_NONE;
        return STANDARD_ERROR;
    }
}

/**
 * Message_Decode reads one character at a time.  If it detects a full NMEA message,
 * it translates that message into a BB_Event struct, which can be passed to other 
 * services.
 * 
 * @param char_in - The next character in the NMEA0183 message to be decoded.
 * @param decoded_message - a pointer to a message struct, used to "return" a message
 *                          if char_in is the last character of a valid message, 
 *                              then decoded_message
 *                              should have the appropriate message type.
 *                              otherwise, it should have type NO_EVENT.
 * @return SUCCESS if no error was detected
 *         STANDARD_ERROR if an error was detected
 * 
 * note that ANY call to Message_Decode may modify decoded_message.
 */
int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event) {
    int i = 0;
    //    printf("printing string %s", )
    switch (decode.state) {
        case waiting_for_delimiter:
            payload_pointer = 0;
            checksum_pointer = 0;
            i = 0;
            /* check if char_is a start delimiter */
            if (char_in == '$') {
                decode.state = recording_payload;
            } else {
                decode.state = waiting_for_delimiter;
                return STANDARD_ERROR;
            }
            break;
        case recording_payload:
            /* check if payload_array exceeds length */
            if (MESSAGE_MAX_PAYLOAD_LEN <= strlen(payload_array)) {
                return STANDARD_ERROR;
            }
            /* check if char_is *, then start recording checksum*/
            if (char_in == '*') {
                decode.state = recording_checksum;
                /* check if char is A-Z or 0-9, then store in payload_array */
            } else if (((char_in >= ASCII_A) && (char_in <= ASCII_Z)) || ((char_in >= ASCII_0) && (char_in <= ASCII_9))) {
                payload_array[payload_pointer] = char_in;
                payload_pointer++;
                decode.state = recording_payload;
                /* check if char_is an invalid character */
            } else if ((char_in == '$') || (char_in == '\n')) {
                decode.state = waiting_for_delimiter;
                return STANDARD_ERROR;
            }
            break;
        case recording_checksum:
            /* check if checksum_array exceeds length */
            if (MESSAGE_CHECKSUM_LEN <= strlen(checksum_array)) {
                if (char_in == '\n') {
                    /* if end of string, call Message_ParseMessage */
                    parse_result = Message_ParseMessage(payload_array, checksum_array, &vessel);
                    /* if parse_result = 1, then success */
                    if (parse_result != 0) {
                        decoded_message_event->type = vessel.type;
                        decode.state = waiting_for_delimiter;
                        return SUCCESS;
                        /* if parse_result = 0, then failed */
                    } else {
                        decode.state = waiting_for_delimiter;
                        return STANDARD_ERROR;
                    }

                } else {
                    return STANDARD_ERROR;
                }
            }
            /* if char_in is a hex character, then store in checksum_array */
            if (((char_in >= ASCII_A) && (char_in <= ASCII_F)) || ((char_in >= ASCII_0) && (char_in <= ASCII_9))) {
                decode.state = recording_checksum;
                checksum_array[checksum_pointer] = char_in;
                checksum_pointer++;
            } else {
                return STANDARD_ERROR;
            }
            break;
        default:
            break;
    }
}
