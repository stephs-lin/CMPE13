#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include "Agent.h"
#include "Oled.h"
#include "OledDriver.h"
#include "xc.h"
#include "Message.h"
#include "BattleBoats.h"
#include "Negotiation.h"
#include "Field.h"
#include "Buttons.h"
#include "FieldOled.h"
//#include "Leds.h"
static Field own_field;
static Field opp_field;
static FieldOledTurn playerTurn;
static uint8_t turn_number;
static AgentState currentState;
static uint8_t currentEvent = BUTTON_EVENT_NONE;
static NegotiationData hashA;
static NegotiationData A;
static NegotiationData B;
static GuessData myGuess;
static GuessData oppGuess;
static Message myMessage;
//static BB_EventType myBBEvent;

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *      Initializing both fields
 *      Placing boats on your field
 *      Setting the start state of the Agent SM
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * The agent can assume that stdlib's rand() function has been seeded properly, so it is not needed
 * to call srand() inside AgentInit().
 *  */
void AgentInit(void)
{
    FieldInit(&own_field, &opp_field);
    FieldAIPlaceAllBoats(&own_field);
    OledClear(OLED_COLOR_BLACK);
    //FieldOledDrawScreen(&own_field, &opp_field, playerTurn, turn_number);
    currentState = AGENT_STATE_START;
    currentEvent = BUTTON_EVENT_NONE;
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */
Message AgentRun(BB_Event event)
{
    //            AGENT_STATE_START, //0
    //            AGENT_STATE_CHALLENGING, //1
    //            AGENT_STATE_ACCEPTING, //2
    //            AGENT_STATE_ATTACKING, //3
    //            AGENT_STATE_DEFENDING, //4
    //            AGENT_STATE_WAITING_TO_SEND, //5
    //            AGENT_STATE_END_SCREEN, //6
    switch (currentState) {
    case AGENT_STATE_START:
        LATE = 0x02;
        //printf("current button Event is, %X", currentEvent);
        if (event.type == BB_EVENT_START_BUTTON) {
            //printf("start button\n");
            //printf("here\n");
            A = rand() & 0xFFFF;
            FieldInit(&own_field, &opp_field);
            hashA = NegotiationHash(A);
            currentState = AGENT_STATE_CHALLENGING;
            //send CHA
            myMessage.type = MESSAGE_CHA;
            myMessage.param0 = hashA;
            //return myMessage;
        } else if (event.type == BB_EVENT_CHA_RECEIVED/*CHA received*/) {
            //printf("CHA received\n");
            B = rand() & 0xFFFF;
            FieldInit(&own_field, &opp_field);
            currentState = AGENT_STATE_ACCEPTING;
            //send ACC
            myMessage.type = MESSAGE_ACC;
            myMessage.param0 = B;
            //return myMessage;
        } else {
            currentState = AGENT_STATE_START;
        }
        if (event.type == BB_EVENT_RESET_BUTTON) {
            //currentState = AGENT_STATE_START;
            AgentInit();
        }
        break;
    case AGENT_STATE_CHALLENGING:
        if (event.type == BB_EVENT_RESET_BUTTON) {
            AgentInit();
        }
        if (event.type == BB_EVENT_ACC_RECEIVED) {
            myMessage.type = MESSAGE_REV;
            myMessage.param0 = A;
            if (NegotiateCoinFlip(A, B) == HEADS) {
                currentState = AGENT_STATE_WAITING_TO_SEND;
            } else {
                currentState = AGENT_STATE_DEFENDING;
            }
        }
        break;
    case AGENT_STATE_ACCEPTING:
        LATE = 0x08;
        if (event.type == BB_EVENT_REV_RECEIVED/* REV received */) {
            //printf("reV received\n");
            if (NegotiationVerify(A, hashA) == TRUE) {
                if (NegotiateCoinFlip(A, B) == HEADS) {
                    //printf("heads");
                    currentState = AGENT_STATE_DEFENDING;
                } else {
                    //printf("tails");
                    myGuess = FieldAIDecideGuess(&opp_field);
                    //printf("after AI decide\n");
                    currentState = AGENT_STATE_ATTACKING;
                    //send SHO
                    myMessage.type = MESSAGE_SHO;
                    myMessage.param0 = myGuess.row;
                    myMessage.param0 = myGuess.col;
                    //return myMessage;
                }
            } else {
                currentState = AGENT_STATE_END_SCREEN;
            }
        }
        if (event.type == BB_EVENT_RESET_BUTTON) {
            //currentState = AGENT_STATE_START;
            AgentInit();
        }
        break;
    case AGENT_STATE_ATTACKING:
        LATE = 0x10;
        if (event.type == BB_EVENT_RESET_BUTTON) {
            //currentState = AGENT_STATE_START;
            AgentInit();
        }
        if (event.type == BB_EVENT_RES_RECEIVED/* RES received */) {
            //printf("res received\n");
            FieldUpdateKnowledge(&opp_field, &myGuess);
            if (opp_field.smallBoatLives == 0 && opp_field.mediumBoatLives == 0 &&
                    opp_field.largeBoatLives == 0 && opp_field.hugeBoatLives == 0) {
                currentState = AGENT_STATE_END_SCREEN;
            } else {
                currentState = AGENT_STATE_DEFENDING;
            }
        }
        break;
    case AGENT_STATE_DEFENDING:
        LATE = 0x20;

        if (event.type == BB_EVENT_RESET_BUTTON) {
            //currentState = AGENT_STATE_START;
            AgentInit();
        }
        //printf("\nin defending state\n");
        if (event.type == BB_EVENT_SHO_RECEIVED/* SHO received */) {
            //printf("sho received\n");
            FieldUpdateKnowledge(&own_field, &myGuess);
            //send RES
            if (own_field.smallBoatLives == 0 && own_field.mediumBoatLives == 0 &&
                    own_field.largeBoatLives == 0 && own_field.hugeBoatLives == 0) {
                currentState = AGENT_STATE_END_SCREEN;
                //send RES
                myMessage.type = MESSAGE_RES;
                oppGuess.row = event.param0/*SHO row*/;
                oppGuess.col = event.param1/*SHO col*/;
                FieldRegisterEnemyAttack(&own_field, &oppGuess);
                myGuess = FieldAIDecideGuess(&opp_field);
                myMessage.param0 = myGuess.row;
                myMessage.param1 = myGuess.col;
                myMessage.param2 = FieldGetSquareStatus(&own_field, oppGuess.row, oppGuess.col);

                //return myMessage;
            } else {
                currentState = AGENT_STATE_WAITING_TO_SEND;
                //send RES
                myMessage.type = MESSAGE_RES;
                //return myMessage;
            }
        }
        break;
    case AGENT_STATE_WAITING_TO_SEND:
        LATE = 0x40;
        if (event.type == BB_EVENT_RESET_BUTTON) {
            //currentState = AGENT_STATE_START;
            AgentInit();
        }
        //printf("\nin waiting to send state\n");
        if (event.type == BB_EVENT_MESSAGE_SENT/* Message Sent(if message event occurred) */) {
            //printf("waiting to send");
            turn_number++;
            playerTurn ++;
            myGuess = FieldAIDecideGuess(&opp_field);
            //send SHO
            myMessage.type = MESSAGE_SHO;
            myMessage.param0 = myGuess.row;
            myMessage.param1 = myGuess.col;
            //return myMessage;
            currentState = AGENT_STATE_ATTACKING;
        }
        break;
    case AGENT_STATE_END_SCREEN:
        LATE = 0x80;
        if (event.type == BB_EVENT_START_BUTTON) {
            //currentState = AGENT_STATE_START;
            AgentInit();
        }
        if (currentState == AGENT_STATE_ATTACKING) {
            OledDrawString("\n\n              VICTORY");
            OledUpdate();
        } else {
            OledDrawString("\n\n              DEFEAT");
            OledUpdate();
        }
        break;
    default:
        //printf("default");
        break;
    }
    return myMessage;
}

/** * 
 * @return Returns the current state that AgentGetState is in.  
 * 
 * This function is very useful for testing AgentRun.
 */
AgentState AgentGetState(void)
{
    //printf("currentstate is %d\n", currentState);
    if (currentState == AGENT_STATE_START) {
        //printf("currentevent is %d\n", currentEvent);
    }
    //printf("currentstate is %d\n", currentState);
    return currentState;
}

/** * 
 * @param Force the agent into the state given by AgentState
 * 
 * This function is very useful for testing AgentRun.
 */
void AgentSetState(AgentState newState)
{
    currentState = newState;
}