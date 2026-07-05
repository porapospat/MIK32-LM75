/*
 * @file:   ssd1306.h
 * @brief:  Реализация библиотеки дисплея SSD1306
 */

#ifndef __SSD1306_H__
#define __SSD1306_H__

/* Includes ------------------------------------------------------------------*/
#include "mik32_hal_ssd1306.h"
#include "mik32_lm75/bus_i2c.h"

/* Exported defines ----------------------------------------------------------*/
#define SSD1306_128x32

/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief      Инициализация SSD1306
 * @param hi2c Указатель на структуру обработчика I2C 
 */
void SSD1306_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief      Очистка всего экрана в горизонтальном режиме
 * @param hi2c Указатель на структуру обработчика I2C 
 */
void SSD1306_Clear(I2C_HandleTypeDef *hi2c);

/**
 * @brief      Вывод значения температуры с учётом знака
 * @param hi2c Указатель на структуру обработчика I2C 
 * @param temp Значение температуры в градусах Цельсия
 */
void SSD1306_WriteTemp(I2C_HandleTypeDef *hi2c, float temp);

/**
 * @brief            Вывод на дисплей заданного символа
 * @param hi2c       Указатель на структуру обработчика I2C интерфейса
 * @param start_col  Номер стартовой колонки
 * @param end_col    Номер конечной колонки
 * @param start_page Номер стартовой страницы
 * @param end_page   Номер конечной страницы
 * @param symbol     Символ для вывода
 */
void SSD1306_WriteSymbol(I2C_HandleTypeDef *hi2c, uint8_t start_col, uint8_t end_col,
                         uint8_t start_page, uint8_t end_page, uint8_t symbol);

#endif /* __SSD1306_H__ */
