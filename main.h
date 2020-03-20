// STM32F103
#include "stm32f10x.h"

// cstd
#include <stdbool.h>

#ifndef __MAIN_H__
#define __MAIN_H__


// Ports and pin numbers
// Button
#define BTN_PIN                 GPIOA, 1
// Jumper
#define JUMPER_PIN              GPIOB, 9
// Onboard LED
#define LED_PIN                 GPIOC, 13
// USART
#define USART_TX_PIN            GPIOB, 6
#define USART_RX_PIN            GPIOB, 7
// SPI
#define SPI_NSS_PIN             GPIOA, 4
#define SPI_SCK_PIN             GPIOA, 5
#define SPI_MISO_PIN            GPIOA, 6
#define SPI_MOSI_PIN            GPIOA, 7
// TIM1 outputs for full-bridge PWM
#define PWM_1P                  GPIOA, 8
#define PWM_2P                  GPIOA, 9
#define PWM_1N                  GPIOB, 13
#define PWM_2N                  GPIOB, 14

#endif
