/**
 * @file NegotiationTest.c
 *
 * @author Skylar Takeuchi <sktakeuc.
 *
 * @date June 7, 2025
 */

 // #includes
 #include <stdio.h>
 #include <stdint.h>
 #include "Negotiation.h"
 #include "BOARD.h"

 // Tests
 int main(void) {
    BOARD_Init();
    printf("Starting Negotiation.c Tests...\n");

    // NegotiationHash() test
    printf("\nTesting NegotiationHash()...\n");
    NegotiationData input = 1234;
    NegotiationData expected = (input * input) % PUBLIC_KEY;
    NegotiationData result = NegotiationHash(input);
    if (result == expected) {
        printf("TEST PASSED: Hash(%u) returned %u as expected.\n", input, result);
    } else {
        printf("TEST FAILED: Hash(%u) returned %u, expected %u.\n", input, result, expected);
    }

    // NegotiationVerify() test (SHOULD PASS)
    printf("\nTesting NegotiationVerify() - PASS case...\n");
    if (NegotiationVerify(input, expected)) {
        printf("TEST PASSED: NegotiationVerify(%u, %u) = TRUE as expected.\n", input, expected);
    } else {
        printf("TEST FAILED: NegotiationVerify(%u, %u) = FALSE unexpectedly.\n", input, expected);
    }

    // NegotiationVerify() test (SHOULD FAIL)
    printf("\nTesting NegotiationVerify() - FAIL case...\n");
    if (!NegotiationVerify(input + 1, expected)) {
        printf("TEST PASSED: NegotiationVerify(%u, %u) = FALSE as expected.\n", input + 1, expected);
    } else {
        printf("TEST FAILED: NegotiationVerify(%u, %u) = TRUE unexpectedly.\n", input + 1, expected);
    }

    // NegotiateCoinFlip() HEADS
    printf("\nTesting NegotiateCoinFlip() -> HEADS...\n");
    NegotiationData A = 0b1010;
    NegotiationData B = 0b0100;
    NegotiationOutcome outcome = NegotiateCoinFlip(A, B);
    if (outcome == HEADS) {
        printf("TEST PASSED: NegotiateCoinFlip() returned HEADS as expected.\n");
    } else {
        printf("TEST FAILED: NegotiateCoinFlip() returned TAILS unexpectedly.\n");
    }

    // NegotiateCoinFlip() TAILS
    printf("\nTesting NegotiateCoinFlip -> TAILS...\n");
    A = 0b1111; B = 0b0000;
    outcome = NegotiateCoinFlip(A, B);
    if (outcome == TAILS) {
        printf("TEST PASSED: NegotiateCoinFlip() returned TAILS as expected.\n");
    } else {
        printf("TEST FAILED: NegotiateCoinFlip() returned HEADS unexpectedly.\n");
    }

    printf("\nAll Negotiation.c tests complete.\n");
    return 0;
 }