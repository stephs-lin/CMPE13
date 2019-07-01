// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

//CE13 standard libraries:
#include "Buttons.h"
#include "Uart1.h"
#include "Oled.h"

// Battleboats Libraries:
#include "BattleBoats.h"
#include "Agent.h"
#include "Negotiation.h"
#include "Message.h"
#include "Field.h"

BB_Event battleboatEvent;
Message correctMessageTest;
AgentState testState;

int main(void)
{
    BOARD_Init();
    //Set up LEDs:
    TRISE = 0;
    LATE = 0;

    //initialize CE13 libraries:
    ButtonsInit();
    OledInit();

    //Print a greeting:
    OledDrawString("This is BattleBoats!\nPress BTN4 to\nchallenge, or wait\nfor opponent.");
    OledUpdate();

    //Initialize Agent module:
    //    BB_EVENT_NO_EVENT, //0
    //    BB_EVENT_START_BUTTON, //1
    //    BB_EVENT_RESET_BUTTON, //2
    //    BB_EVENT_CHA_RECEIVED, //3
    //    BB_EVENT_ACC_RECEIVED, //4
    //    BB_EVENT_REV_RECEIVED, //5
    //    BB_EVENT_SHO_RECEIVED, //6
    //    BB_EVENT_RES_RECEIVED, //7
    //    BB_EVENT_MESSAGE_SENT, //8
    //    BB_EVENT_ERROR, //9
    //--------------------------------
    //    MESSAGE_NONE, //used if no message is to be sent
    //    MESSAGE_CHA,
    //    MESSAGE_ACC,
    //    MESSAGE_REV,
    //    MESSAGE_SHO,
    //    MESSAGE_RES,
    // AGENT_STATE_
    AgentInit();
    int i;
    int tailsPassed = 0;
    int headsPassed = 0;
    int counter = 0;
    for (i = 0; i < 50; i++) {
        battleboatEvent.type = BB_EVENT_ACC_RECEIVED;
        AgentSetState(AGENT_STATE_CHALLENGING);
        correctMessageTest = AgentRun(battleboatEvent);
        testState = AgentGetState();
        if (correctMessageTest.type == MESSAGE_REV && testState == AGENT_STATE_WAITING_TO_SEND) {
            //printf("success\n");
            headsPassed++;
            counter++;
        } else if (correctMessageTest.type == MESSAGE_REV && testState == AGENT_STATE_DEFENDING) {
            //printf("success\n");
            tailsPassed++;
            counter++;
        }
    }
    if (headsPassed + tailsPassed == 50) {
        printf("Challenging Test Passed\n");
        printf("Num tails Passed %d\n", tailsPassed);
        printf("Num heads Passed %d\n", headsPassed);
    } else {
        printf("Num tails Passed %d\n", tailsPassed);
        printf("Num heads Passed %d\n", headsPassed);
    }
    printf("counter is %d\n", counter);
    printf("-------------------------------------------\n");
    int headsPassed2 = 0;
    int tailsPassed2 = 0;
    int j;
    for (j = 0; j < 50; j++) {
        battleboatEvent.type = BB_EVENT_REV_RECEIVED;
        AgentSetState(AGENT_STATE_ACCEPTING);
        correctMessageTest = AgentRun(battleboatEvent);
        testState = AgentGetState();
        if (correctMessageTest.type == 4 && testState == AGENT_STATE_ATTACKING) {
            tailsPassed2++;
        } else if (testState == AGENT_STATE_DEFENDING) {
            headsPassed2++;
        }
    }
    if (headsPassed + tailsPassed == 50) {
        printf("Accepting Test Passed\n");
        printf("Num tails Passed2 %d\n", tailsPassed2);
        printf("Num heads Passed2 %d\n", headsPassed2);
    } else {
        printf("Fail, only %d tests passed\n", tailsPassed2);
        printf("Fail, only %d tests passed\n", headsPassed2);
    }
    printf("-------------------------------------------\n");
    int testPassed = 0;
    for (j = 0; j < 50; j++) {
        battleboatEvent.type = BB_EVENT_MESSAGE_SENT;
        AgentSetState(AGENT_STATE_WAITING_TO_SEND);
        correctMessageTest = AgentRun(battleboatEvent);
        testState = AgentGetState();
        if (correctMessageTest.type == 4 && testState == AGENT_STATE_ATTACKING) {
            testPassed++;
        }
    }
    if (testPassed == 50) {
        printf("Waiting to Send Test Passed\n");
        printf("tests Passed %d\n", testPassed);
    } else {
        printf("Fail, only %d tests passed of 50\n", testPassed);
    }
    printf("-------------------------------------------\n");
    printf("now doing attacking test\n");
    testPassed = 0;
    for (j = 0; j < 50; j++) {
        battleboatEvent.type = BB_EVENT_RES_RECEIVED;
        AgentSetState(AGENT_STATE_ATTACKING);
        correctMessageTest = AgentRun(battleboatEvent);
        testState = AgentGetState();
        if (testState == AGENT_STATE_DEFENDING) {
            testPassed++;
        }
    }
    if (testPassed == 50) {
        printf("Attacking Test Passed\n");
        printf("tests Passed %d\n", testPassed);
    } else {
        printf("Fail, only %d attack tests passed of 50\n", testPassed);
    }
    printf("-------------------------------------------\n");
        printf("now doing defending test\n");
    testPassed = 0;
    for (j = 0; j < 50; j++) {
        battleboatEvent.type = BB_EVENT_SHO_RECEIVED;
        AgentSetState(AGENT_STATE_DEFENDING);
        correctMessageTest = AgentRun(battleboatEvent);
        testState = AgentGetState();
        if (correctMessageTest.type == 5 && testState == AGENT_STATE_WAITING_TO_SEND) {
            testPassed++;
        }
    }
    if (testPassed == 50) {
        printf("Defending Test Passed\n");
        printf("tests Passed %d\n", testPassed);
    } else {
        printf("Fail, only %d defend tests passed of 50\n", testPassed);
    }
    //printf("Num heads Passed2 %d\n", headsPassed2);
    while (1);
}










