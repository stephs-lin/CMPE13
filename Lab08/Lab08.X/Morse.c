/*
 * File:   Morse.c
 * Author: Stephanie Lin
 *
 * Created on May 28, 2019, 5:03 PM
 */

#include <stdio.h>
#include "xc.h"
#include "Morse.h"
#include "Buttons.h"
#include "BinaryTree.h"
#include "Leds.h"

#define treelevel 6

/*setting MorseState*/
typedef enum {
    W_WORD = 1, W_LETTER = 2, W_PULSE = 3, DOT = 4, DASH = 5
} MorseState;

/*creating struct for states*/
typedef struct {
    MorseState state;
} MorseData;

/*creating struct for timers*/
struct Timer {
    uint8_t event; //   True or False
    uint16_t timeRemaining; //    2^16 -1
};

/*initializing timers*/
static struct Timer DOT_TIMEOUT = {FALSE, MORSE_DOT_TIMEOUT};
static struct Timer LETTER_TIMEOUT = {FALSE, MORSE_LETTER_TIMEOUT};
static struct Timer WORD_TIMEOUT = {FALSE, MORSE_WORD_TIMEOUT};

/*initializing global variables and modules*/
MorseEvent morse = {MORSE_EVENT_NONE, ' '};
static MorseData morseData = {W_WORD};
static Node *root;
static Node *current_root;
static uint16_t currentEvent;
static uint8_t morseEvent;
static int counter = 0;

int MorseInit(void) {
    ButtonsInit();
    /*initializing treeData*/
    char treeData[] = "_EISH54V_3UF____2ARL_____WP__J_1TNDB6_X__KC__Y__MGZ7_Q__O_8__90\0";
    root = TreeCreate(treelevel, treeData);
    current_root = root;
    /*if root NULL, then return STANDARD_ERROR. otherwise, return SUCCESS*/
    if (root == NULL) {
        printf("\nSTANDARD_ERROR\n");
        return STANDARD_ERROR;
    } else {
        return SUCCESS;
    }
}

MorseEvent MorseDecode(MorseEvent input_event) {
    /*check if input_event.type is MORSE_EVENT_DOT*/
    if ((input_event.type == MORSE_EVENT_DOT) && (counter < treelevel)) {
        /*set leftchild to current root*/
        current_root = GetLeftChild(current_root);
        /*if current_root error, then return MORSE_EVENT_ERROR*/
        if ((current_root == NULL)) {
            morse.type = MORSE_EVENT_ERROR;
            morse.parameter = MORSE_CHAR_BAD_CHAR;
            return morse;
        }/*else return MORSE_EVENT_NONE with current node*/
        else {
            counter++;
            morse.type = MORSE_EVENT_NONE;
            morse.parameter = current_root->data;
            return morse;
        }
    }/*check if input_event.type is MORSE_EVENT_DASH*/
    else if ((input_event.type == MORSE_EVENT_DASH) && (counter < treelevel)) {
        /*set rightchild to current root*/
        current_root = GetRightChild(current_root);
        /*if current_root error, then return MORSE_EVENT_ERROR*/
        if ((current_root == NULL)) {
            morse.type = MORSE_EVENT_ERROR;
            morse.parameter = MORSE_CHAR_BAD_CHAR;
            return morse;
        }/*else return MORSE_EVENT_NONE with current node*/
        else {
            counter++;
            morse.type = MORSE_EVENT_NONE;
            morse.parameter = current_root->data;
            return morse;
        }
    }/*check if input_event.type is MORSE_EVENT_NEW_LETTER*/
    else if (input_event.type == MORSE_EVENT_NEW_LETTER && (counter < treelevel)) {
        /*return with MORSE_EVENT_CHAR_DECODED current node*/
        morse.parameter = current_root->data;
        morse.type = MORSE_EVENT_CHAR_DECODED;
        current_root = root;
        morseData.state = W_WORD;
        counter = 0;
        return morse;
    }/*check if input_event.type is MORSE_EVENT_NEW_WORD*/
    else if (input_event.type == MORSE_EVENT_NEW_WORD) {
        printf("\nENTERING INTO MORSE NEW WORD");
        /*return MORSE_EVENT_CHAR_DECODED with current node*/
        morse.parameter = current_root->data;
        morse.type = MORSE_EVENT_NEW_WORD;
        current_root = root;
        morseData.state = W_WORD;
        counter = 0;
        return morse;
    }
    return morse;
}

MorseEvent MorseCheckEvents(void) {
    DOT_TIMEOUT.timeRemaining--;
    LETTER_TIMEOUT.timeRemaining--;
    WORD_TIMEOUT.timeRemaining--;
    /*store button events*/
    currentEvent = ButtonsCheckEvents();
    /*check for morseData.state*/
    switch (morseData.state) {
        case W_WORD:
            morse.type = MORSE_EVENT_NONE;
            morseData.state = W_WORD;
            if (currentEvent) {
                /*if button event 4 down, next state is dot*/
                if (currentEvent == BUTTON_EVENT_4DOWN) {
                    currentEvent = BUTTON_EVENT_NONE;
                    morseData.state = DOT;
                    /*reset timeout*/
                    DOT_TIMEOUT.timeRemaining = MORSE_DOT_TIMEOUT;
                }
            } else {
                morseData.state = W_WORD;
            }
            break;
        case W_LETTER:
//            printf("\nW_LETTER");
//            printf("\nTIME %d", WORD_TIMEOUT.timeRemaining);
            if (currentEvent) {
                /*if button event 4 down, next state is dot*/
                if (currentEvent == BUTTON_EVENT_4DOWN) {
                    currentEvent = BUTTON_EVENT_NONE;
                    morseData.state = DOT;
                    /*reset timeout*/
                    DOT_TIMEOUT.timeRemaining = MORSE_DOT_TIMEOUT;
                    //                    WORD_TIMEOUT.timeRemaining = MORSE_WORD_TIMEOUT;
                    //                    LETTER_TIMEOUT.timeRemaining = MORSE_LETTER_TIMEOUT;
                }
            }
            /*check for time out*/
            if (WORD_TIMEOUT.timeRemaining <= 0) {
                morseData.state = W_WORD;
                morse.type = MORSE_EVENT_NEW_WORD;
            }
            else {
                morseData.state = W_LETTER;
                morse.type = MORSE_EVENT_NONE;
            }
            return morse;
            break;
        case W_PULSE:
            if (currentEvent) {
                /*if button event 4 down, next state is dot*/
                if (currentEvent == BUTTON_EVENT_4DOWN) {
                    currentEvent = BUTTON_EVENT_NONE;
                    morseData.state = DOT;
                    /*reset timeout*/
                    DOT_TIMEOUT.timeRemaining = MORSE_DOT_TIMEOUT;
                    //                    WORD_TIMEOUT.timeRemaining = MORSE_WORD_TIMEOUT;
                    //                    LETTER_TIMEOUT.timeRemaining = MORSE_LETTER_TIMEOUT;
                }
            }
            /*check for time out*/
            if (LETTER_TIMEOUT.timeRemaining <= 0) {
                morseData.state = W_LETTER;
                morse.type = MORSE_EVENT_NEW_LETTER;
            }
//            }else {
//                morseData.state = W_PULSE;
//                morse.type = MORSE_EVENT_NONE;
//            }
            break;
        case DOT:
            if (currentEvent) {
                /*if button event 4 up, next state is waiting for pulse*/
                if (currentEvent == BUTTON_EVENT_4UP) {
                    currentEvent = BUTTON_EVENT_NONE;
                    morseData.state = W_PULSE;
                    morse.type = MORSE_EVENT_DOT;
                    /*reset timeout*/
                    WORD_TIMEOUT.timeRemaining = MORSE_WORD_TIMEOUT;
                    //                    printf("\nTIME11 %d", WORD_TIMEOUT.timeRemaining );
                    LETTER_TIMEOUT.timeRemaining = MORSE_LETTER_TIMEOUT;
                    //                    DOT_TIMEOUT.timeRemaining = MORSE_DOT_TIMEOUT;
                }
            }
            /*check for time out*/
            if (DOT_TIMEOUT.timeRemaining <= 0) {
                morseData.state = DASH;
            }
            break;
        case DASH:
            if (currentEvent) {
                /*if button event 4 up, next state is waiting for pulse*/
                if (currentEvent == BUTTON_EVENT_4UP) {
                    currentEvent = BUTTON_EVENT_NONE;
                    morseData.state = W_PULSE;
                    morse.type = MORSE_EVENT_DASH;
                    /*reset timeout*/
                    WORD_TIMEOUT.timeRemaining = MORSE_WORD_TIMEOUT;
                    LETTER_TIMEOUT.timeRemaining = MORSE_LETTER_TIMEOUT;
                    return morse;
                }
            }
            break;
        default:
            morse.type = MORSE_EVENT_NONE;
            return morse;
    }
    return morse;
}