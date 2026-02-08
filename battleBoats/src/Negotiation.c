/**
 * @file Negotiation.c
 * 
 * @author Morgan Stein
 * 
 * @date June 3 2025
 */

 // Libraries
 #include "Negotiation.h"

 // Define true and false
 #define TRUE 1
 #define FALSE 0

 // Implement BEEF hash
 NegotiationData NegotiationHash(NegotiationData secret){
    return (secret * secret) % PUBLIC_KEY;
 }

 // Verification of hash commitment
 int NegotiationVerify(NegotiationData secret, NegotiationData commitment){
    return NegotiationHash(secret) == commitment ? TRUE : FALSE;
 }

 // Coin Flip
 // A XOR B: if 1 --> heads | if else --> tails
 NegotiationOutcome NegotiateCoinFlip(NegotiationData A, NegotiationData B){
    NegotiationData result = A ^ B;
    uint8_t parity = 0;
    while (result){
        parity ^= (result & 1);
        result >>= 1;
    }
    return (parity == 1) ? HEADS : TAILS;
 }
 
// Returns a value of B that makes XOR parity with hash_a = 1 (HEADS).
NegotiationData NegotiateGenerateBGivenHash(NegotiationData hash_a) {
   // For extra credit: find B such that parity(hash_a XOR B) == 1
   for (NegotiationData b = 0; b < 0xFFFF; b++) {
       if (NegotiateCoinFlip(hash_a, b) == HEADS) {
           return b;
       }
   }
   return 0; // fallback
}

// Returns a value of A that makes XOR parity with B = 1 (HEADS).
NegotiationData NegotiateGenerateAGivenB(NegotiationData B) {
   for (NegotiationData a = 0; a < 0xFFFF; a++) {
       if (NegotiateCoinFlip(a, B) == HEADS) {
           return a;
       }
   }
   return 0; // fallback
}
