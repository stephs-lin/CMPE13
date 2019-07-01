
// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>


//CMPE13 Support Library
#include "BOARD.h"
#include "Leds_Lab06.h"
#include "Oled.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

#define LEFT 0
#define RIGHT 1
#define TIMERA_COUNT 3
// **** Declare any datatypes here ****

struct Timer {
    uint8_t event; //   True or False
    int16_t timeRemaining; //    2^16 -1
};

// **** Define global, module-level, or external variables here ****
static struct Timer TimerA = {FALSE, TIMERA_COUNT};


// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();
    OledInit();
    LEDS_INIT();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to slin41's lab6 part2 (bounce_switch).  Compiled on %s %s.\n", __TIME__, __DATE__);
    uint8_t switchesState = SWITCH_STATES();
    int16_t timeRemainingA, timeRemainingB, timeRemainingC, timeRemainingD;
    int direction;
    int prev_sw1 = 0;
    int curr_sw1 = 0;
    char leds_state;
    LEDS_SET(0x01);
    while (1) {
        //poll timer events and react if any occur

        if (TimerA.event == FALSE) {
            OledUpdate();
        } else if (TimerA.event == TRUE) {
            TimerA.event = FALSE;

            //check if LED is all the way on the left
            if (LEDS_GET() == 0xffffff80) {
                direction = RIGHT;
                leds_state = 0x40;
                LEDS_SET(leds_state);
            }
                //check if LED is all the way to the right
            else if (LEDS_GET() == 0x01) {
                direction = LEFT;
                leds_state = 0x02;
                LEDS_SET(leds_state);
            }
                //if LED is neither, check which direction it is currently in
            else if (LEDS_GET() != 0x01 && LEDS_GET() != 0x80) {
                if (direction == RIGHT) {
                    leds_state = LEDS_GET();
                    leds_state = leds_state >> 1; //bit shift right
                    LEDS_SET(leds_state);
                } else {
                    leds_state = LEDS_GET();
                    leds_state = leds_state << 1; //bit shift left
                    LEDS_SET(leds_state);
                }
            } else {
            }

//            OledDrawString("\n\nA");
            OledUpdate();
        }
    }
}


/***************************************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks
 **************************************************************************************************/

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void) {
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    uint8_t switchesState = SWITCH_STATES();
    int16_t timeRemainingA, timeRemainingB, timeRemainingC, timeRemainingD;
    //    printf("\ntime remaining %f", TimerA.timeRemaining);

    TimerA.timeRemaining--;
    if (TimerA.timeRemaining == 0) {
        TimerA.event = TRUE;
        TimerA.timeRemaining = TIMERA_COUNT;
        if (switchesState & SWITCH_STATE_SW1) {
            TimerA.timeRemaining += 3;
        }
        if (switchesState & SWITCH_STATE_SW2) {
            TimerA.timeRemaining += 5;
        }
        if (switchesState & SWITCH_STATE_SW3) {
            TimerA.timeRemaining += 7;
        }
        if (switchesState & SWITCH_STATE_SW4) {
            TimerA.timeRemaining += 9;
        }
    }




    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}
