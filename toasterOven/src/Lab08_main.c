/**
 * @file    Lab08_main_template.c
 *
 * @author
 *
 * @date
 */
// **** Include libraries here ****
// Standard libraries.
#include <stdio.h>
#include <string.h>
#include <Ascii.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Course library.
#include <Adc.h>
#include <BOARD.h>
#include <Buttons.h>
#include <Leds.h>
#include <Oled.h>
#include <Timers.h>


// **** Set any macros or preprocessor directives here ****
#define DEGREE_SYMBOL "\xF8"
#define LONG_PRESS 5
#define TIM2_TICKS_PER_SECOND 10

// **** Set any local typedefs here ****
typedef enum
{
    SETUP,
    COOKING,
    RESET_PENDING,
    SELECTOR_CHANGE_PENDING
} OvenState;
typedef enum{
    BAKE,
    TOAST,
    BROIL
} CookingMode;

typedef enum{
    TIME_SELECT,
    TEMP_SELECT,
    NONE_SELECT
} SelectMode;

typedef struct
{
    OvenState state;
    CookingMode mode;
    SelectMode select;
    uint16_t cookTime;
    uint16_t setCookTime;
    uint16_t temp;
    uint16_t setTemp;
    uint16_t elapsedTime;
    uint8_t buttonEvent;
    uint8_t adcUpdateFlag;
    uint8_t smEventFlag;
  

    // add more members to this struct
} OvenData;

// **** Declare any datatypes here ****

// **** Define any module-level, global, or external variables here ****
static volatile OvenData oven;
static volatile uint16_t adcValue = 0;
static volatile uint8_t timerFlag = FALSE;
static uint8_t btn3held = 0;
static bool btn3press = false;
static bool btn4press = false;
static uint8_t btn4held = 0;
static uint8_t secondTickCounter = 0;



// **** Put any helper functions here ****
void updateOvenOLED(void);
void updateLEDProgressBar(void);
void changeMode(void);
void toggleSelect(void);
void toggleCooking(void);

uint8_t checkTimerEvents(void) {
    uint8_t events = Buttons_CheckEvents();

    if (events & BUTTON_EVENT_3DOWN) {
        btn3press = true;
        btn3held = 0;
    }
    if (events & BUTTON_EVENT_3UP) {
        if (btn3held >= LONG_PRESS) {
            oven.state = SELECTOR_CHANGE_PENDING;
        } else {
            if (oven.mode == BAKE && oven.state == SETUP) {
                oven.select = (oven.select == TIME_SELECT) ? TEMP_SELECT : TIME_SELECT;
            }
        }
        btn3press = false;
        btn3held = 0;
        oven.smEventFlag = TRUE;
        
    }

    if (events & BUTTON_EVENT_4DOWN) {
        btn4press = true;
        btn4held = 0;
    }
    if (events & BUTTON_EVENT_4UP) {
        if (btn4held >= LONG_PRESS) {
            oven.state = RESET_PENDING;
            oven.smEventFlag = TRUE;
        }
        
        btn4press = false;
    }

    return events;
}



void updateLEDProgressBar(void) {
    if (oven.setCookTime == 0) {
        LEDs_Set(0x00);
        return;
    }

    uint8_t ledsOn = (8 * oven.cookTime) / oven.setCookTime;
    uint8_t ledMask = 0xFF << (8 - ledsOn);
    LEDs_Set(ledMask);
}

void changeMode() {
    oven.mode = (oven.mode + 1) % 3;
    oven.state = SETUP;
    oven.select = TIME_SELECT;
    
}
void toggleSelect() {
    printf("[TOGGLE] Before toggle: select = %d\n", oven.select);

    if (oven.mode == BAKE && oven.state == SETUP) {
        if (oven.select == TIME_SELECT) {
            oven.select = TEMP_SELECT;
        } else {
            oven.select = TIME_SELECT;
        }
    }

    printf("[TOGGLE] After toggle: select = %d\n", oven.select);
}


void toggleCooking() {
    if (oven.state == SETUP) {
        oven.state = COOKING;
    } else {
        oven.state = SETUP;
    }
    
}


/* This function will update your OLED to reflect the state. */


void updateOvenOLED(void) {
    printf("[OLED] draw: mode=%d, select=%d, time=%u, temp=%u\n",
           oven.mode, oven.select, oven.cookTime, oven.temp);

    char buffer[256] = "";
    char line[64];

    // Oven frame (fixed part)
    const char *ovenGraphic[4] = {
        "|'''''|",   // Top heating element
        "|     |",   // Empty interior
        "|-----|",   // Tray
        "|,,,,,|"    // Bottom heating element
    };

    // Mode string
    const char *modeStr = (oven.mode == BAKE) ? "BAKE" :
                          (oven.mode == TOAST) ? "TOAST" : "BROIL";

    // Selector arrows (Bake + Setup)
    char timeIndicator = ' ';
    char tempIndicator = ' ';
    if (oven.mode == BAKE && oven.state == SETUP) {
        if (oven.select == TIME_SELECT) {
            timeIndicator = '>';
        } else if (oven.select == TEMP_SELECT) {
            tempIndicator = '>';
        }
    }

    // Clamp time display
    uint16_t timeToShow = oven.cookTime;
    if (timeToShow < 1) timeToShow = 1;
    if (timeToShow > 256) timeToShow = 256;

    // Clamp temp display
    uint16_t tempToShow = (oven.mode == BROIL) ? 500 : oven.temp;

    // Compose the 4 lines
    snprintf(line, sizeof(line), "%s MODE: %s\n", ovenGraphic[0], modeStr);
    strcat(buffer, line);

    snprintf(line, sizeof(line), "%s  %cTIME: %u:%02u\n", ovenGraphic[1], timeIndicator, timeToShow / 60, timeToShow % 60);
    strcat(buffer, line);

    if (oven.mode != TOAST) {
        snprintf(line, sizeof(line), "%s  %cTEMP: %u%s\n", ovenGraphic[2], tempIndicator, tempToShow, DEGREE_SYMBOL);
        strcat(buffer, line);
    } else {
        snprintf(line, sizeof(line), "%s\n", ovenGraphic[2]);
        strcat(buffer, line);
    }

    snprintf(line, sizeof(line), "%s\n", ovenGraphic[3]);
    strcat(buffer, line);

    // Display the buffer on the OLED
    OLED_Clear(OLED_COLOR_BLACK);
    OLED_DrawString(buffer);
    OLED_Update();
}



/* This function will execute your state machine.
 * It should ONLY run if an event flag has been set.
 */
void runOvenSM(void) {
    if (!oven.smEventFlag) return;
    oven.smEventFlag = FALSE;

    switch (oven.state) {
        case SETUP:
            oven.cookTime = oven.setCookTime;
            oven.temp = oven.setTemp;
            updateOvenOLED();
            break;

        case COOKING:
            if (oven.cookTime == 0) {
                oven.state = SETUP;
                oven.cookTime = oven.setCookTime;
                oven.temp = oven.setTemp;
                oven.elapsedTime = 0;
                LEDs_Set(0x00);
            }
            updateLEDProgressBar();
            updateOvenOLED();
            break;

        case SELECTOR_CHANGE_PENDING:
            oven.mode = (oven.mode + 1) % 3;
            oven.state = SETUP;
            oven.select = TIME_SELECT;
            oven.smEventFlag = TRUE;
            break;

        case RESET_PENDING:
            if (oven.state == RESET_PENDING) {
                if (oven.setCookTime == 0) {
                    oven.setCookTime = 1;
                }
                oven.state = COOKING;
                oven.cookTime = oven.setCookTime;
                oven.temp = (oven.mode == BROIL) ? 500 : oven.setTemp;
                oven.elapsedTime = 0;
                updateLEDProgressBar();
                updateOvenOLED();
            }
            break;
        
        

        default:
            break;
    }
}


int main(void) {
    BOARD_Init();
    Buttons_Init();
    LEDs_Init();
    Timers_Init();
    ADC_Init(ADC_CONTINUOUS_INTERRUPT);
    HAL_ADC_Start_IT(&hadc1); 
    


    oven.state = SETUP;
    oven.mode = BAKE;
    oven.select = TIME_SELECT;
    oven.cookTime = 60;
    oven.temp = 350;
    oven.setCookTime = oven.cookTime;
    oven.setTemp = oven.temp;
    oven.elapsedTime = 0;
    oven.buttonEvent = 0;
    oven.adcUpdateFlag = FALSE;
    oven.smEventFlag = FALSE;

    OLED_Init();
    OLED_Clear(OLED_COLOR_BLACK);
    updateOvenOLED();

    while (1) {
        checkTimerEvents();

        if (oven.adcUpdateFlag) {
            oven.adcUpdateFlag = FALSE;
            if (oven.state == SETUP) {
                if (oven.select == TIME_SELECT || oven.mode != BAKE) {
                    oven.setCookTime = (adcValue * 300) / 1023;
                } else if (oven.select == TEMP_SELECT && oven.mode == BAKE) {
                    oven.setTemp = 300 + ((adcValue * 250) / 1023);
                    if (oven.setTemp > 555) oven.setTemp = 555;
                }
                oven.smEventFlag = TRUE;
            }
        }
        
        

        if (timerFlag) {
            oven.smEventFlag = TRUE;
            timerFlag = FALSE;
        }

        if (oven.smEventFlag) {
            runOvenSM();
            oven.smEventFlag = FALSE;
        }
    }
}



/**
 * This is the interrupt for the TIM peripheral. It will trigger whenever a timer
 * ticks over from its period to 0.
 *
 * It should not be called, and should communicate with main code only by using
 * module-level variables.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim2) // This will be triggered every TIM2_DEFAULT_FREQ_HZ
    {
        /***************************************************************************
         * Your code goes in between this comment and the following one with
         * asterisks.
         **************************************************************************/
        if (htim == &htim2) {
            timerFlag = TRUE;
    
            // Track button hold durations
            if (btn3press) btn3held++;
            if (btn4press) btn4held++;

            //seconds
            secondTickCounter++;
            if (secondTickCounter >= TIM2_TICKS_PER_SECOND) {
                secondTickCounter = 0;
    
                if (oven.state == COOKING && oven.cookTime > 0) {
                    oven.cookTime--;
                    oven.elapsedTime++;
                    oven.smEventFlag = TRUE;

                    if (oven.cookTime == 0){
                        oven.state = SETUP;
                        oven.cookTime = oven.setCookTime;
                        oven.temp = oven.setTemp;
                        oven.elapsedTime = 0;
                        LEDs_Set(0x00);
                        oven.smEventFlag = TRUE;
                    }
                }
            }
        }
    

        /***************************************************************************
         * Your code goes in between this comment and the preceding one with
         * asterisks.
         **************************************************************************/
    }
    else if (htim == &htim3) // This will be triggered every TIM3_DEFAULT_FREQ_HZ
    {
        /***************************************************************************
         * Your code goes in between this comment and the following one with
         * asterisks.
         **************************************************************************/

         
        /***************************************************************************
         * Your code goes in between this comment and the preceding one with
         * asterisks.
         **************************************************************************/
    }
    else if (htim == &htim4) // This will be triggered every TIM4_DEFAULT_FREQ_HZ
    {
        /***************************************************************************
         * Your code goes in between this comment and the following one with
         * asterisks.
         **************************************************************************/

        /***************************************************************************
         * Your code goes in between this comment and the preceding one with
         * asterisks.
         **************************************************************************/
    }
}

/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new
 * ADC reading is available in the ADC when you are configured as
 * ADC_CONTINUOUS_INTERRUPT or ADC_SINGLE_SHOT_INTERRUPT.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc1)
    {
        /***************************************************************************
         * Your code goes in between this comment and the following one with
         * asterisks.
         **************************************************************************/
        
         uint16_t raw = HAL_ADC_GetValue(hadc);
         uint8_t newVal = (raw >> 4) & 0xFF;
 
         if (newVal != ((adcValue >> 4) & 0xFF)) {
             adcValue = raw;
 
             if (oven.state == SETUP) {
                 if (oven.mode == BAKE) {
                     if (oven.select == TIME_SELECT) {
                         oven.setCookTime = (adcValue * 300) / 1023;
                     } else if (oven.select == TEMP_SELECT) {
                         oven.setTemp = 300 + ((adcValue * 250) / 1023);
                         if (oven.setTemp > 555) oven.setTemp = 555;
                     }
                 } else {
                     oven.setCookTime = (adcValue * 300) / 1023;
                 }
 
                 oven.smEventFlag = TRUE;
             }
         }
   
        
        /***************************************************************************
         * Your code goes in between this comment and the preceding one with
         * asterisks.
         **************************************************************************/
    }
}

/**
 * This is the interrupt for the ADC1 peripheral's analog watchdog. It will trigger
 * whenever a new ADC reading is available that is outside of the high/low thresholds
 * that you set in ADC_Watchdog_Config(). This interrupt is only active when the ADC
 * is configured as ADC_CONTINUOUS_WATCHDOG or ADC_SINGLE_SHOT_WATCHDOG.
 *
 * It should not be called, and should communicate with main code only by using
 * module-level variables.
 */
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc1)
    {
        /***************************************************************************
         * Your code goes in between this comment and the following one with
         * asterisks.
         **************************************************************************/
        static uint16_t lastAdcValue = 0;
        uint16_t newValue = HAL_ADC_GetValue(hadc);
        if (abs((int)newValue - (int)lastAdcValue) > 2) {
            adcValue = newValue;
            oven.adcUpdateFlag = TRUE;
            oven.smEventFlag = TRUE;
            lastAdcValue = newValue;
        }
        
        /***************************************************************************
         * Your code goes in between this comment and the preceding one with
         * asterisks.
         **************************************************************************/
    }
}
