/*
 * @file:   bus_usart.c
 * @brief:  Вспомогательный драйвер USART для примера
 */

/* Includes ------------------------------------------------------------------*/
#include "bus_usart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief        Инициализация USART0
 * @param husart Указатель на структуру обработчика USART
 */
void USART_Init(USART_HandleTypeDef *husart)
{
    husart->Instance = UART_0;

    husart->transmitting = Enable;
    husart->receiving = Disable;
    husart->frame = Frame_8bit;
    husart->parity_bit = Disable;
    husart->parity_bit_inversion = Disable;
    husart->bit_direction = LSB_First;
    husart->data_inversion = Disable;
    husart->tx_inversion = Disable;
    husart->rx_inversion = Disable;
    husart->swap = Disable;
    husart->lbm = Disable;
    husart->stop_bit = StopBit_1;
    husart->mode = Asynchronous_Mode;
    husart->xck_mode = XCK_Mode3;
    husart->last_byte_clock = Disable;
    husart->overwrite = Disable;
    husart->rts_mode = AlwaysEnable_mode;
    husart->dma_tx_request = Disable;
    husart->dma_rx_request = Disable;
    husart->channel_mode = Duplex_Mode;
    husart->tx_break_mode = Disable;
    husart->Interrupt.ctsie = Disable;
    husart->Interrupt.eie = Disable;
    husart->Interrupt.idleie = Disable;
    husart->Interrupt.lbdie = Disable;
    husart->Interrupt.peie = Disable;
    husart->Interrupt.rxneie = Disable;
    husart->Interrupt.tcie = Disable;
    husart->Interrupt.txeie = Disable;
    husart->Modem.rts = Disable;
    husart->Modem.cts = Disable;
    husart->Modem.dtr = Disable;
    husart->Modem.dcd = Disable;
    husart->Modem.dsr = Disable;
    husart->Modem.ri = Disable;
    husart->Modem.ddis = Disable;
    husart->baudrate = BAUDRATE;

    HAL_USART_Init(husart);
}
