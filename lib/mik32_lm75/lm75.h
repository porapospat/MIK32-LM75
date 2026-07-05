/*
 * @file:   lm75.h
 * @brief:  Библиотека для работы с датчиком температуры LM75
 */

#ifndef __LM75_H__
#define __LM75_H__

/* Includes ------------------------------------------------------------------*/
#include "mik32_hal_i2c.h"

/* Exported defines ----------------------------------------------------------*/
#define LM75_ADDR          0x4A
#define LM75_DATA_SIZE     2

/* Регистры LM75 */
#define LM75_REG_TEMP      0x00  /* Регистр температуры */
#define LM75_REG_CONFIG    0x01  /* Регистр конфигурации */
#define LM75_REG_THYST     0x02  /* Регистр гистерезиса */
#define LM75_REG_TOS       0x03  /* Регистр порога отключения */

#define CONFIG_REG_DEFAULT 0x00

/* Биты регистра конфигурации */
#define CONFIG_BIT_FAULT0                0x03
#define CONFIG_BIT_FAULT1                0x04
#define CONFIG_BIT_OS_POLARITY           0x02
#define CONFIG_BIT_COMPARATOR_INTERRUPT  0x01
#define CONFIG_BIT_SHUTDOWN              0x00

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Статус датчика LM75
 */
typedef enum
{
    LM75_STATE_OK,
    LM75_STATE_ERROR
} LM75_StateTypeDef;

/**
 * @brief Полярность вывода OS датчика LM75
 */
typedef enum
{
    LM75_POLARITY_LOW = 0,  /* Активный низкий уровень */
    LM75_POLARITY_HIGH = 1  /* Активный высокий уровень */
} LM75_OsPolarityTypeDef;

/**
 * @brief Режим прерывания по компаратору датчика LM75
 */
typedef enum
{
    LM75_COMPARATOR_INTERRUPT_DISABLE = 0, /* Режим компаратора */
    LM75_COMPARATOR_INTERRUPT_ENABLE = 1   /* Режим прерывания */
} LM75_InterruptModeTypeDef;

/**
 * @brief Режим пониженного потребления датчика LM75
 */
typedef enum
{
    LM75_SHUTDOWN_DISABLE = 0, /* Нормальный режим */
    LM75_SHUTDOWN_ENABLE = 1    /* Режим пониженного потребления */
} LM75_ShutdownModeTypeDef;

/**
 * @brief Конфигурация датчика LM75
 */
typedef struct
{
    LM75_OsPolarityTypeDef os_polarity;             /* Бит OS_POLARITY: 1 — высокий, 0 — низкий */
    LM75_InterruptModeTypeDef comparator_interrupt; /* Бит COMP_INT: 1 — прерывание, 0 — компаратор */
    LM75_ShutdownModeTypeDef shutdown_mode;         /* Бит SHUTDOWN: 1 — выключен, 0 — рабочий */
} LM75_ConfigTypedef;

/**
 * @brief Обработчик датчика LM75
 */
typedef struct
{
    uint8_t addr;                  /* Адрес устройства */
    LM75_StateTypeDef state;       /* Статус датчика */
    I2C_HandleTypeDef *hi2c;       /* Обработчик I2C интерфейса */
    LM75_ConfigTypedef config;     /* Конфигурация датчика */
} LM75_HandleTypeDef;

/* Exported functions --------------------------------------------------------*/

/**
 * @brief       Инициализация датчика температуры LM75
 * @param hlm75 Указатель на структуру обработчика LM75
 * @param hi2c  Указатель на структуру обработчика I2C интерфейса
 * @param addr  Адрес I2C датчика LM75 (обычно 0x4A)
 * @return      Статус операции
 */
LM75_StateTypeDef LM75_Init(LM75_HandleTypeDef *hlm75, I2C_HandleTypeDef *hi2c, uint8_t addr);

/**
 * @brief       Применение конфигурации к датчику LM75
 * @param hlm75 Указатель на дескриптор LM75
 * @return      Статус операции
 */
LM75_StateTypeDef LM75_ApplyConfig(LM75_HandleTypeDef *hlm75);

/**
 * @brief       Чтение значения температуры с датчика LM75
 * @param hlm75 Указатель на структуру обработчика LM75
 * @return      Значение температуры в градусах Цельсия
 */
float LM75_ReadTemp(LM75_HandleTypeDef *hlm75);

/**
 * @brief       Чтение текущего режима работы датчика LM75
 * @param hlm75 Указатель на структуру обработчика LM75
 * @return      Текущее значение регистра конфигурации датчика
 */
uint8_t LM75_GetConfig(LM75_HandleTypeDef *hlm75);

/**
 * @brief        Установка пользовательской конфигурации датчика LM75
 * @param hlm75  Указатель на структуру обработчика LM75
 * @param config Указатель на структуру с новой конфигурацией
 * @return       Статус операции
 */
LM75_StateTypeDef LM75_SetConfig(LM75_HandleTypeDef *hlm75, LM75_ConfigTypedef *config);

/**
 * @brief      Вывод значения температуры в консоль
 * @param temp Значение температуры в градусах Цельсия
 */
void LM75_PrintTemp(float temp);

#endif /* __LM75_H__ */
