//Stephanie Lin  slin41@ucsc.edu

// Standard libraries
#include <stdio.h>
#include <math.h>
#include <xc.h>

//User libraries:
#include "BOARD.h"


// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double Divide(double operand1, double operand2);
double AbsoluteValue(double operand);
double CelsiusToFahrenheit(double operand);
double FahrenheitToCelsius(double operand);
double Average(double operand1, double operand2);
double Tangent(double operand);
double Round(double operand);
//add more prototypes here
char operator[1];
double operand1;
double operand2;
double result;
int rounded;

void CalculatorRun(void) {
    while (1) {

        // **** Greets the user ****
        printf("\n\nWelcome to Stephanie's calculator program! Compiled at %s %s\n", __DATE__, __TIME__);
        do {
            printf("\nPlease enter an operator: +, -, *, /, m, a, f, c, t, or r\n");

            scanf("%s", operator);
            printf("\nYou entered: %s\n", operator);

            // **** If operator invalid, prints error message ****
            if ((operator[0] != 43) && (operator[0] != 45) && (operator[0] != 42) && (operator[0] != 47) && (operator[0] != 109) && (operator[0] != 97) && (operator[0] != 102) && (operator[0] != 99) && (operator[0] != 116) && (operator[0] != 114)) {
                operator[0] = 0;
                printf("\nInvalid Operator\n");
            } else {
            }
        } while (operator[0] == 0);
        // **** Checks if binary operator ****
        if ((operator[0] == 43 && operator[1] == 0) || (operator[0] == 45 && operator[1] == 0) || (operator[0] == 42 && operator[1] == 0) || (operator[0] == 47 && operator[1] == 0) || (operator[0] == 109 && operator[1] == 0)) {

            // **** Asks for two operands ****
            printf("\nEnter the first operand: ");
            scanf(" %lf", &operand1);
            printf("You entered %f", operand1);
            printf("\nEnter the second operand: ");
            scanf(" %lf", &operand2);
            printf("\nYou entered %f", operand2);

            // **** Checks which operator chosen and runs appropriate function ****
            if (operator[0] == 43 && operator[1] == 0) {
                result = Add(operand1, operand2);
            } else if (operator[0] == 45) {
                result = Subtract(operand1, operand2);
            } else if (operator[0] == 42) {
                result = Multiply(operand1, operand2);
            } else if (operator[0] == 47) {
                result = Divide(operand1, operand2);
            } else if (operator[0] == 109) {
                result = Average(operand1, operand2);
            } else {
            }

            // **** Prints results ****
            if (result == HUGE) {
                printf("\nDivide by zero error!\n");
            } else if (operator[0] == 109) {
                printf("\nResult of Average(%f, %f): %f\n", operand1, operand2, result);
            } else {
                printf("\nResult of (%f %s %f): %f\n", operand1, operator, operand2, result);
            }

            // **** Checks if unary operator ****
        } else if ((operator[0] == 97 && operator[1] == 0) || (operator[0] == 102 && operator[1] == 0) || (operator[0] == 99 && operator[1] == 0) || (operator[0] == 116 && operator[1] == 0) || (operator[0] == 114 && operator[1] == 0)) {
            printf("\nEnter the first operand: ");
            scanf(" %lf", &operand1);
            printf("You entered %f", operand1);

            // **** Checks which operator chosen and runs appropriate function****
            // **** Prints results ****
            if (operator[0] == 97) {
                result = AbsoluteValue(operand1);
                printf("\nResult of AbsoluteValue(%f): %f\n", operand1, result);
            } else if (operator[0] == 102) {
                result = CelsiusToFahrenheit(operand1);
                printf("\nResult of CelsiusToFahrenheit(%f): %f\n", operand1, result);
            } else if (operator[0] == 99) {
                result = FahrenheitToCelsius(operand1);
                printf("\nResult of FahrenheitToCelsius(%f): %f\n", operand1, result);
            } else if (operator[0] == 116) {
                result = Tangent(operand1);
                printf("\nResult of Tangent(%f): %f\n", operand1, result);
            } else if (operator[0] == 114) {
                result = Round(operand1);
                printf("\nResult of Round(%f): %f\n", operand1, result);
            } else {
            }

        } else {
        }
    }
};

/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2) {
    return operand1 + operand2;
}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2) {
    return operand1 - operand2;
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2) {
    return operand1 * operand2;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2) {
    if (operand2 != 48) {
        return operand1 / operand2;
    } else {
        return HUGE;
    }
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double operand) {
    if (operand < 0) {
        return operand * -1;
    } else {
        return operand;
    }
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand) {
    return (operand * (9.0 / 5.0)) + 32;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand) {
    return (operand - 32)*(5.0 / 9.0);
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2) {
    return (operand1 + operand2) / 2;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand) {
    return tan(operand);
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand) {
    // **** Checks if operand is positive ****
    if (operand > 0) {
        rounded = operand + 0.5;
        return rounded;
        // **** Checks if operand is negative ****
    } else if (operand < 0) {
        rounded = operand - 0.5;
        return rounded;
    } else {
    }
}


