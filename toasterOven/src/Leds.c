/**
 * @file    Leds_template.c
 *
 * This library provides an interface for controlling the LEDs on the UCSC
 * Nucleo I/O Shield development board.
 *
 * @author
 *
 * @date
 */
// Standard libraries.
#include <stdio.h>
#include <stdlib.h>

// Course libraries.
#include <Leds.h>

/**
 * LEDs_Init() Initializes the LED bar by doing three things:
 *      1) Enables usage of the GPIO clocks for needed ports.
 *      2) Use the appropriate SFRs to set each LED pin to "output" mode.
 *      3) Use the appropriate SFRs to set each LED pin's output value to 0 (low
 *         voltage). 
 * After calling LEDs_Init(), the other functions in this file can be used to 
 * manipulate the LED bar.
 */
int8_t LEDs_Init(void) {
    // Enable GPIO clocks for ports C and B.
#ifdef STM32F4
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Init GOIO output pins for LEDs.
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif  /*  STM32F4 */
    LEDs_Set(0x0); // Reset LEDs.

    return SUCCESS;
}

/**
 * LEDs_Set() controls the output on the LED bar.  Input values are 8-bit 
 * patterns, where each bit describes the desired state of the corresponding 
 * LED. So, for example, LEDs_Set(0x80) should  leave the first LED on, and the
 * last 7 LEDs off. LEDs_Set(0x1F) should turn off the first three LEDs and turn
 * on the remaining five LEDs. 
 *
 * @param newPattern:  An 8-bit value describing the desired output on the LED
 * bar. 
 *
 * LEDs_Set should not change any LED pins to inputs.
 */
void LEDs_Set(uint8_t newPattern) {
#ifdef STM32F4
    /***************************************************************************
     * Your code goes in between this comment and the following one with
     * asterisks.
     **************************************************************************/
    for (int i = 0; i <4; i++) {
        HAL_GPIO_WritePin(GPIOB, (1<<i), (newPattern & (1<<i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    for (int i = 4; i <8; i++){
        HAL_GPIO_WritePin(GPIOC, (1<<(i+4)), (newPattern & (1<<i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }

    /***************************************************************************
     * Your code goes in between this comment and the preceding one with
     * asterisks.
     **************************************************************************/
#endif  /*  STM32F4 */
}

/**
 * LEDs_Get() returns the current state of the LED bar.  Return values are 8-bit
 * patterns, where each bit describes the current state of the corresponding
 * LED. So, for example, if the LED bar's LED's are
 *
 * [ON OFF ON OFF   OFF ON OFF ON],
 *
 * LEDs_Get() should return 0xA5.
 * 
 * @return  (uint8_t)   An 8-bit value describing the current output on the LED
 *                      bar.  
 * 
 * LEDs_Get() should not change the state of the LEDs, or any SFRs.
 */
uint8_t LEDs_Get(void) {
    uint8_t ledState = 0x00;
#ifdef STM32F4
    /***************************************************************************
     * Your code goes in between this comment and the following one with
     * asterisks.
     **************************************************************************/
    for (int i = 0; i<4; i++) {
        if(HAL_GPIO_ReadPin(GPIOB, (1<<i)) == GPIO_PIN_SET){
            ledState |= (1<<i);
        }
    }
    for (int i = 4; i<8; i++) {
        if(HAL_GPIO_ReadPin(GPIOB, (1<<(i+4))) == GPIO_PIN_SET){
            ledState |= (1<<i);
        }
    }

    /***************************************************************************
     * Your code goes in between this comment and the preceding one with
     * asterisks.
     **************************************************************************/
#endif  /*  STM32F4 */
    return ledState;
}

