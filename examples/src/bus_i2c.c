/*
 * @file:   bus_i2c.c
 * @brief:  Вспомогательный драйвер I2C для примера
 */

/* Includes ------------------------------------------------------------------*/
#include "bus_i2c.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief      Инициализация I2C
 * @param I2Cx Указатель на структуру обработчика I2C
 */
void I2C_Init(I2C_HandleTypeDef *I2Cx)
{
    I2Cx->Instance = I2C_0;

    /* Общие настройки */
    I2Cx->Init.Mode = HAL_I2C_MODE_MASTER;
    I2Cx->Init.DigitalFilter = I2C_DIGITALFILTER_OFF;
    I2Cx->Init.AnalogFilter = I2C_ANALOGFILTER_DISABLE;
    I2Cx->Init.AutoEnd = I2C_AUTOEND_ENABLE;

    /* Настройка частоты */
    I2Cx->Clock.PRESC = 1;
    I2Cx->Clock.SCLDEL = 15;
    I2Cx->Clock.SDADEL = 15;
    I2Cx->Clock.SCLH = 75;
    I2Cx->Clock.SCLL = 75;

    if (HAL_I2C_Init(I2Cx) != HAL_OK)
    {
        xprintf("I2C_Init: ошибка инициализации\n");
    }
}

/**
 * @brief             Запись байта
 * @param hi2c        Указатель на структуру обработчика I2C
 * @param device_addr Адрес ведомого устройства
 * @param reg_addr    Адрес регистра
 * @param data        Байт данных для записи
 * @return            Статус операции записи
 */
HAL_StatusTypeDef I2C_write_byte(I2C_HandleTypeDef *hi2c, uint8_t device_addr, uint8_t reg_addr, uint8_t data)
{
    return I2C_write_data(hi2c, device_addr, reg_addr, &data, 1);
}

/**
 * @brief             Чтение байта
 * @param hi2c        Указатель на структуру обработчика I2C
 * @param device_addr Адрес ведомого устройства
 * @param reg_addr    Адрес регистра
 * @param data        Указатель на буфер для прочитанного байта
 * @return            Статус операции чтения
 */
HAL_StatusTypeDef I2C_read_byte(I2C_HandleTypeDef *hi2c, uint8_t device_addr,
                                uint8_t reg_addr, uint8_t *data)
{
    return I2C_read_data(hi2c, device_addr, reg_addr, data, 1);
}

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
                                uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;

    /* Отправка адреса регистра */
    status = HAL_I2C_Master_Transmit(hi2c, device_addr, &reg_addr, 1, I2C_TIMEOUT_DEFAULT);
    if (status != HAL_OK)
    {
        xprintf("I2C_read_data: ошибка передачи #%d, код %d, ISR %d\n",
                status, hi2c->ErrorCode, hi2c->Instance->ISR);
        HAL_I2C_Reset(hi2c);
        return status;
    }

    /* Чтение данных */
    status = HAL_I2C_Master_Receive(hi2c, device_addr, data, len, I2C_TIMEOUT_DEFAULT);
    if (status != HAL_OK)
    {
        xprintf("I2C_read_data: ошибка приёма #%d, код %d, ISR %d\n",
                status, hi2c->ErrorCode, hi2c->Instance->ISR);
        HAL_I2C_Reset(hi2c);
    }

    return status;
}

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
                                 uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;
    uint8_t buffer[I2C_WRITE_BUFFER_SIZE];

    if (len > (I2C_WRITE_BUFFER_SIZE - 1U))
    {
        return HAL_ERROR;
    }

    buffer[0] = reg_addr;
    memcpy(&buffer[1], data, len);

    status = HAL_I2C_Master_Transmit(hi2c, device_addr, buffer, len + 1U, I2C_TIMEOUT_DEFAULT);
    if (status != HAL_OK)
    {
        xprintf("I2C_write_data: ошибка передачи #%d, код %d, ISR %d\n",
                status, hi2c->ErrorCode, hi2c->Instance->ISR);
        HAL_I2C_Reset(hi2c);
    }

    return status;
}
