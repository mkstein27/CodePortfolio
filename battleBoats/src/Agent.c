/**
 * @file Agent.c
 * 
 * @author Morgan Stein
 * 
 * @date June 4 2025
 */

 // Libraries
 #include <stdio.h>
 #include <stdlib.h>

// Project Files 
#include "Negotiation.h"
#include "Field.h"
#include "Agent.h"
#include "Message.h"

// Turn cheating on and off by changing to 0 or 1 (extra credit)
#define CHEATING 1

// Variables
static AgentState currentState;
static Field myField, oppField;
static uint8_t turnNumber;
static NegotiationData storedA, storedB, storedHashA;


// Initialize agent
void AgentInit(void){
    currentState = AGENT_STATE_START;
    turnNumber = 0;
    FieldInit(&myField, &oppField);
}
// Get state
AgentState AgentGetState(void){
    return currentState;
}
// Change state
void AgentSetState(AgentState newState){
    currentState = newState;
}
// State machine
Message AgentRun(BB_Event event){
    Message outMessage = { .type = MESSAGE_NONE };

    switch (currentState){
        case AGENT_STATE_START:
            if (event.type == BB_EVENT_START_BUTTON){
                storedA = rand() & 0xFFFF;
                storedHashA = NegotiationHash(storedA);
                outMessage.type = MESSAGE_CHA;
                outMessage.param0 = storedHashA;
                currentState = AGENT_STATE_CHALLENGING;
            }
            else if (event.type == BB_EVENT_CHA_RECEIVED){
                storedHashA = event.param0;

                #if CHEATING
                    storedB = NegotiateGenerateBGivenHash(storedHashA);
                #else 
                    storedB = rand() & 0xFFFF;
                #endif

                outMessage.type = MESSAGE_ACC;
                outMessage.param0 = storedB;
                currentState = AGENT_STATE_ACCEPTING;
            }
            break;

        case AGENT_STATE_CHALLENGING:
            if (event.type == BB_EVENT_ACC_RECEIVED){
                storedB = event.param0;

                #if CHEATING
                    storedA = NegotiateGenerateAGivenB(storedB);
                #endif

                storedHashA = NegotiationHash(storedA);
                outMessage.type = MESSAGE_REV;
                outMessage.param0 = storedA;
                NegotiationOutcome outcome = NegotiateCoinFlip(storedA, storedB);
                currentState = (outcome == HEADS) ? AGENT_STATE_ATTACKING : AGENT_STATE_DEFENDING;
            }
            break;

        case AGENT_STATE_ACCEPTING:
            if (event.type == BB_EVENT_REV_RECEIVED){
                storedA = event.param0;
                if (!NegotiationVerify(storedA, storedHashA)){
                    currentState = AGENT_STATE_END_SCREEN;
                    break;
                }
                NegotiationOutcome outcome = NegotiateCoinFlip(storedA, storedB);
                currentState = (outcome == TAILS) ? AGENT_STATE_ATTACKING : AGENT_STATE_DEFENDING;
            }
            break;

        case AGENT_STATE_ATTACKING:
            if (event.type == BB_EVENT_RESET_BUTTON){
                AgentInit();
            }
            break;

        case AGENT_STATE_DEFENDING:
            if (event.type == BB_EVENT_RESET_BUTTON){
                AgentInit();
            }
            break;

        case AGENT_STATE_END_SCREEN:
            if (event.type == BB_EVENT_RESET_BUTTON){
                AgentInit();
            }
            break;

        default:
            break;
    }
    return outMessage;
}