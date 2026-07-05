/*
 * @file:   main.c
 * @brief:  Проект для микроконтроллера MIK32 Амур, который считывает температуру
 *          с датчика LM75 и отображает её на OLED дисплее SSD1306 (128x32)
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "bus_i2c.h"
#include "bus_usart.h"
#include "ssd1306.h"
#include "lm75.h"

/* Defines -------------------------------------------------------------------*/
#define DELAY_MS  500

/* Private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef hi2c;
static USART_HandleTypeDef husart;
static LM75_HandleTypeDef hlm75;

/* Private function prototypes -----------------------------------------------*/
static void periph_Init(void);

/* Private functions ---------------------------------------------------------*/
int main(void)
{
    SystemClock_Config();
    periph_Init();

    while (1)
    {
        //LM75_PrintTemp(LM75_ReadTemp(&hlm75));         /* Вывод температуры в UART     */
        SSD1306_WriteTemp(&hi2c, LM75_ReadTemp(&hlm75)); /* Вывод температуры на SSD1306 */
        HAL_DelayMs(DELAY_MS);
    }
}

static void periph_Init(void)
{
    USART_Init(&husart);
    I2C_Init(&hi2c);
    SSD1306_Init(&hi2c);
    LM75_Init(&hlm75, &hi2c, LM75_ADDR);
}
