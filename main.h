// STM32F103
#include "stm32f10x.h"

// cstd
#include <stdbool.h>

#ifndef __MAIN_H__
#define __MAIN_H__


// Ports and pin numbers
// Button
#define BTN_PIN                 GPIOB, 7
// Jumper
#define JUMPER_PIN              GPIOA, 7
// Onboard LED
#define LED_PIN                 GPIOC, 13
// USART
#define USART_TX_PIN            GPIOB, 10
#define USART_RX_PIN            GPIOB, 11
// SPI
#define SPI_NSS_PIN             GPIOB, 12
#define SPI_SCK_PIN             GPIOB, 13
#define SPI_MISO_PIN            GPIOB, 14
#define SPI_MOSI_PIN            GPIOB, 15
// TIM1 outputs for H-bridge PWM
#define EN_PIN                  GPIOB, 6
#define PWM_T1C2                GPIOA, 9
#define PWM_T1C3                GPIOA, 10

#endif
