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

#define TIMERA_COUNT 76
#define TIMERB_COUNT 114
#define TIMERC_COUNT 190
//#define ONN 1
//#define OFF 0
// **** Declare any datatypes here ****

struct Timer {
    uint8_t event; //   True or False
    int16_t timeRemaining; //    2^16 -1
};

// **** Define global, module-level, or external variables here ****
static struct Timer TimerA = {FALSE, TIMERA_COUNT};
static struct Timer TimerB = {FALSE, TIMERB_COUNT};
static struct Timer TimerC = {FALSE, TIMERC_COUNT};

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
    //    TimerA->event = FALSE;
    //    TimerA->timeRemaining = 76;

    printf("Welcome to slin41's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);
    //    OledDrawString("Welcome\n");
    //    OledUpdate();
    int prev_state_a, prev_state_b, prev_state_c;
    int curr_state_a, curr_state_b, curr_state_c;
    while (1) {
        if (TimerA.event == FALSE) {
            OledDrawString("Not triggered yet");
            OledUpdate();
            //clearing LED
            prev_state_a = prev_state_a & 0x01;
            LEDS_SET(prev_state_a);
        } else if (TimerA.event == TRUE) {
            TimerA.event = FALSE;
            OledDrawString("\nA");
            OledUpdate();
            //toggling LED
            curr_state_a = prev_state_a ^ 0x01;
            curr_state_a = curr_state_a & 0x01;
            LEDS_SET(curr_state_a);
            prev_state_a = curr_state_a;
        }
        if (TimerB.event == FALSE) {
            OledUpdate();
            //clearing LED
            prev_state_b = prev_state_b & 0x02;
            LEDS_SET(prev_state_b);
        } else if (TimerB.event == TRUE) {
            TimerB.event = FALSE;
            OledDrawString("\nB");
            OledUpdate();
            //toggling LED
            curr_state_b = prev_state_b ^ 0x02;
            curr_state_b = curr_state_b & 0x02;
            LEDS_SET(curr_state_b);
            prev_state_b = curr_state_b;
        }
        if (TimerC.event == FALSE) {
            OledUpdate();
            //clearing LED
            prev_state_c = prev_state_c & 0x04;
            LEDS_SET(prev_state_c);

        } else if (TimerC.event == TRUE) {
            TimerC.event = FALSE;
            OledDrawString("\nC");
            OledUpdate();
            //toggling LED
            curr_state_c = prev_state_c ^ 0x04;
            curr_state_c = curr_state_c & 0x04;
            LEDS_SET(curr_state_c);
            prev_state_c = curr_state_c;
        }


        //poll timer A
        //react to timer A events
        //clear timer A event flag
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}

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

    //update timerA
    //if timerA has counted down,
    //generate timerA event
    //reset timerA
    TimerA.timeRemaining--;
    TimerB.timeRemaining--;
    TimerC.timeRemaining--;

    //reset timer if reaches zero
    if (TimerA.timeRemaining == 0) {
        TimerA.event = TRUE;
        TimerA.timeRemaining = TIMERA_COUNT;
    }
    if (TimerB.timeRemaining == 0) {
        TimerB.event = TRUE;
        TimerB.timeRemaining = TIMERB_COUNT;
    }
    if (TimerC.timeRemaining == 0) {
        TimerC.event = TRUE;
        TimerC.timeRemaining = TIMERC_COUNT;
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}