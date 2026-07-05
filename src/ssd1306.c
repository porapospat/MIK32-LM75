/*
 * @file:   ssd1306.c
 * @brief:  Библиотека для дисплея SSD1306 (128x32)
 */

/* Includes ------------------------------------------------------------------*/
#include "mik32_lm75/ssd1306.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief      Инициализация SSD1306
 * @param hi2c Указатель на структуру обработчика I2C интерфейса
 */
void SSD1306_Init(I2C_HandleTypeDef *hi2c)
{
    /* Задержка для включения экрана */
    for (volatile int i = 0; i < 1000; i++);

    /* Инициализация */
    HAL_SSD1306_Init(hi2c, BRIGHTNESS_FULL);

    /* Очистка */
    SSD1306_Clear(hi2c);
}

/**
 * @brief      Вывод значения температуры с учётом знака
 * @param hi2c Указатель на структуру обработчика I2C интерфейса
 * @param temp Значение температуры в градусах Цельсия
 */
void SSD1306_WriteTemp(I2C_HandleTypeDef *hi2c, float temp)
{
    int int_part = (int)temp;
    int abs_int = (int_part < 0) ? -int_part : int_part;
    int frac_part = (int)((temp - int_part) * 100);
    if (frac_part < 0)
    {
        frac_part = -frac_part;
    }

    if (temp < 0.0f)
    {
        /* Область 1: знак минус */
        SSD1306_WriteSymbol(hi2c, START_COLUMN_TH, END_COLUMN_TH, START_PAGE, END_PAGE, SYMBOL_MINUS);

        /* Область 2: целая часть (десятки) */
        HAL_SSD1306_SetBorder(hi2c, START_COLUMN_H, END_COLUMN_H, START_PAGE, END_PAGE);
        HAL_SSD1306_Write(hi2c, (abs_int / 10) % 10);

        /* Область 3: целая часть (единицы) */
        HAL_SSD1306_SetBorder(hi2c, START_COLUMN_COLON, END_COLUMN_COLON, START_PAGE, END_PAGE);
        HAL_SSD1306_Write(hi2c, abs_int % 10);

        /* Область 4: знак точка */
        SSD1306_WriteSymbol(hi2c, START_COLUMN_TM, END_COLUMN_TM, START_PAGE, END_PAGE, SYMBOL_DOT);

        /* Область 5: дробная часть (десятые) */
        HAL_SSD1306_SetBorder(hi2c, START_COLUMN_M, END_COLUMN_M, START_PAGE, END_PAGE);
        HAL_SSD1306_Write(hi2c, (frac_part / 10) % 10);
    }
    else
    {
        /* Область 1: целая часть (десятки) */
        HAL_SSD1306_SetBorder(hi2c, START_COLUMN_TH, END_COLUMN_TH, START_PAGE, END_PAGE);
        HAL_SSD1306_Write(hi2c, (abs_int / 10) % 10);

        /* Область 2: целая часть (единицы) */
        HAL_SSD1306_SetBorder(hi2c, START_COLUMN_H, END_COLUMN_H, START_PAGE, END_PAGE);
        HAL_SSD1306_Write(hi2c, abs_int % 10);

        /* Область 3: знак точка */
        SSD1306_WriteSymbol(hi2c, START_COLUMN_COLON, END_COLUMN_COLON, START_PAGE, END_PAGE, SYMBOL_DOT);

        /* Область 4: дробная часть (десятые) */
        HAL_SSD1306_SetBorder(hi2c, START_COLUMN_TM, END_COLUMN_TM, START_PAGE, END_PAGE);
        HAL_SSD1306_Write(hi2c, (frac_part / 10) % 10);

        /* Область 5: дробная часть (сотые) */
        HAL_SSD1306_SetBorder(hi2c, START_COLUMN_M, END_COLUMN_M, START_PAGE, END_PAGE);
        HAL_SSD1306_Write(hi2c, frac_part % 10);
    }
}

/**
 * @brief      Вывод сообщения об ошибке на дисплей (формат --.--)
 * @param hi2c Указатель на структуру обработчика I2C интерфейса
 */
void SSD1306_WriteError(I2C_HandleTypeDef *hi2c)
{
    HAL_SSD1306_SetBorder(hi2c, START_COLUMN_TH, END_COLUMN_TH, START_PAGE, END_PAGE);
    HAL_SSD1306_Write(hi2c, SYMBOL_MINUS);

    HAL_SSD1306_SetBorder(hi2c, START_COLUMN_H, END_COLUMN_H, START_PAGE, END_PAGE);
    HAL_SSD1306_Write(hi2c, SYMBOL_MINUS);

    SSD1306_WriteSymbol(hi2c, START_COLUMN_COLON, END_COLUMN_COLON, START_PAGE, END_PAGE, SYMBOL_DOT);

    HAL_SSD1306_SetBorder(hi2c, START_COLUMN_TM, END_COLUMN_TM, START_PAGE, END_PAGE);
    HAL_SSD1306_Write(hi2c, SYMBOL_MINUS);

    HAL_SSD1306_SetBorder(hi2c, START_COLUMN_M, END_COLUMN_M, START_PAGE, END_PAGE);
    HAL_SSD1306_Write(hi2c, SYMBOL_MINUS);
}

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
                         uint8_t start_page, uint8_t end_page, uint8_t symbol)
{
    HAL_SSD1306_SetBorder(hi2c, start_col, end_col, start_page, end_page);
    HAL_SSD1306_Write(hi2c, symbol);
}

/**
 * @brief      Очистка всего экрана в горизонтальном режиме
 * @param hi2c Указатель на структуру обработчика I2C интерфейса
 */
void SSD1306_Clear(I2C_HandleTypeDef *hi2c)
{
    HAL_SSD1306_SetBorder(hi2c, START_COLUMN, END_COLUMN, START_PAGE, END_PAGE);
    HAL_SSD1306_CLR_SCR(hi2c);
}
