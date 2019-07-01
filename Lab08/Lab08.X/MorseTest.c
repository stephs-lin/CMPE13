// **** Include libraries here ****
// Standard C libraries


//CMPE13 Support Library
#include "BOARD.h"
#include <stdio.h>
// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Morse.h"
#include "Leds.h"
MorseEvent morseEvent;

#define TIMERA_COUNT 25
#define TIMERB_COUNT 100
#define TIMERC_COUNT 200

struct Timer {
    uint8_t event; //   True or False
    int16_t timeRemaining; //    2^16 -1
};

static struct Timer DOT_TIMEOUT = {FALSE, TIMERA_COUNT};
static struct Timer LETTER_TIMEOUT = {FALSE, TIMERB_COUNT};
static struct Timer WORD_TIMEOUT = {FALSE, TIMERC_COUNT};

static MorseEvent event;
static MorseEvent result2;

//static uint16_t currentEvent;
//static uint8_t morseEvent;

int main() {

    BOARD_Init();
    int result;
    result = MorseInit();

    printf("Welcome to slin41's Lab8 Morse.h tester!  Compiled on %s %s\n", __DATE__, __TIME__);
    printf("Beginning automatic portion of Morse.h tester:\n");

    if (result == 1) {
        printf("\nWas MorseInit successful? SUCCESSFUL.\n");
    } else {
        printf("Was MorseInit successful? FAILED.\n");
    }

    event.type = MORSE_EVENT_DOT;
    printf("Testing MorseDecode....\n");
    MorseDecode(event);
    event.type = MORSE_EVENT_NEW_LETTER;
    result2 = MorseDecode(event);
    printf("\nExpected Result: E\n");
    printf("\nResult: %c\n", result2.parameter);


    printf("\nBeginning interactive portion of Morse.h tester:\n");

    // <editor-fold defaultstate="collapsed" desc="Configure Timer">


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // </editor-fold>

    while (1) {
        /*check for a morseEvent*/
        //        if(morseEvent.type == MORSE_EVENT_NEW_WORD){
        //            printf("\n hererer cond: %d", morseEvent.type);
        //        }
        if (morseEvent.type != MORSE_EVENT_NONE) {
            //            printf("\n entera morseevent cond: %d", morseEvent.type);
            event.type = morseEvent.type;
            /*pass event.type into MorseDecode*/
            result2 = MorseDecode(event);
            /*if morseEvent.type is a letter, then print it*/
            if (result2.type == MORSE_EVENT_CHAR_DECODED) {
                    printf("%c", result2.parameter);
                result2.type = MORSE_EVENT_NONE;
            }
            if (result2.type == MORSE_EVENT_NEW_WORD) {
                printf(" ");
                result2.type = MORSE_EVENT_NONE;
            }
            /*resent event.type and morseEvent.type to morse_event_none*/
            event.type = MORSE_EVENT_NONE;
            morseEvent.type = MORSE_EVENT_NONE;
        }
        morseEvent.type = MORSE_EVENT_NONE;
    };
}

/* Timer2 is used to check for Morse events.
 * 
 * You shouldn't need to modify this ISR!
 */
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    morseEvent = MorseCheckEvents();

}

