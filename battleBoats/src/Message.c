/**
 * @file    Message.c
 *
 * @author  Skylar Takeuchi <sktakeuc>
 *
 * @date    June 4, 2025
 */

// Libraries
#include "Message.h"
#include <stdint.h>
#include <string.h>
#include "BattleBoats.h"

// Message_CalculateCheckSum() : XOR every character in the payload string to generate a checksum
uint8_t Message_CalculateChecksum(const char *payload)
{
    uint8_t checksum = 0;
    while (*payload != '\0')
    {
        checksum ^= *payload;
        payload++;
    }
    return checksum;
}

// Message_Encode(): Encodes a message into a full NMEA message string (payload + checksum), result 
// goes into a message_stringm returns length of the encoded string length
int Message_Encode(char *message_string, Message message_to_encode)
{
    char payload[MESSAGE_MAX_PAYLOAD_LEN];
    uint8_t checksum;

    switch (message_to_encode.type)
    {
    case MESSAGE_CHA:
        sprintf(payload, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
        break;

    case MESSAGE_ACC:
        sprintf(payload, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
        break;

    case MESSAGE_REV:
        sprintf(payload, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
        break;

    case MESSAGE_SHO:
        sprintf(payload, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
        break;

    case MESSAGE_RES:
        sprintf(payload, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1, message_to_encode.param2);
        break;

    default:
        return 0; // MESSAGE_NONE or invalid message type
    }

    checksum = Message_CalculateChecksum(payload);
    sprintf(message_string, MESSAGE_TEMPLATE, payload, checksum);
    return strlen(message_string);
}

// Message_ParseMessage(): Parses a payload and checksum string into a BB_Event struct, verifies
// that the check sum is correct before parsing, returns SUCCESS if parsed correctly and STANDARD_ERROR if not
int Message_ParseMessage(const char *payload, const char *checksum_string, BB_Event *message_event)
{
    if (strlen(checksum_string) != 2)
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    uint8_t calculated_checksum = Message_CalculateChecksum(payload);
    char calculated_str[3];
    sprintf(calculated_str, "%02X", calculated_checksum);

    if (strncmp(calculated_str, checksum_string, 2) != 0)
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    char type[4]; // Holds 3-character message type + null terminator
    if (sscanf(payload, "%3s", type) != 1)
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    // Match message type and parse accordingly
    if (strcmp(type, "CHA") == 0)
    {
        message_event->type = BB_EVENT_CHA_RECEIVED;
        sscanf(payload, "CHA,%u", &message_event->param0);
    }
    else if (strcmp(type, "ACC") == 0)
    {
        message_event->type = BB_EVENT_ACC_RECEIVED;
        sscanf(payload, "ACC,%u", &message_event->param0);
    }
    else if (strcmp(type, "REV") == 0)
    {
        message_event->type = BB_EVENT_REV_RECEIVED;
        sscanf(payload, "REV,%u", &message_event->param0);
    }
    else if (strcmp(type, "SHO") == 0)
    {
        message_event->type = BB_EVENT_SHO_RECEIVED;
        sscanf(payload, "SHO,%u,%u", &message_event->param0, &message_event->param1);
    }
    else if (strcmp(type, "RES") == 0)
    {
        message_event->type = BB_EVENT_RES_RECEIVED;
        sscanf(payload, "RES,%u,%u,%u", &message_event->param0, &message_event->param1, &message_event->param2);
    }
    else
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    return SUCCESS;
}

// Message_Decode(): Reads characters one-by-one until a complete message is formed, once \r\n is seen, the
// full string is parsed into a BB_Event, returns SUCCESS on complete or ongoing parsing, returns STANDARD_ERROR
// if the message is invalid or malfomred
int Message_Decode(unsigned char char_in, BB_Event *decoded_message_event)
{
    static char message_buffer[MESSAGE_MAX_LEN];
    static int index = 0;
    static int recording = FALSE;

    // Start of message
    if (char_in == '$')
    {
        index = 0;
        recording = TRUE;
        message_buffer[index++] = char_in;
        decoded_message_event->type = BB_EVENT_NO_EVENT;
        return SUCCESS;
    }

    // Skip characters until we see a '$'
    if (!recording)
    {
        decoded_message_event->type = BB_EVENT_NO_EVENT;
        return SUCCESS;
    }

    // Add character to buffer
    if (index < MESSAGE_MAX_LEN - 1)
    {
        message_buffer[index++] = char_in;
    }
    else
    {
        // Overflow protection
        recording = FALSE;
        decoded_message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    // Check for message end (\r\n)
    if (char_in == '\n' && index >= 2 && message_buffer[index - 2] == '\r')
    {
        message_buffer[index] = '\0'; // Null terminate the string
        recording = FALSE;

        // Split message into payload and checksum
        char *start = message_buffer + 1; // Skip the initial $
        char *asterisk = strchr(start, '*');
        if (asterisk == NULL)
        {
            decoded_message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }

        *asterisk = '\0'; // Break into payload and checksum
        char *payload = start;
        char *checksum_str = asterisk + 1;

        // Ensure checksum has at least 2 characters
        if (strlen(checksum_str) < 2)
        {
            decoded_message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }

        // Validate both checksum characters are hex digits
        for (int i = 0; i < 2; i++)
        {
            char c = checksum_str[i];
            if (!((c >= '0' && c <= '9') ||
                  (c >= 'A' && c <= 'F') ||
                  (c >= 'a' && c <= 'f')))
            {
                decoded_message_event->type = BB_EVENT_ERROR;
                return STANDARD_ERROR;
            }
        }

        // Null terminate just after the second checksum character to remove \r\n
        checksum_str[2] = '\0';

        // Final parse of message
        if (Message_ParseMessage(payload, checksum_str, decoded_message_event) == SUCCESS)
        {
            return SUCCESS;
        }
        else
        {
            decoded_message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }
    }

    decoded_message_event->type = BB_EVENT_NO_EVENT;
    return SUCCESS;
}