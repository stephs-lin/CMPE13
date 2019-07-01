/*
 * File:   Buttons.c
 * Author: Stephanie Lin
 *
 * Created on May 12, 2019, 3:55 PM
 */

// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "LEDS_Lab06.h"
#include "Oled.h"
#include "Buttons.h"

//#define B1_MASK 0x03
//#define B2_MASK 0x0C
//#define B3_MASK 0x30
//#define B4_MASK 0xc0
//#define RESET 0x00
//#define COUNTER_CHECK 5

struct Button {
    uint8_t previous;
    uint8_t current;
    uint8_t lastevent;
    int counter;
};

static struct Button B1 = {RESET, RESET, RESET, FALSE};
static struct Button B2 = {RESET, RESET, RESET, FALSE};
static struct Button B3 = {RESET, RESET, RESET, FALSE};
static struct Button B4 = {RESET, RESET, RESET, FALSE};

/**
 * This function initializes the proper pins such that the butt                                    ons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified, so that this library does not interfere with other libraries.
 */
void ButtonsInit(void) {
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

/**
 * ButtonsCheckEvents function checks the current button states and returns
 *  any events that have occured since its last
 * call.  This function should be called repeatedly in a Timer ISR, though it can
 * be called in main() during testing.
 *
 * In normal use, this function should only be used after ButtonsInit().
 * 
 * This function should assume that the buttons start in an off state with 
 * value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, 
 * BUTTONS_EVENT_NONE should be returned. 
 * 
 * @return  Each bit of the return value corresponds to one ButtonEvent flag,
 *          as described in Buttons.h.  If no events are detected, BUTTONS_EVENT_NONE is returned.
 * 
 * Note that more than one event can occur simultaneously, though this situation is rare.
 * To handle this, the output should be a bitwise OR of all applicable event flags. 
 * For example, if button 1 was released at the same time that button 2 was pressed,
 * this function should return (BUTTON_EVENT_1UP | BUTTON_EVENT_2DOWN) 
 * 
 */
uint8_t ButtonsCheckEvents(void) {
    while (1) {
        //resetting buttonEvents
        buttonEvents = 0x00;
        
        //grabbing buttonState by calling BUTTON_STATES function
        uint8_t buttonsState = BUTTON_STATES();
        
        /* START OF TESTING BUTTON 1*/
        
        //setting current button1 state equal to bitmasked buttonState
        B1.current = buttonsState & BUTTON_STATE_1;
        //if states changed, start debouncing
        if (B1.previous != B1.current) {
            //if button is toggled up
            if ((buttonsState & BUTTON_STATE_1)) {
                //check if counter is up
                if ((B1.counter >= 0) && (B1.counter < BUTTONS_DEBOUNCE_PERIOD)) {
                    B1.counter = 0;
                } else if (B1.counter >= BUTTONS_DEBOUNCE_PERIOD) {
                    B1.counter = 0;
                    
                }
                buttonEvents |= BUTTON_EVENT_1DOWN;
                //if button is toggled down
            } else if (~(buttonsState & BUTTON_STATE_1)) {
                if ((B1.counter >= 0) && (B1.counter < BUTTONS_DEBOUNCE_PERIOD)) {
                    B1.counter = 0;
                } else if (B1.counter >= BUTTONS_DEBOUNCE_PERIOD) {
                    B1.counter = 0;
                    
                }
                buttonEvents |= BUTTON_EVENT_1UP;
            }
            B1.previous = B1.current;
            //if states are the same
        } else {
            //increase the counter
            B1.counter++;
            B1.counter++;
            if ((B1.counter >= 0) && (B1.counter < BUTTONS_DEBOUNCE_PERIOD)) {
                //if counter not up, do nothing
            } else if (B1.counter >= BUTTONS_DEBOUNCE_PERIOD) {
                            //if counter is up, set BUTTON_EVENT_NONE
                B1.counter = 0;
                buttonEvents |= BUTTON_EVENT_NONE;
            }
            B1.previous = B1.current;
        }
        /* END OF TESTING BUTTON 1*/
        
        /* START OF TESTING BUTTON 2*/
        B2.current = buttonsState & BUTTON_STATE_2;
        if (B2.previous != B2.current) {
            if ((buttonsState & BUTTON_STATE_2)) {
                buttonEvents |= BUTTON_EVENT_2DOWN;
            } else if (~(buttonsState & BUTTON_STATE_2)) {
                buttonEvents |= BUTTON_EVENT_2UP;
            }
            B2.previous = B2.current;
        }
                /* END OF TESTING BUTTON 2*/

        /* START OF TESTING BUTTON 3*/
        B3.current = buttonsState & BUTTON_STATE_3;
        if (B3.previous != B3.current) {
            if ((buttonsState & BUTTON_STATE_3)) {
                buttonEvents |= BUTTON_EVENT_3DOWN;
            } else if (~(buttonsState & BUTTON_STATE_3)) {
                buttonEvents |= BUTTON_EVENT_3UP;
            }
            B3.previous = B3.current;
        }
        /* END OF TESTING BUTTON 3*/

        /* START OF TESTING BUTTON 4*/
        B4.current = buttonsState & BUTTON_STATE_4;
        if (B4.previous != B4.current) {
            if ((buttonsState & BUTTON_STATE_4)) {
                buttonEvents |= BUTTON_EVENT_4DOWN;
            } else if (~(buttonsState & BUTTON_STATE_4)) {
                buttonEvents |= BUTTON_EVENT_4UP;
            }
            B4.previous = B4.current;
        }
        //        /* END OF TESTING BUTTON 4*/

        return buttonEvents;
    }
}
