/*
 * @file:   board.h
 * @brief:  Базовая конфигурация MIK32 Амур
 */

#ifndef __BOARD_H__
#define __BOARD_H__

/* Includes ------------------------------------------------------------------*/
#include "mik32_hal.h"
#include "mik32_hal_pcc.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief Конфигурация источника тактового сигнала
 */
void SystemClock_Config(void);

#endif /* __BOARD_H__ */
