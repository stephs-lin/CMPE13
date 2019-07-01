#include <stdio.h>
#include "BOARD.h"
#include "Field.h"
#include "Oled.h"
#include "OledDriver.h"
#include "xc.h"
#include "Buttons.h"
#include <time.h>

#define seed_rand(x) 

static Field myField;
static Field oppField;

int main(void)
{
    //time_t ranTime;
    BOARD_Init();
    ButtonsInit();
    OledInit();
    //Print a greeting:
    OledDrawString("This is FieldTest!\n");
    OledUpdate();
    FieldInit(&myField, &oppField);
    int status = FieldGetSquareStatus(&myField, 5 ,5);
    int setTest = FieldSetSquareStatus(&myField, 5, 5, FIELD_SQUARE_HUGE_BOAT);
    int statusAfterSet = FieldGetSquareStatus(&myField, 5 ,5);
    printf("status is: %d\nsetTest is: %d\nstatusAfterSet is: %d\n",status,setTest,statusAfterSet);
    Field addBoatField;
    Field addOppBoatField;
    FieldInit(&addBoatField, &addOppBoatField);
    FieldAddBoat(&addBoatField, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(&addBoatField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);
    FieldAddBoat(&addBoatField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE);
    FieldAddBoat(&addBoatField, 0, 6, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_LARGE);
    
    GuessData guess1;
    guess1.row = 0;
    guess1.col = 2;
    FieldRegisterEnemyAttack(&addBoatField, &guess1);
    Field aiPlaceField;
    FieldInit(&aiPlaceField, &addOppBoatField);
    //srand((unsigned)time(&ranTime));
    FieldAIPlaceAllBoats(&aiPlaceField);
    GuessData myGuess;
    myGuess = FieldAIDecideGuess(&aiPlaceField);
    SquareStatus updatedStatus;
    updatedStatus = FieldUpdateKnowledge(&aiPlaceField, &myGuess);
    printf("updated status of guess is %d\n", updatedStatus);
    printf("myGuess row is: %d \n        col is: %d\n", myGuess.row, myGuess.col);
    int boatStatus = FieldGetBoatStates(&aiPlaceField);
    printf("boatStatus is: %X\n",boatStatus);
    boatStatus = FieldGetBoatStates(&addBoatField);
    printf("boatStatus is: %X\n",boatStatus);
    
    printf("\n\nNext repeat test:\n");
    FieldInit(&addBoatField, &addOppBoatField);
    FieldAddBoat(&addBoatField, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(&addBoatField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);
    FieldAddBoat(&addBoatField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE);
    FieldAddBoat(&addBoatField, 0, 6, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_LARGE);

    guess1.row = 0;
    guess1.col = 2;
    FieldRegisterEnemyAttack(&addBoatField, &guess1);
    FieldInit(&aiPlaceField, &addOppBoatField);
    //srand((unsigned)time(&ranTime));
    FieldAIPlaceAllBoats(&aiPlaceField);
    myGuess = FieldAIDecideGuess(&aiPlaceField);
    updatedStatus = FieldUpdateKnowledge(&aiPlaceField, &myGuess);
    printf("updated status of guess is %d\n", updatedStatus);
    printf("myGuess row is: %d \n        col is: %d\n", myGuess.row, myGuess.col);
    boatStatus = FieldGetBoatStates(&aiPlaceField);
    printf("boatStatus is: %X\n",boatStatus);
    boatStatus = FieldGetBoatStates(&addBoatField);
    printf("boatStatus is: %X\n",boatStatus);
    
    printf("\n\nNext repeat test:\n");
    FieldInit(&addBoatField, &addOppBoatField);
    FieldAddBoat(&addBoatField, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(&addBoatField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);
    FieldAddBoat(&addBoatField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE);
    FieldAddBoat(&addBoatField, 0, 6, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_LARGE);

    guess1.row = 0;
    guess1.col = 2;
    FieldRegisterEnemyAttack(&addBoatField, &guess1);
    FieldInit(&aiPlaceField, &addOppBoatField);
    //srand((unsigned)time(&ranTime));
    FieldAIPlaceAllBoats(&aiPlaceField);
    myGuess = FieldAIDecideGuess(&aiPlaceField);
    updatedStatus = FieldUpdateKnowledge(&aiPlaceField, &myGuess);
    printf("updated status of guess is %d\n", updatedStatus);
    printf("myGuess row is: %d \n        col is: %d\n", myGuess.row, myGuess.col);
    boatStatus = FieldGetBoatStates(&aiPlaceField);
    printf("boatStatus is: %X\n",boatStatus);
    boatStatus = FieldGetBoatStates(&addBoatField);
    printf("boatStatus is: %X\n",boatStatus);
    while(1);
}
