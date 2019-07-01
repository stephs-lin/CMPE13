// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Oled.h"
#define trigger 5
#define HIGH_THRESHOLD 1020
#define LOW_THRESHOLD 3
#define MAX_VOLTAGE 3.3
#define MIN_VOLTAGE 0

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

struct AdcResult {
    uint8_t event; //   True or False
    float voltage; //    0 to 1023
};
// **** Define global, module-level, or external variables here ****
static struct AdcResult adc1 = {FALSE, 0};
static double previous_measure;
static double current_measure;
// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();
    OledInit();

    // Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
    AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time

    AD1CON1bits.ADON = 1; // and finally turn it on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    previous_measure = 0;
    current_measure = 0;

    printf("Welcome to slin41's lab6 part3 (bounce_adc).  Compiled on %s %s.\n", __TIME__, __DATE__);

    while (1) {
        double percentage;
        char buff[50];


        if (adc1.event == TRUE) {
            adc1.event = FALSE;
            percentage = (adc1.voltage / 3.3);
            sprintf(buff, "\nRaw: %.0f\nPercentage: %01.0f%", current_measure, (double) (percentage * 100));
            OledClear(OLED_COLOR_BLACK);
            OledUpdate();
            OledDrawString(buff);
            OledUpdate();


        }

    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new ADC reading is available
 * in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_ADC_VECTOR, ipl2auto) AdcHandler(void) {
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;

    //Find the mean
    current_measure = ((ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7) / 8);

    //If absolute value of difference between previous and current is larger than window, trigger
    if ((((previous_measure - current_measure)*1) >= trigger) | (((previous_measure - current_measure)*-1) >= trigger)) {

        //if difference is large enough, store current into previous
        adc1.event = TRUE;
        previous_measure = current_measure;

        //calculate the voltage
        adc1.voltage = ((current_measure / 1023)*3.3);

        //check if current measure meets thresholds

        if (current_measure >= HIGH_THRESHOLD) {
            adc1.voltage = MAX_VOLTAGE;
        } else if (current_measure <= LOW_THRESHOLD) {
            adc1.voltage = MIN_VOLTAGE;
        }

    } else {
    }
}