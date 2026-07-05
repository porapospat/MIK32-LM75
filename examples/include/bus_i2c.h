/*
 * @file:   bus_i2c.h
 * @brief:  Вспомогательный драйвер I2C для примера
 */

#ifndef __BUS_I2C_H__
#define __BUS_I2C_H__

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "mik32_hal_i2c.h"
#include "xprintf.h"

/* Exported defines ----------------------------------------------------------*/
#define I2C_WRITE_BUFFER_SIZE  16

/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief      Инициализация I2C
 * @param I2Cx Указатель на структуру обработчика I2C
 */
void I2C_Init(I2C_HandleTypeDef *I2Cx);

/**
 * @brief             Запись байта
 * @param hi2c        Указатель на структуру обработчика I2C
 * @param device_addr Адрес ведомого устройства
 * @param reg_addr    Адрес регистра
 * @param data        Байт данных для записи
 * @return            Статус операции записи
 */
HAL_StatusTypeDef I2C_write_byte(I2C_HandleTypeDef *hi2c, uint8_t device_addr, uint8_t reg_addr, uint8_t data);

/**
 * @brief             Чтение данных по I2C
 * @param hi2c        Указатель на структуру обработчика I2C
 * @param device_addr Адрес ведомого устройства
 * @param reg_addr    Адрес регистра
 * @param data        Буфер для копирования прочитанных данных
 * @param len         Количество байт для чтения
 * @return            Статус операции чтения
 */
HAL_StatusTypeDef I2C_read_data(I2C_HandleTypeDef *hi2c, uint8_t device_addr,
                                uint8_t reg_addr, uint8_t *data, uint16_t len);

/**
 * @brief             Запись данных по I2C
 * @param hi2c        Указатель на структуру обработчика I2C
 * @param device_addr Адрес ведомого устройства
 * @param reg_addr    Адрес регистра
 * @param data        Буфер с данными для записи
 * @param len         Количество байт для записи
 * @return            Статус операции записи
 */
HAL_StatusTypeDef I2C_write_data(I2C_HandleTypeDef *hi2c, uint8_t device_addr,
                                 uint8_t reg_addr, uint8_t *data, uint16_t len);

/**
 * @brief             Чтение байта
 * @param hi2c        Указатель на структуру обработчика I2C
 * @param device_addr Адрес ведомого устройства
 * @param reg_addr    Адрес регистра
 * @param data        Указатель на буфер для прочитанного байта
 * @return            Статус операции чтения
 */
HAL_StatusTypeDef I2C_read_byte(I2C_HandleTypeDef *hi2c, uint8_t device_addr,
                                uint8_t reg_addr, uint8_t *data);

#endif /* __BUS_I2C_H__ */
