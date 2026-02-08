/**
 * @file    Field.c
 *
 * @author  Skylar Takeuchi <sktakeuc>
 *
 * @date    June 4, 2025
 */

// Libraries
#include "Field.h"
#include <stdint.h>
#include "BOARD.h"
#include <stdlib.h>

// FieldInit(): Initialize both your own and opponents field structs, your field squares
// are marked as empty and the opponents as unknown, the boat lives are also initialized.
void FieldInit(Field *ownField, Field *oppField)
{
    for (int row = 0; row < FIELD_ROWS; row++)
    {
        for (int col = 0; col < FIELD_COLS; col++)
        {
            ownField->grid[row][col] = FIELD_SQUARE_EMPTY;
            oppField->grid[row][col] = FIELD_SQUARE_UNKNOWN;
        }
    }

    // Set own boat lives to 0 (boat lives will be set when placed)
    ownField->smallBoatLives = 0;
    ownField->mediumBoatLives = 0;
    ownField->largeBoatLives = 0;
    ownField->hugeBoatLives = 0;

    // Set opponent's boat lives (assumed placed at start)
    oppField->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    oppField->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    oppField->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    oppField->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
}

// Retrieves the value at the specified field position (row, col).
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col)
{
    if (row >= FIELD_ROWS || col >= FIELD_COLS)
    {
        return FIELD_SQUARE_INVALID;
    }
    return f->grid[row][col];
}

// Sets a square to a new status and returns its old value.
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p)
{
    if (row >= FIELD_ROWS || col >= FIELD_COLS)
    {
        return FIELD_SQUARE_INVALID;
    }

    SquareStatus oldValue = f->grid[row][col];
    f->grid[row][col] = p;
    return oldValue;
}

// Attempts to place a boat of given type and direction on the player's field, return SUCCESS
// if placed successfully, and STANDARD_ERROR if place NOT successfully.
uint8_t FieldAddBoat(Field *ownField, uint8_t row, uint8_t col, BoatDirection dir, BoatType boatType)
{
    uint8_t length;
    SquareStatus boatSymbol;

    // First, get boat size and symbol
    switch (boatType)
    {
    case FIELD_BOAT_TYPE_SMALL:
        length = FIELD_BOAT_SIZE_SMALL;
        boatSymbol = FIELD_SQUARE_SMALL_BOAT;
        break;

    case FIELD_BOAT_TYPE_MEDIUM:
        length = FIELD_BOAT_SIZE_MEDIUM;
        boatSymbol = FIELD_SQUARE_MEDIUM_BOAT;
        break;

    case FIELD_BOAT_TYPE_LARGE:
        length = FIELD_BOAT_SIZE_LARGE;
        boatSymbol = FIELD_SQUARE_LARGE_BOAT;
        break;

    case FIELD_BOAT_TYPE_HUGE:
        length = FIELD_BOAT_SIZE_HUGE;
        boatSymbol = FIELD_SQUARE_HUGE_BOAT;
        break;

    default:
        return STANDARD_ERROR;
    }

    // Ensure that the boat fits and that there's no overlap
    for (int i = 0; i < length; i++)
    {
        int r = row + (dir == FIELD_DIR_SOUTH ? i : 0);
        int c = col + (dir == FIELD_DIR_EAST ? i : 0);

        if (r >= FIELD_ROWS || c >= FIELD_COLS)
        {
            return STANDARD_ERROR;
        }
        if (ownField->grid[r][c] != FIELD_SQUARE_EMPTY)
        {
            return STANDARD_ERROR;
        }
    }

    // Place the boat
    for (int i = 0; i < length; i++)
    {
        int r = row + (dir == FIELD_DIR_SOUTH ? i : 0);
        int c = col + (dir == FIELD_DIR_EAST ? i : 0);
        ownField->grid[r][c] = boatSymbol;
    }

    // Track lives for the boats
    switch (boatType)
    {
    case FIELD_BOAT_TYPE_SMALL:
        ownField->smallBoatLives = length;
        break;

    case FIELD_BOAT_TYPE_MEDIUM:
        ownField->mediumBoatLives = length;
        break;

    case FIELD_BOAT_TYPE_LARGE:
        ownField->largeBoatLives = length;
        break;

    case FIELD_BOAT_TYPE_HUGE:
        ownField->hugeBoatLives = length;
        break;
    }

    return SUCCESS;
}

// Processes incoming enemy attacks and updates boat lives and square status, writes the result
// into the "opp_guess" structure.
SquareStatus FieldRegisterEnemyAttack(Field *ownField, GuessData *opp_guess)
{
    uint8_t row = opp_guess->row;
    uint8_t col = opp_guess->col;

    // Out of bounds check
    if (row >= FIELD_ROWS || col >= FIELD_COLS)
    {
        opp_guess->result = RESULT_MISS;
        return FIELD_SQUARE_INVALID;
    }

    SquareStatus current = ownField->grid[row][col];

    switch (current)
    {
    case FIELD_SQUARE_EMPTY:
        ownField->grid[row][col] = FIELD_SQUARE_MISS;
        opp_guess->result = RESULT_MISS;
        break;

    case FIELD_SQUARE_SMALL_BOAT:
        ownField->grid[row][col] = FIELD_SQUARE_HIT;
        ownField->smallBoatLives--;
        if (ownField->smallBoatLives == 0)
        {
            opp_guess->result = RESULT_SMALL_BOAT_SUNK;
        }
        else
        {
            opp_guess->result = RESULT_HIT;
        }
        break;

    case FIELD_SQUARE_MEDIUM_BOAT:
        ownField->grid[row][col] = FIELD_SQUARE_HIT;
        ownField->mediumBoatLives--;
        if (ownField->mediumBoatLives == 0)
        {
            opp_guess->result = RESULT_MEDIUM_BOAT_SUNK;
        }
        else
        {
            opp_guess->result = RESULT_HIT;
        }
        break;

    case FIELD_SQUARE_LARGE_BOAT:
        ownField->grid[row][col] = FIELD_SQUARE_HIT;
        ownField->largeBoatLives--;
        if (ownField->largeBoatLives == 0)
        {
            opp_guess->result = RESULT_LARGE_BOAT_SUNK;
        }
        else
        {
            opp_guess->result = RESULT_HIT;
        }
        break;

    case FIELD_SQUARE_HUGE_BOAT:
        ownField->grid[row][col] = FIELD_SQUARE_HIT;
        ownField->hugeBoatLives--;
        if (ownField->hugeBoatLives == 0)
        {
            opp_guess->result = RESULT_HUGE_BOAT_SUNK;
        }
        else
        {
            opp_guess->result = RESULT_HIT;
        }
        break;

    default:
        opp_guess->result = RESULT_MISS;
        break;
    }

    return current;
}

// Updates the opponent field knowledge based on the result of your guess.
SquareStatus FieldUpdateKnowledge(Field *oppField, const GuessData *own_guess)
{
    uint8_t row = own_guess->row;
    uint8_t col = own_guess->col;

    SquareStatus oldValue = oppField->grid[row][col];

    switch (own_guess->result)
    {
    case RESULT_MISS:
        oppField->grid[row][col] = FIELD_SQUARE_EMPTY;
        break;

    case RESULT_HIT:
        oppField->grid[row][col] = FIELD_SQUARE_HIT;
        break;

    case RESULT_SMALL_BOAT_SUNK:
        oppField->grid[row][col] = FIELD_SQUARE_HIT;
        oppField->smallBoatLives = 0;
        break;

    case RESULT_MEDIUM_BOAT_SUNK:
        oppField->grid[row][col] = FIELD_SQUARE_HIT;
        oppField->mediumBoatLives = 0;
        break;

    case RESULT_LARGE_BOAT_SUNK:
        oppField->grid[row][col] = FIELD_SQUARE_HIT;
        oppField->largeBoatLives = 0;
        break;

    case RESULT_HUGE_BOAT_SUNK:
        oppField->grid[row][col] = FIELD_SQUARE_HIT;
        oppField->hugeBoatLives = 0;
        break;
    }

    return oldValue;
}

// Returns the bitwise OR of all living boats (used for game over checks).
uint8_t FieldGetBoatStates(const Field *f)
{
    uint8_t status = 0;

    if (f->smallBoatLives > 0)
    {
        status |= FIELD_BOAT_STATUS_SMALL;
    }

    if (f->mediumBoatLives > 0)
    {
        status |= FIELD_BOAT_STATUS_MEDIUM;
    }

    if (f->largeBoatLives > 0)
    {
        status |= FIELD_BOAT_STATUS_LARGE;
    }

    if (f->hugeBoatLives > 0)
    {
        status |= FIELD_BOAT_STATUS_HUGE;
    }

    return status;
}

//  AI attempts to place all boat randomly on its own field, will continue until all boats are
// placed correctly.
uint8_t FieldAIPlaceAllBoats(Field *ownField)
{
    BoatType boatTypes[] = {
        FIELD_BOAT_TYPE_SMALL,
        FIELD_BOAT_TYPE_MEDIUM,
        FIELD_BOAT_TYPE_LARGE,
        FIELD_BOAT_TYPE_HUGE};

    for (int i = 0; i < FIELD_NUM_BOATS; i++)
    {
        BoatType type = boatTypes[i];
        uint8_t placed = FALSE;

        while (!placed)
        {
            BoatDirection dir = (rand() % 2 == 0) ? FIELD_DIR_EAST : FIELD_DIR_SOUTH;

            uint8_t length;
            switch (type)
            {
            case FIELD_BOAT_TYPE_SMALL:
                length = FIELD_BOAT_SIZE_SMALL;
                break;

            case FIELD_BOAT_TYPE_MEDIUM:
                length = FIELD_BOAT_SIZE_MEDIUM;
                break;

            case FIELD_BOAT_TYPE_LARGE:
                length = FIELD_BOAT_SIZE_LARGE;
                break;

            case FIELD_BOAT_TYPE_HUGE:
                length = FIELD_BOAT_SIZE_HUGE;
                break;

            default:
                return STANDARD_ERROR;
            }

            uint8_t row = (dir == FIELD_DIR_SOUTH) ? rand() % (FIELD_ROWS - length + 1) : rand() % FIELD_ROWS;
            uint8_t col = (dir == FIELD_DIR_EAST) ? rand() % (FIELD_COLS - length + 1) : rand() % FIELD_COLS;

            if (FieldAddBoat(ownField, row, col, dir, type) == SUCCESS)
            {
                placed = TRUE;
            }
        }
    }
    return SUCCESS;
}

// AI guessing algorithim: linear scan for the first unknown square.
GuessData FieldAIDecideGuess(const Field *oppField)
{
    GuessData guess;

    for (uint8_t row = 0; row < FIELD_ROWS; row++)
    {
        for (uint8_t col = 0; col < FIELD_COLS; col++)
        {
            if (oppField->grid[row][col] == FIELD_SQUARE_UNKNOWN)
            {
                guess.row = row;
                guess.col = col;

                return guess;
            }
        }
    }

    // Fallback guess if the grid is full
    guess.row = 0xFF;
    guess.col = 0xFF;
    return guess;
}