// **** Include libraries here ****
// Standard C libraries


//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
//------don't include BinaryTree.h or Morse.h!-----
//#include "Morse.h"

//Helper functions:
void OledAddToTopLine(MorseEvent event);
void OledClearTopLine(MorseEvent event);
void OledAddToBottomLine(MorseEvent event);
static uint16_t buttonEvent;
static uint16_t currentEvent;

int main() {
    BOARD_Init();

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

    printf("\nWelcome to slin41's Lab8 Morse decoder!  Compiled on %s %s\n", __DATE__, __TIME__);

    //initialization code

    while (1) {
        //poll for morse events:

        if (morseEvent.type != MORSE_EVENT_NONE) {
            event.type = morseEvent.type;
            /*pass event.type into MorseDecode*/
            result2 = MorseDecode(event);
            /*if morseEvent.type is a letter, then print it*/
            if (morseEvent.type == MORSE_EVENT_NEW_LETTER) {
                printf("\nEntered letter: %c\n", result2.parameter);
            }
            /*resent event.type and morseEvent.type to morse_event_none*/
            event.type = MORSE_EVENT_NONE;
            morseEvent.type = MORSE_EVENT_NONE;
        }

        if (morseEvent) {
            //update OLED, if appropriate
            //decode morseEvent
            //update OLED, if appropriate
            //consume morseEvent
        }
    };
}

/* Timer2 is used to check for Morse events.
 * 
 * You shouldn't need to modify this ISR!
 */
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    IFS0CLR = 1 << 8;
    morseEvent = MorseCheckEvents();
}
