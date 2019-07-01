// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Oled.h"
#include "Leds.h"
#include "Buttons.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>



// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define TIMERA_COUNT 10
#define TIMERB_COUNT 100
#define SELECTOR_ASCII 62
#define SPACE_ASCII 32
#define BROIL_TEMP 500
#define LONG_PRESS 5
#define trigger 5
#define HIGH_THRESHOLD 1020
#define LOW_THRESHOLD 3
#define MAX_VOLTAGE 100
#define MIN_VOLTAGE 0
#define DEFAULT_TEMP 350

// **** Set any local typedefs here ****

typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING
} OvenState;
//COOKING MODE STATE

typedef enum {
    BAKE_OFF, TOAST_OFF, BROIL_OFF, BAKE_ON, TOAST_ON, BROIL_ON,
} OvenCookMode;

//INPUT SELECTOR

typedef enum {
    SELECT_TIME, SELECT_TEMP
} InputSelector;

typedef struct {
    OvenState state;
    uint16_t temperature; //in degree F
    OvenCookMode cookmode;
    uint16_t button_press_time; //long press ( >1 )or short press ( <1 )
    uint16_t cooking_start_time;
    int cooking_remainingtime;
    InputSelector select;
    //add more members to this struct
} OvenData;

struct Timer {
    uint8_t event; //   True or False
    int16_t timeRemaining; //    2^16 -1
};

struct AdcResult {
    uint8_t event; //   True or False
    float voltage; //    0 to 1023
};
// **** Define global, module-level, or external variables here ****
static struct AdcResult adc1 = {FALSE, 0};
static struct Timer TimerA = {FALSE, TIMERA_COUNT};
static struct Timer TimerB = {FALSE, TIMERB_COUNT};

// **** Declare any datatypes here ****
//STRUCT
OvenData ovenData = {SETUP, DEFAULT_TEMP, BAKE_OFF, 0, 01, 0, SELECT_TIME};
// **** Define any module-level, global, or external variables here ****
char displayText[4 * 21];
static const char topOvenHeaterOn[] = "\x1\x1\x1\x1\x1";
static const char topOvenHeaterOff[] = "\x2\x2\x2\x2\x2";
static const char bottomOvenHeaterOn[] = "\x3\x3\x3\x3\x3";
static const char bottomOvenHeaterOff[] = "\x4\x4\x4\x4\x4";

//For selector. Use ASCII >: 62 space: 32
//static const char bakeDisplayText[] = "|%5s| Mode: Bake\n"; //Top pattern
//static const char bakeDisplayText[] = "|    | %cTime: %2d:%02d\n"; //%c-> slector %d: min %d: sec
//static const char bakeDisplayText[] = "|    | %cTemp: %3d\xF8" "F\n";//%d->temp value
//static const char bakeDisplayText[] = "|%5s| ";//Bottom Pattern

static const char bakeDisplayText[] = "|%5s| Mode: Bake\n|     |%cTime: "
        "%2d:%02d\n|-----|%cTemp: %3d\xF8" "F\n|%5s| ";
static const char toastDisplayText[] = "|%5s| Mode: Toast\n|     |%cTime: "
        "%2d:%02d\n|-----| \n|%5s| ";
static const char broilDisplayText[] = "|%5s| Mode: Broil\n|     |%cTime: "
        "%2d:%02d\n|-----|%cTemp: %3d\xF8" "F\n|%5s| ";

static char BTN_4DOWN;
static char BTN_3DOWN;
static char BTN_4UP;
static char BTN_3UP;
static char ADC_CHANGED;
static float ADC_PREVIOUS = 0;
static uint16_t ADC_CURRENT;
static uint16_t ADC_READ;
static uint16_t ADC_RESULT;
static uint16_t buttonEvent;
static uint16_t currentEvent;
static uint16_t free_running_timer;
static uint16_t TIMER_TICK;
static uint16_t BUTTON_START;
static uint16_t BUTTON_END;
static uint16_t TIME;
static uint16_t SELECT;
static uint16_t FLAG5;
static uint16_t MIN;
static uint16_t SEC;
//static uint16_t SELECT;
// **** Put any helper functions here ****

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData) {
    OledClear(OLED_COLOR_BLACK);
    //    ovenData.cooking_remainingtime = ovenData.cooking_start_time;

    /*  CALCULATING SEC AND MIN  */
    SEC = ovenData.cooking_remainingtime % 60;
    MIN = ovenData.cooking_remainingtime / 60;

    /*  DISPLAY BAKE ON/OFF  */
    if (ovenData.cookmode == BAKE_ON) {
        printf("\nbake on\n");
        if (ovenData.select == 0) {
            TIME = 62;
            SELECT = 32;
        } else {
            TIME = 32;
            SELECT = 62;
        }
        sprintf(displayText, bakeDisplayText, topOvenHeaterOn, SELECT, MIN, SEC, TIME, 350, bottomOvenHeaterOn);
    }
    if (ovenData.cookmode == BAKE_OFF) {
        printf("\nbake off\n");
        if (ovenData.select == 0) {
            TIME = 62;
            SELECT = 32;
        } else {
            TIME = 32;
            SELECT = 62;
        }
        sprintf(displayText, bakeDisplayText, topOvenHeaterOff, SELECT, MIN, SEC, TIME, ovenData.temperature, bottomOvenHeaterOff);
    }
    
    /*  DISPLAY TOAST ON/OFF  */
    if (ovenData.cookmode == TOAST_ON) {
        printf("\ntoast on\n");
        sprintf(displayText, toastDisplayText, topOvenHeaterOff, SPACE_ASCII, MIN, SEC, bottomOvenHeaterOn);
    }
    if (ovenData.cookmode == TOAST_OFF) {
        printf("\ntoast off\n");
        sprintf(displayText, toastDisplayText, topOvenHeaterOff, SPACE_ASCII, MIN, SEC, bottomOvenHeaterOff);
    }
    
    /*  DISPLAY BROIL ON/OFF  */
    if (ovenData.cookmode == BROIL_ON) {
        printf("\nbroil on\n");
        sprintf(displayText, broilDisplayText, topOvenHeaterOn, SPACE_ASCII, MIN, SEC, SPACE_ASCII, BROIL_TEMP, bottomOvenHeaterOff);
    }
    if (ovenData.cookmode == BROIL_OFF) {
        printf("\nbroil off\n");
        sprintf(displayText, broilDisplayText, topOvenHeaterOff, SPACE_ASCII, MIN, SEC, SPACE_ASCII, BROIL_TEMP, bottomOvenHeaterOff);
    }

    OledDrawString(displayText);
    OledUpdate();
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void) {
    switch (ovenData.state) {
        case SETUP:
            if (currentEvent) {
                /*  CHECKING BUTTON_EVENT_3DOWN  */
                if ((currentEvent & BUTTON_EVENT_3DOWN)) {
                    currentEvent = BUTTON_EVENT_NONE;
                    BUTTON_START = free_running_timer;
                    ovenData.state = SELECTOR_CHANGE_PENDING;
                }
                /*  CHECKING BUTTON_EVENT_4DOWN  */
                if (currentEvent & BUTTON_EVENT_4DOWN) {
                    BUTTON_START = free_running_timer;
                    /*  TRANSITION TO NEXT MODE  */
                    if (ovenData.cookmode == BAKE_OFF) {
                        ovenData.cookmode = BAKE_ON;
                        ovenData.cooking_remainingtime = ovenData.cooking_start_time;
                        ovenData.state = COOKING;
                    } else if (ovenData.cookmode == TOAST_OFF) {
                        ovenData.cookmode = TOAST_ON;
                        ovenData.cooking_remainingtime = ovenData.cooking_start_time;
                        ovenData.state = COOKING;
                    } else {
                        ovenData.cookmode = BROIL_ON;
                        ovenData.cooking_remainingtime = ovenData.cooking_start_time;
                        ovenData.state = COOKING;
                    }
                    TIMER_TICK = 0;
                }
            }
            /*  CHECKING IF ADC CHANGED  */
            if (ADC_CHANGED == 1) {
                ADC_CHANGED = 0;
                /*  ADC_CHANGING START TIME  */
                if (ovenData.select == 1) {
                    ovenData.cooking_start_time = ((ADC_CURRENT >> 2) + 1);
                    ovenData.cooking_remainingtime = ovenData.cooking_start_time;
                    updateOvenOLED(ovenData);
                    ovenData.state = SETUP;
                }
                    /*  ADC_CHANGING TEMPERATURE  */
                else {
                    ovenData.temperature = (ADC_CURRENT >> 2) + 300;
                    updateOvenOLED(ovenData);
                    ovenData.state = SETUP;
                }
            }
            currentEvent = BUTTON_EVENT_NONE;
            break;

        case SELECTOR_CHANGE_PENDING:
            /*  CHECKING BUTTON_EVENT_3UP  */
            if ((currentEvent & BUTTON_EVENT_3UP)) {
                BUTTON_END = free_running_timer;
                if ((BUTTON_END - BUTTON_START) < LONG_PRESS) {
                    /*  TRANSITIONS TO NEXT MODE  */
                    if (ovenData.cookmode == BROIL_OFF) {
                        ovenData.cookmode = BAKE_OFF;
                        updateOvenOLED(ovenData);
                        ovenData.state = SETUP;
                    } else if (ovenData.cookmode == TOAST_OFF) {
                        ovenData.cookmode = BROIL_OFF;
                        updateOvenOLED(ovenData);
                        ovenData.state = SETUP;
                    } else if (ovenData.cookmode == BAKE_OFF) {
                        ovenData.cookmode = TOAST_OFF;
                        updateOvenOLED(ovenData);
                        ovenData.state = SETUP;
                    }
                } else {
                    if (ovenData.select == SELECT_TEMP) {
                        ovenData.select = SELECT_TIME;
                        updateOvenOLED(ovenData);
                        ovenData.state = SETUP;
                    } else if (ovenData.select == SELECT_TIME) {
                        ovenData.select = SELECT_TEMP;
                        updateOvenOLED(ovenData);
                        ovenData.state = SETUP;
                    }
                }
            }
            break;
        case COOKING:
            ovenData.cooking_remainingtime = ovenData.cooking_remainingtime - TIMER_TICK;
            /*  CHECKING BUTTON_EVENT_4DOWN  */
            if (currentEvent & BUTTON_EVENT_4DOWN) {
                ovenData.state = RESET_PENDING;
                updateOvenOLED(ovenData);
                BUTTON_START = free_running_timer;
            } else {
                /*  CHECKING IF COUNT DOWN IS OVER  */
                if (ovenData.cooking_remainingtime != 0) {
                    if (TIMER_TICK == 1) {
                        ovenData.cooking_remainingtime--;
                        updateOvenOLED(ovenData);
                        ovenData.state = COOKING;
                        TIMER_TICK = FALSE;
                    }
                    /*  TRANSITION FROM ON TO OFF  */
                } else {
                    if (ovenData.cookmode == BAKE_ON) {
                        ovenData.cookmode = BAKE_OFF;
                    } else if (ovenData.cookmode == TOAST_ON) {
                        ovenData.cookmode = TOAST_OFF;
                    } else if (ovenData.cookmode == BROIL_ON) {
                        ovenData.cookmode = BROIL_OFF;
                    }
                    ovenData.cooking_remainingtime = ovenData.cooking_start_time;
                    updateOvenOLED(ovenData);
                    ovenData.state = SETUP;
                }
                break;
            }
        case RESET_PENDING:
            /*  CHECKING BUTTON_EVENT_4UP  */
            if (currentEvent & BUTTON_EVENT_4UP) {
                ovenData.state = COOKING;
                BUTTON_END = free_running_timer;
                ovenData.cooking_remainingtime--;
            }
            /*  CHECK FOR LONG_PRESS  */
            if ((BUTTON_END - BUTTON_START) >= LONG_PRESS) {
                ovenData.cooking_remainingtime = 0;
                if (ovenData.cookmode == BAKE_ON) {
                    ovenData.cookmode = BAKE_OFF;
                } else if (ovenData.cookmode == TOAST_ON) {
                    ovenData.cookmode = TOAST_OFF;
                } else {
                    ovenData.cookmode = BROIL_OFF;
                }
                updateOvenOLED(ovenData);
                ovenData.state = SETUP;
            } else {
                updateOvenOLED(ovenData);
                LEDS_SET(newPattern);
                ovenData.state = RESET_PENDING;
            }
            break;
        default:
            break;
    }
}

int main() {
    BOARD_Init();
    OledInit();
    LEDS_INIT();
    ButtonsInit();
    AdcInit();

    //    LEDS_SET(0xFF);

    //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">

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

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>

    printf("Welcome to slin41's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here

    while (1) {
        /* BUTTON EVENT CHECKER  */
        if (buttonEvent != FALSE) {
            currentEvent = buttonEvent;
            runOvenSM();
            updateOvenOLED(ovenData);
            buttonEvent = FALSE;
        }
        /* ADC EVENT CHECKER */
        if (adc1.event != FALSE || buttonEvent != FALSE) {
            ADC_CHANGED = 1;
            runOvenSM();
            adc1.event = FALSE;
        } else {
            ADC_CHANGED = 0;
        }
    };
}

/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

    TimerA.timeRemaining--;
    free_running_timer++;
    if (TimerA.timeRemaining == 0) {
        TimerA.event = TRUE;
        TimerA.timeRemaining = TIMERA_COUNT;
        TIMER_TICK++;
        FLAG5 = TRUE;
    }
    //add event-checking code here
}

/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    TimerB.timeRemaining--;

    if (TimerB.timeRemaining == 0) {
        TimerB.event = TRUE;
        TimerB.timeRemaining = TIMERB_COUNT;
    }
    if (AdcChanged() == 1) {
        ADC_CURRENT = AdcRead();
        adc1.event = TRUE;
    }
    buttonEvent = ButtonsCheckEvents();
}