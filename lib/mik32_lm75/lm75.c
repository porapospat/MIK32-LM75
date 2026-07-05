/*
 * @file:   lm75.c
 * @brief:  Библиотека для работы с датчиком температуры LM75
 */

/* Includes ------------------------------------------------------------------*/
#include "lm75.h"
#include "xprintf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief             Чтение данных из регистра LM75 по I2C
 * @param hi2c        Указатель на обработчик I2C
 * @param device_addr Адрес устройства
 * @param reg_addr    Адрес регистра
 * @param data        Буфер для чтения
 * @param len         Количество байт
 * @return            Статус операции HAL
 */
static HAL_StatusTypeDef lm75_i2c_read_reg(I2C_HandleTypeDef *hi2c, uint8_t device_addr,
                                           uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_Master_Transmit(hi2c, device_addr, &reg_addr, 1, I2C_TIMEOUT_DEFAULT);
    if (status != HAL_OK)
    {
        return status;
    }

    return HAL_I2C_Master_Receive(hi2c, device_addr, data, len, I2C_TIMEOUT_DEFAULT);
}

/**
 * @brief             Запись байта в регистр LM75 по I2C
 * @param hi2c        Указатель на обработчик I2C
 * @param device_addr Адрес устройства
 * @param reg_addr    Адрес регистра
 * @param data        Записываемый байт
 * @return            Статус операции HAL
 */
static HAL_StatusTypeDef lm75_i2c_write_reg(I2C_HandleTypeDef *hi2c, uint8_t device_addr,
                                            uint8_t reg_addr, uint8_t data)
{
    uint8_t buffer[2] = {reg_addr, data};

    return HAL_I2C_Master_Transmit(hi2c, device_addr, buffer, 2, I2C_TIMEOUT_DEFAULT);
}

/**
 * @brief       Инициализация датчика температуры LM75
 * @param hlm75 Указатель на структуру обработчика LM75
 * @param hi2c  Указатель на структуру обработчика I2C интерфейса
 * @param addr  Адрес I2C датчика LM75
 * @return      Статус операции
 */
LM75_StateTypeDef LM75_Init(LM75_HandleTypeDef *hlm75, I2C_HandleTypeDef *hi2c, uint8_t addr)
{
    if (hlm75 == NULL || hi2c == NULL)
    {
        return LM75_STATE_ERROR;
    }

    hlm75->addr = addr;
    hlm75->hi2c = hi2c;
    hlm75->state = LM75_STATE_OK;

    hlm75->config.os_polarity = LM75_POLARITY_LOW;
    hlm75->config.comparator_interrupt = LM75_COMPARATOR_INTERRUPT_DISABLE;
    hlm75->config.shutdown_mode = LM75_SHUTDOWN_DISABLE;

    return LM75_ApplyConfig(hlm75);
}

/**
 * @brief       Применение конфигурации к датчику LM75
 * @param hlm75 Указатель на дескриптор LM75
 * @return      Статус операции
 */
LM75_StateTypeDef LM75_ApplyConfig(LM75_HandleTypeDef *hlm75)
{
    uint8_t reg_config = 0;

    if (hlm75 == NULL || hlm75->hi2c == NULL)
    {
        return LM75_STATE_ERROR;
    }

    reg_config |= (hlm75->config.os_polarity & 0x01U) << 2;
    reg_config |= (hlm75->config.comparator_interrupt & 0x01U) << 1;
    reg_config |= (hlm75->config.shutdown_mode & 0x01U) << 0;

    if (lm75_i2c_write_reg(hlm75->hi2c, hlm75->addr, LM75_REG_CONFIG, reg_config) != HAL_OK)
    {
        hlm75->state = LM75_STATE_ERROR;
        return LM75_STATE_ERROR;
    }

    hlm75->state = LM75_STATE_OK;
    return LM75_STATE_OK;
}

/**
 * @brief       Чтение значения температуры с датчика LM75
 * @param hlm75 Указатель на структуру обработчика LM75
 * @return      Значение температуры в градусах Цельсия
 */
float LM75_ReadTemp(LM75_HandleTypeDef *hlm75)
{
    uint8_t data[LM75_DATA_SIZE];
    int16_t temp;
    float temperature;

    if (hlm75 == NULL || hlm75->hi2c == NULL)
    {
        return 0.0f;
    }

    if (lm75_i2c_read_reg(hlm75->hi2c, hlm75->addr, LM75_REG_TEMP, data, LM75_DATA_SIZE) != HAL_OK)
    {
        hlm75->state = LM75_STATE_ERROR;
        return 0.0f;
    }

    temp = (int16_t)((data[0] << 8) | data[1]);
    temp = temp >> 5;

    if (temp & 0x0400)
    {
        temp |= 0xF800;
    }

    temperature = (float)temp * 0.125f;

    hlm75->state = LM75_STATE_OK;
    return temperature;
}

/**
 * @brief       Чтение текущего режима работы датчика LM75
 * @param hlm75 Указатель на структуру обработчика LM75
 * @return      Текущее значение регистра конфигурации датчика
 */
uint8_t LM75_GetConfig(LM75_HandleTypeDef *hlm75)
{
    uint8_t config = 0;

    if (hlm75 == NULL || hlm75->hi2c == NULL)
    {
        return 0;
    }

    if (lm75_i2c_read_reg(hlm75->hi2c, hlm75->addr, LM75_REG_CONFIG, &config, 1) != HAL_OK)
    {
        hlm75->state = LM75_STATE_ERROR;
        return 0;
    }

    hlm75->config.os_polarity = (LM75_OsPolarityTypeDef)((config >> 2) & 0x01U);
    hlm75->config.comparator_interrupt = (LM75_InterruptModeTypeDef)((config >> 1) & 0x01U);
    hlm75->config.shutdown_mode = (LM75_ShutdownModeTypeDef)((config >> 0) & 0x01U);

    hlm75->state = LM75_STATE_OK;
    return config;
}

/**
 * @brief        Установка пользовательской конфигурации датчика LM75
 * @param hlm75  Указатель на структуру обработчика LM75
 * @param config Указатель на структуру с новой конфигурацией
 * @return       Статус операции
 */
LM75_StateTypeDef LM75_SetConfig(LM75_HandleTypeDef *hlm75, LM75_ConfigTypedef *config)
{
    uint8_t reg_config = 0;

    if (hlm75 == NULL || hlm75->hi2c == NULL || config == NULL)
    {
        return LM75_STATE_ERROR;
    }

    hlm75->config.os_polarity = config->os_polarity;
    hlm75->config.comparator_interrupt = config->comparator_interrupt;
    hlm75->config.shutdown_mode = config->shutdown_mode;

    reg_config |= (hlm75->config.os_polarity & 0x01U) << 2;
    reg_config |= (hlm75->config.comparator_interrupt & 0x01U) << 1;
    reg_config |= (hlm75->config.shutdown_mode & 0x01U) << 0;

    if (lm75_i2c_write_reg(hlm75->hi2c, hlm75->addr, LM75_REG_CONFIG, reg_config) != HAL_OK)
    {
        hlm75->state = LM75_STATE_ERROR;
        return LM75_STATE_ERROR;
    }

    hlm75->state = LM75_STATE_OK;
    return LM75_STATE_OK;
}

/**
 * @brief      Вывод значения температуры в консоль
 * @param temp Значение температуры в градусах Цельсия
 */
void LM75_PrintTemp(float temp)
{
    int int_part = (int)temp;
    int frac_part = (int)((temp - int_part) * 100);

    if (frac_part < 0)
    {
        frac_part = -frac_part;
    }

    if (int_part < 0)
    {
        xprintf("Temperature: -%d.%02d C\n", -int_part, frac_part);
    }
    else
    {
        xprintf("Temperature: %d.%02d C\n", int_part, frac_part);
    }
}
