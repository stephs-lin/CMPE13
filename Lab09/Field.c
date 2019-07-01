#include <stdio.h>
#include "BOARD.h"
#include "Field.h"
#include "Oled.h"
#include "OledDriver.h"
#include "xc.h"

//#define FIELD_COLS 10
//#define  FIELD_ROWS 6

/**
 * This function is optional, but recommended.   It prints a representation of both
 * fields, similar to the OLED display.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     SquareStatus.
 */
void FieldPrint_UART(Field *own_field, Field * opp_field);

/**
 * FieldInit() will initialize two passed field structs for the beginning of play.
 * Each field's grid should be filled with the appropriate SquareStatus.
 * Additionally, your opponent's field's boatLives parameters should be filled
 *  (your own field's boatLives will be set when boats are added)
 * 
 * FieldAI_PlaceAllBoats() should NOT be called in this function.
 * 
 * @param own_field     //A field representing the agents own ships
 * @param opp_field     //A field representing the opponent's ships
 */
void FieldInit(Field *own_field, Field * opp_field)
{
    int i;
    int j;
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            own_field->grid[i][j] = FIELD_SQUARE_EMPTY;
            opp_field->grid[i][j] = FIELD_SQUARE_UNKNOWN;
        }
    }
    own_field->smallBoatLives = 0;
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    own_field->mediumBoatLives = 0;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    own_field->largeBoatLives = 0;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    own_field->hugeBoatLives = 0;
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
    //A struct for tracking all of the necessary data for an agent's field.
}

/**
 * Retrieves the value at the specified field position.
 * @param f     //The Field being referenced
 * @param row   //The row-component of the location to retrieve
 * @param col   //The column-component of the location to retrieve
 * @return  FIELD_SQUARE_INVALID if row and col are not valid field locations
 *          Otherwise, return the status of the referenced square 
 */
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col)
{
    SquareStatus errorStatus = FIELD_SQUARE_INVALID;
    if (row > 5 || col > 10) {
        //f->grid[row][col] = FIELD_SQUARE_INVALID;
        return errorStatus;
        //return f->grid[row][col];
    } else if (row < 0 || col < 0) {
        return errorStatus;
    } else {
        errorStatus = f->grid[row][col];
        return errorStatus;
    }
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_SQUARE_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p)
{
    SquareStatus errorStatus = FIELD_SQUARE_INVALID;
    if (row > 5 || col > 9) {
        return errorStatus;
    } else if (row < 0 || col < 0) {
        return errorStatus;
    } else {
        SquareStatus oldStatus = f->grid[row][col];
        f->grid[row][col] = p;
        return oldStatus;
    }
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_SQUARE_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_TYPE_SMALL);
 * }
 *
 * should result in a field like:
 *      0 1 2 3 4 5 6 7 8 9
 *     ---------------------
 *  0 [ 3 3 3 . . . 3 . . . ]
 *  1 [ 4 4 4 4 . . 3 . . . ]
 *  2 [ . . . . . . 3 . . . ]
 *  3 [ . . . . . . . . . . ]
 *  4 [ . . . . . . . . . . ]
 *  5 [ . . . . . . . . . . ]
 *     
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_SQUARE_*_BOAT values from the
 * SquareStatus enum.
 * @return SUCCESS for success, STANDARD_ERROR for failure
 */
uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type)
{
    //SquareStatus errorStatus = FIELD_SQUARE_INVALID;
    int i = 0;
    if (row > 5 || col > 9) {
        return STANDARD_ERROR;
    } else if (row < 0 || col < 0) {
        return STANDARD_ERROR;
    }
    switch (boat_type) {
    case FIELD_BOAT_TYPE_SMALL:
        i = 2;
        if (dir == FIELD_DIR_SOUTH) {
            int rowMax = row + 3;
            for (i = row; i < rowMax; i++) {
                if (i >= 6 || i < 0) {
                    return STANDARD_ERROR;
                } else if (f->grid[i][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
            }
            int j = row;
            for (i = 2; i >= 0; i--) {
                f->grid[j][col] = FIELD_SQUARE_SMALL_BOAT;
                j++;
            }
            f->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
        } else if (dir == FIELD_DIR_EAST) {
            int colMax = col + 3;
            for (i = col; i < colMax; i++) {
                if (i > 9 || i < 0) {
                    return STANDARD_ERROR;
                } else if (f->grid[row][i] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
            }
            int j = col;
            for (i = 2; i >= 0; i--) {
                f->grid[row][j] = FIELD_SQUARE_SMALL_BOAT;
                j++;
            }
            f->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
        }
        break;
    case FIELD_BOAT_TYPE_MEDIUM:
        i = 3;
        if (dir == FIELD_DIR_SOUTH) {
            int rowMax = row + 4;
            for (i = row; i < rowMax; i++) {
                if (i >= 6 || i < 0) {
                    return STANDARD_ERROR;
                } else if (f->grid[i][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
            }
            int j = row;
            for (i = 3; i >= 0; i--) {
                f->grid[j][col] = FIELD_SQUARE_MEDIUM_BOAT;
                j++;
            }
            f->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
        } else if (dir == FIELD_DIR_EAST) {
            int colMax = col + 4;
            for (i = col; i < colMax; i++) {
                if (i > 9 || i < 0) {
                    return STANDARD_ERROR;
                } else if (f->grid[row][i] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
            }
            int j = col;
            for (i = 3; i >= 0; i--) {
                f->grid[row][j] = FIELD_SQUARE_MEDIUM_BOAT;
                j++;
            }
            f->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
        }
        break;
    case FIELD_BOAT_TYPE_LARGE:
        i = 4;
        if (dir == FIELD_DIR_SOUTH) {
            int rowMax = row + 5;
            for (i = row; i < rowMax; i++) {
                if (i >= 6 || i < 0) {
                    return STANDARD_ERROR;
                } else if (f->grid[i][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
            }
            int j = row;
            for (i = 4; i >= 0; i--) {
                f->grid[j][col] = FIELD_SQUARE_LARGE_BOAT;
                j++;
            }
            f->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
        } else if (dir == FIELD_DIR_EAST) {
            int colMax = col + 5;
            for (i = col; i < colMax; i++) {
                if (i > 9 || i < 0) {
                    return STANDARD_ERROR;
                } else if (f->grid[row][i] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
            }
            int j = col;
            for (i = 4; i >= 0; i--) {
                f->grid[row][j] = FIELD_SQUARE_LARGE_BOAT;
                j++;
            }
            f->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
        }
        break;
    case FIELD_BOAT_TYPE_HUGE:
        i = 5;
        if (dir == FIELD_DIR_SOUTH) {
            int rowMax = row + 6;
            for (i = row; i < rowMax; i++) {
                if (i >= 6 || i < 0) {
                    return STANDARD_ERROR;
                } else if (f->grid[i][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
            }
            int j = row;
            for (i = 5; i >= 0; i--) {
                f->grid[j][col] = FIELD_SQUARE_HUGE_BOAT;
                j++;
            }
            f->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
        } else if (dir == FIELD_DIR_EAST) {
            int colMax = col + 6;
            for (i = col; i < colMax; i++) {
                if (i > 9 || i < 0) {
                    return STANDARD_ERROR;
                } else if (f->grid[row][i] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
            }
            int j = col;
            for (i = 5; i >= 0; i--) {
                f->grid[row][j] = FIELD_SQUARE_HUGE_BOAT;
                j++;
            }
            f->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
        }
        break;
    default:
        //printf("default");
        return STANDARD_ERROR;
    }
    return SUCCESS;
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_SQUARE_HIT or FIELD_SQUARE_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The result is stored in gData->result as an
 *               output.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
SquareStatus FieldRegisterEnemyAttack(Field *f, GuessData *gData)
{
    SquareStatus errorStatus = FIELD_SQUARE_INVALID;
    //SquareStatus prevStatus = f->grid
    //update gData shot result
    if (gData->row > 5 || gData->col > 9) {
        //f->grid[row][col] = FIELD_SQUARE_INVALID;
        return errorStatus;
        //return f->grid[row][col];
    } else if (gData->row < 0 || gData->col < 0) {
        return errorStatus;
    }
    SquareStatus enemyGuess = f->grid[gData->row][gData->col];
    switch (enemyGuess) {
    case FIELD_SQUARE_EMPTY:
        gData->result = 0;
        f->grid[gData->row][gData->col] = FIELD_SQUARE_MISS;
        break;
    case FIELD_SQUARE_SMALL_BOAT:
        gData->result = 1;
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->smallBoatLives--;
        if (f->smallBoatLives <= 0) {
            gData->result = 2;
        }
        break;
    case FIELD_SQUARE_MEDIUM_BOAT:
        gData->result = 1;
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->smallBoatLives--;
        if (f->mediumBoatLives <= 0) {
            gData->result = 3;
        }
        break;
    case FIELD_SQUARE_LARGE_BOAT:
        gData->result = 1;
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->smallBoatLives--;
        if (f->largeBoatLives <= 0) {
            gData->result = 4;
        }
        break;
    case FIELD_SQUARE_HUGE_BOAT:
        gData->result = 1;
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->smallBoatLives--;
        if (f->hugeBoatLives <= 0) {
            gData->result = 5;
        }
        break;
    default:
        //printf("default");
        break;
    }
    return enemyGuess;
}
//    FIELD_SQUARE_EMPTY = 0, 	
//    FIELD_SQUARE_SMALL_BOAT, 
//    FIELD_SQUARE_MEDIUM_BOAT, 	
//    FIELD_SQUARE_LARGE_BOAT, 
//    FIELD_SQUARE_HUGE_BOAT, 	
//    FIELD_SQUARE_UNKNOWN,
//    FIELD_SQUARE_HIT,			// A field position that was attacked and contained part of a boat.
//    FIELD_SQUARE_MISS, 

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_SQUARE_HIT at that position. If it was a miss, display a FIELD_SQUARE_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * RESULT_*_BOAT_SUNK.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
SquareStatus FieldUpdateKnowledge(Field *f, const GuessData *gData)
{
    SquareStatus oldStatus = f->grid[gData->row][gData->col];
    switch (gData->result) {
    case RESULT_MISS:
        f->grid[gData->row][gData->col] = FIELD_SQUARE_EMPTY;
        break;
    case RESULT_HIT:
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        break;
    case RESULT_SMALL_BOAT_SUNK:
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->smallBoatLives = 0;
        break;
    case RESULT_MEDIUM_BOAT_SUNK:
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->mediumBoatLives = 0;
        break;
    case RESULT_LARGE_BOAT_SUNK:
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->largeBoatLives = 0;
        break;
    case RESULT_HUGE_BOAT_SUNK:
        f->grid[gData->row][gData->col] = FIELD_SQUARE_HIT;
        f->hugeBoatLives = 0;
        break;
    default:
        //printf("default");
        break;
    }
    return oldStatus;
    //    RESULT_MISS,                //0
    //    RESULT_HIT,                 //1
    //    RESULT_SMALL_BOAT_SUNK,     //2
    //    RESULT_MEDIUM_BOAT_SUNK,    //3
    //    RESULT_LARGE_BOAT_SUNK,     //4
    //    RESULT_HUGE_BOAT_SUNK,      //5
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f)
{
    uint8_t orredStates = 0;
    if (f->smallBoatLives == 0) {
        orredStates |= 0;
    } else {
        orredStates |= 1;
    }
    if (f->mediumBoatLives == 0) {
        orredStates |= 0;
    } else {
        orredStates |= 2;
    }
    if (f->largeBoatLives == 0) {
        orredStates |= 0;
    } else {
        orredStates |= 4;
    }
    if (f->hugeBoatLives == 0) {
        orredStates |= 0;
    } else {
        orredStates |= 8;
    }
    return orredStates;
}

/**
 * This function is responsible for placing all the boats on a field.
 * 
 * @param f         //agent's own field, to be modified in place.
 * @return SUCCESS if all boats could be placed, STANDARD_ERROR otherwise.
 * 
 * This function should never fail when passed a properly initialized field!
 */
uint8_t FieldAIPlaceAllBoats(Field *f)
{
    //rand between or equal to 0 and 9
    int rowGuess = rand() % 6;
    //rand between or equal to 0 and 5
    int colGuess = rand() % 10;
    //rand either 0 and 1
    int dir = rand() & 0x01;
    //    while(FieldAddBoat(&f, rowGuess, colGuess, dir, FIELD_BOAT_TYPE_HUGE) == SUCCESS){
    //        set turn ount to zero in agent init static global variable in agent
    //        
    //    }
    while (FieldAddBoat(f, rowGuess, colGuess, dir, FIELD_BOAT_TYPE_HUGE) == STANDARD_ERROR) {
        rowGuess = rand() % 6;
        colGuess = rand() % 10;
        dir = rand() & 0x01;
    }
    while (FieldAddBoat(f, rowGuess, colGuess, dir, FIELD_BOAT_TYPE_LARGE) == STANDARD_ERROR) {
        rowGuess = rand() % 6;
        colGuess = rand() % 10;
        dir = rand() & 0x01;
    }
    while (FieldAddBoat(f, rowGuess, colGuess, dir, FIELD_BOAT_TYPE_MEDIUM) == STANDARD_ERROR) {
        rowGuess = rand() % 6;
        colGuess = rand() % 10;
        dir = rand() & 0x01;
    }
    while (FieldAddBoat(f, rowGuess, colGuess, dir, FIELD_BOAT_TYPE_SMALL) == STANDARD_ERROR) {
        rowGuess = rand() % 6;
        colGuess = rand() % 10;
        dir = rand() & 0x01;
    }
    return SUCCESS;
}

/**
 * Given a field, decide the next guess.
 *
 * This function should not attempt to shoot a square which has already been guessed.
 *
 * You may wish to give this function static variables.  If so, that data should be
 * reset when FieldInit() is called.
 * 
 * @param f an opponent's field.
 * @return a GuessData struct whose row and col parameters are the coordinates of the guess.  The 
 *           result parameter is irrelevant.
 */
GuessData FieldAIDecideGuess(const Field * f)
{
    GuessData aiGuess;
    while (aiGuess.row == NULL && aiGuess.row == NULL) {
        aiGuess.col = rand() % 10;
        aiGuess.row = rand() % 6;
        if(f->grid[aiGuess.row][aiGuess.col] != FIELD_SQUARE_UNKNOWN){
            //havent already guessed?
            aiGuess.row = NULL;
            aiGuess.row = NULL;
        }
        //printf("am i stuck\n");
    }
    //switch by battle boat states 
    //checksum ceck sum by any caracter
    //randc(__TIME__)
    return aiGuess;
}

/** 
 * For Extra Credit:  Make the two "AI" functions above 
 * smart enough to beat our AI in more than 55% of games.
 */