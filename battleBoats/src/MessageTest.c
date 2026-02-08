/**
 * @file MessageTest.c
 * 
 * @author Morgan Stein
 * 
 * @date June 5 2025
 */

 // Libraries 
 #include <stdio.h>
 #include <string.h>
 #include "Message.h"
 #include "BattleBoats.h"

 // Encoding Test
 void TestEncodeCHA(){
    Message msg = { .type = MESSAGE_CHA, .param0 = 12345 };
    char buffer[MESSAGE_MAX_LEN] = {0};

    int len = Message_Encode(buffer, msg);
    printf("ENCODE: %s [%s]\n", buffer, (len > 0) ? "PASS" : "FAIL");
 }

 //Decoding test
 void TestDecodeCHA(){
    const char *teststr = "$CHA,12345*";
    char fullMessage[MESSAGE_MAX_LEN];
    strcpy(fullMessage, teststr);
    uint8_t checksum = Message_CalculateChecksum("CHA,12345");
    char checkstumStr[3];
    sprintf(checkstumStr, "%02X", checksum);
    strcat(fullMessage, checkstumStr);
    strcat(fullMessage, "\r\n");

    BB_Event event = {0};
    for (int i = 0; fullMessage[i] != '\0'; i++){
        Message_Decode(fullMessage[i], &event);
    }

    if(event.type == BB_EVENT_CHA_RECEIVED && event.param0 == 12345){
        printf("DECODE PASS\n");
    }
    else{
        printf("DECODE FAIL (type=%d, param0=%u)\n", event.type, event.param0);
    }
 }

 // Test for bad checksum
 void TestBadChecksum(){
    const char *badStr = "$CHA,12345*00\r\n";
    BB_Event event = {0};

    for (int i = 0; badStr[i] != '\0'; i++){
        Message_Decode(badStr[i], &event);
    }
    printf("BAD CHECKSUM: %s\n", (event. type == BB_EVENT_ERROR) ? "PASS" : "FAIL");
 }

 // Overflow test
 void TestOverflow(){
    BB_Event event = {0};
    Message_Decode('$', &event);
    for (int i = 0; i < MESSAGE_MAX_LEN + 10; i++){
        Message_Decode('A', &event);
    }
    printf("OVERFLOW: %s\n", (event.type == BB_EVENT_ERROR) ? "PASS" : "FAIL");
 }

 // Main
 int main(void){
    printf("Running MessageTest... \n");
    TestEncodeCHA();
    TestDecodeCHA();
    TestBadChecksum();
    TestOverflow();
    printf("Testing complete\n");

    return 0;
 }