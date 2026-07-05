/*
 * @file:   bus_usart.h
 * @brief:  Вспомогательный драйвер USART для примера
 */

#ifndef __BUS_USART_H__
#define __BUS_USART_H__

/* Includes ------------------------------------------------------------------*/
#include "mik32_hal_usart.h"
#include "xprintf.h"

/* Exported defines ----------------------------------------------------------*/
#define BAUDRATE  9600

/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief        Инициализация USART0
 * @param husart Указатель на структуру обработчика USART
 */
void USART_Init(USART_HandleTypeDef *husart);

#endif /* __BUS_USART_H__ */
