/**
 * @file FieldTest.c
 * 
 * @author Morgan Stein
 * 
 * @date June 5 2025
 */

 // Libraries
 #include "Field.h"
 #include "BOARD.h"
 #include <stdio.h>
 #include <string.h>
 

// Macro 

#define ASSERT_EQUAL(actual, expected, message) \
    do { \
        if ((actual) != (expected)) { \
            printf("FAIL: %s | Expected: %d, Got: %d\n", message, (expected), (actual)); \
        } else { \
            printf("PASS: %s\n", message); \
        } \
    } while (0)

// Test field initialization
void TestFieldInit(){
    Field own, opp;
    FieldInit(&own, &opp);

    for (int r = 0; r < FIELD_ROWS; r++) {
        for (int c = 0; c < FIELD_COLS; c++){
            ASSERT_EQUAL(own.grid[r][c], FIELD_SQUARE_EMPTY, "Own initialized to empty");
            ASSERT_EQUAL(opp.grid[r][c], FIELD_SQUARE_UNKNOWN, "Opponent initialized to unknown");
        }
    }

    ASSERT_EQUAL(own.smallBoatLives, 0, "Own small boat lives initialized to 0");
    ASSERT_EQUAL(opp.hugeBoatLives, FIELD_BOAT_SIZE_HUGE, "Opponent huge boat lives initialized correctly");
}

// Test add boat
void TestFieldAddBoat(){
    Field field;
    FieldInit(&field, &(Field){0});

    uint8_t result = FieldAddBoat(&field, 0,0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    ASSERT_EQUAL(result, SUCCESS, "Add small boat horizontally");

    result = FieldAddBoat(&field, 1, 0, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_MEDIUM);
    ASSERT_EQUAL(result, SUCCESS, "Add medium boat vertically");

    // this one should fail (it's out of bounds)
    result = FieldAddBoat(&field, 9, 9, FIELD_DIR_EAST, FIELD_BOAT_TYPE_LARGE);
    ASSERT_EQUAL(result, STANDARD_ERROR, "Large boat out of bounds");
}

// Test registering enemy attack 
void TestFieldRegisterEnemyAttack(){
    Field field;
    FieldInit(&field, &(Field){0});
    FieldAddBoat(&field, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);

    GuessData guess = {.row = 0, .col = 0};
    SquareStatus status = FieldRegisterEnemyAttack(&field, &guess);
    (void)status;
    ASSERT_EQUAL(status, FIELD_SQUARE_SMALL_BOAT, "Enemy hit small boat");
    ASSERT_EQUAL(guess.result, RESULT_SMALL_BOAT_SUNK, "Correct result, small boat sunk");

    guess = (GuessData){.row = 5, .col = 5};
    status = FieldRegisterEnemyAttack(&field, &guess);
    ASSERT_EQUAL(status, FIELD_SQUARE_EMPTY, "Enemy miss");
    ASSERT_EQUAL(guess.result, RESULT_MISS, "Correct result, miss");
}

// Test update field 
void TestFieldUpdate(){
    Field opp;
    FieldInit(&(Field){0}, &opp);

    GuessData guess = {.row = 2, .col = 2, .result = RESULT_HIT};
    FieldUpdateKnowledge(&opp, &guess);
    ASSERT_EQUAL(opp.grid[2][2], FIELD_SQUARE_HIT, "Updated to HIT");

    guess = (GuessData){.row = 3, .col = 3, .result = RESULT_MEDIUM_BOAT_SUNK};
    FieldUpdateKnowledge(&opp, &guess);
    ASSERT_EQUAL(opp.grid[3][3], FIELD_SQUARE_HIT, "Updated to SUNK");
    ASSERT_EQUAL(opp.mediumBoatLives, 0, "Medium boat lives updated to 0");
}

// Test get boat states
void TestFieldGetBoatStates(){
    Field field;
    FieldInit(&field, &(Field){0});
    ASSERT_EQUAL(FieldGetBoatStates(&field), 0, "No boats, 0 state");

    FieldAddBoat(&field, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    ASSERT_EQUAL(FieldGetBoatStates(&field) & FIELD_BOAT_STATUS_SMALL, FIELD_BOAT_STATUS_SMALL, "Small boat state active");
}

// Main
int main(){
    printf("Running FieldTest... \n");
    TestFieldInit();
    TestFieldAddBoat();
    TestFieldRegisterEnemyAttack();
    TestFieldUpdate();
    TestFieldGetBoatStates();
    printf("Testing complete\n");
}