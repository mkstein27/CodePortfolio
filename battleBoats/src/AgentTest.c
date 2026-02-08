/**
 * @file AgentTest.c
 *
 * @author Skylar Takeuchi <sktakeuc.
 *
 * @date June 7, 2025
 */

// #includes
#include <stdio.h>
#include "Agent.h"
#include "Message.h"
#include "BOARD.h"
#include <stdlib.h>

// Testing logic
int main(void)
{
    BOARD_Init();
    printf("Starting Agent.c Test...\n");

    // AgentInit() test
    printf("\nTesting AgentInit()...\n");
    AgentInit();
    if (AgentGetState() == AGENT_STATE_START)
    {
        printf("TEST PASSED: Agent state is AGENT_STATE_START after AgentInit().\n");
    }
    else
    {
        printf("TEST FAILED: Agent state did not set to AGENT_STATE_START.\n");
    }

    // AgentSetState() test
    printf("\nTesting AgentSetState()...\n");
    AgentSetState(AGENT_STATE_ATTACKING);
    if (AgentGetState() == AGENT_STATE_ATTACKING)
    {
        printf("TEST PASSED: AgentSetState() correctly set the state.\n");
    }
    else
    {
        printf("TEST FAILED: AgentSetState() failed to correctly set the state.\n");
    }

    // Testing START + START_BUTTON
    printf("\nTesting AgentRun() from START with BB_EVENT_START_BUTTON...\n");
    AgentInit();
    BB_Event event = {.type = BB_EVENT_START_BUTTON};
    Message msg = AgentRun(event);
    if (msg.type == MESSAGE_CHA)
    {
        printf("TEST PASSED: AgentRun() returned MESSAGE_CHA as expected.\n");
    }
    else
    {
        printf("TEST FAILED: AgentRun() did not return MESSAGE_CHA.\n");
    }

    // Testing ATTACKING + RESET_BUTTON
    printf("\nTesting AgentRun() in AGENT_STATE_ATTACKING with BB_EVENT_RESET_BUTTON...\n");
    AgentSetState(AGENT_STATE_ATTACKING);
    BB_Event resetEvent = {.type = BB_EVENT_RESET_BUTTON};
    msg = AgentRun(resetEvent);
    if (AgentGetState() == AGENT_STATE_START)
    {
        printf("TEST PASSED: Agent reset properly from AGENT_STATE_ATTACKING.\n");
    }
    else
    {
        printf("TEST FAILED: Agent did not reset from AGENT_STATE_ATTACKING.\n");
    }

    // Testing for an Unexpected event in START
    printf("\nTesting AgentRun() does not change state/message unexpectedly...\n");
    AgentInit();
    BB_Event noOpEvent = {.type = BB_EVENT_ACC_RECEIVED}; // Not valid in START state
    msg = AgentRun(noOpEvent);
    if (msg.type == MESSAGE_NONE && AgentGetState() == AGENT_STATE_START)
    {
        printf("TEST PASSED: No unexpected changes in message/state were detected.\n");
    }
    else
    {
        printf("TEST FAILED: Unexpected message or state change occurred.\n");
    }

    // Testing START + CHA_RECEIVED
    printf("\nTesting AgentRun() from START with BB_EVENT_CHA_RECEIVED...\n");
    AgentInit();
    BB_Event chaEvent = {.type = BB_EVENT_CHA_RECEIVED, .param0 = 0xABCD};
    msg = AgentRun(chaEvent);
    if (msg.type == MESSAGE_ACC && AgentGetState() == AGENT_STATE_ACCEPTING)
    {
        printf("TEST PASSED: Received CHA, sent ACC, state is ACCEPTING.\n");
    }
    else
    {
        printf("TEST FAILED: Unexpected behavior on CHA_RECEIVED in START.\n");
    }

    // Testing CHALLENGING + ACC_RECEIVED
    printf("\nTesting AgentRun() from AGENT_STATE_CHALLENGING with BB_EVENT_ACC_RECEIVED...\n");
    AgentInit();
    AgentSetState(AGENT_STATE_CHALLENGING);
    BB_Event accEvent = {.type = BB_EVENT_ACC_RECEIVED, .param0 = 0x5555};
    msg = AgentRun(accEvent);
    if (msg.type == MESSAGE_REV &&
        (AgentGetState() == AGENT_STATE_ATTACKING || AgentGetState() == AGENT_STATE_DEFENDING))
    {
        printf("TEST PASSED: Received ACC, sent REV, and transitioned correctly.\n");
    }
    else
    {
        printf("TEST FAILED: Unexpected behavior on ACC_RECEIVED in CHALLENGING.\n");
    }

    // Testing ACCEPTING + REV_RECEIVED (failure path)
    printf("\nTesting AgentRun() from AGENT_STATE_ACCEPTING with invalid BB_EVENT_REV_RECEIVED...\n");
    AgentInit();
    AgentSetState(AGENT_STATE_ACCEPTING);
    BB_Event revEvent = {.type = BB_EVENT_REV_RECEIVED, .param0 = 0x0001}; // Probably wrong
    msg = AgentRun(revEvent);
    if (AgentGetState() == AGENT_STATE_END_SCREEN)
    {
        printf("TEST PASSED: Invalid REV received, transitioned to END_SCREEN.\n");
    }
    else
    {
        printf("TEST FAILED: Expected transition to END_SCREEN.\n");
    }

    // Testing DEFENDING + RESET_BUTTON
    printf("\nTesting AgentRun() in AGENT_STATE_DEFENDING with BB_EVENT_RESET_BUTTON...\n");
    AgentSetState(AGENT_STATE_DEFENDING);
    resetEvent.type = BB_EVENT_RESET_BUTTON;
    msg = AgentRun(resetEvent);
    if (AgentGetState() == AGENT_STATE_START)
    {
        printf("TEST PASSED: Agent reset properly from DEFENDING.\n");
    }
    else
    {
        printf("TEST FAILED: Agent did not reset from DEFENDING.\n");
    }

    // Testing END_SCREEN + RESET_BUTTON
    printf("\nTesting AgentRun() in AGENT_STATE_END_SCREEN with BB_EVENT_RESET_BUTTON...\n");
    AgentSetState(AGENT_STATE_END_SCREEN);
    resetEvent.type = BB_EVENT_RESET_BUTTON;
    msg = AgentRun(resetEvent);
    if (AgentGetState() == AGENT_STATE_START)
    {
        printf("TEST PASSED: Agent reset properly from END_SCREEN.\n");
    }
    else
    {
        printf("TEST FAILED: Agent did not reset from END_SCREEN.\n");
    }

    printf("Agent.c tests complete.\n");
    return 0;
}
