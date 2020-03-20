// STM32F103
#include "stm32f10x.h"

// user
#include "init.h"
#include "main.h"
#include "gpio.h"

void InitRcc(void)
{
    // port A
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // port B
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // port C
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // TIM4
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    // TIM2
    //RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // TIM1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    // USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    // AFIO
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
}
void InitInterrupt(void)
{
    // TIM1
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    // USART1
    NVIC_EnableIRQ(USART1_IRQn);
    // SPI1
    NVIC_EnableIRQ(SPI1_IRQn);
}
void InitGpio(void)
{   
    // Remap USART
    AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
    
    // Debug LED
    SetGpioMode(LED_PIN, PIN_MODE_2MHZ_OPP);
    
    // Button
    SetGpioMode(BTN_PIN, PIN_MODE_IFLOAT);
    // Jumper
    SetGpioMode(JUMPER_PIN, PIN_MODE_IFLOAT);

    // USART
    SetGpioMode(USART_TX_PIN, PIN_MODE_2MHZ_AFPP);
    SetGpioMode(USART_RX_PIN, PIN_MODE_IFLOAT);
    
    // TIM1 PWM
    SetGpioMode(PWM_1P, PIN_MODE_2MHZ_AFOD);
    SetGpioMode(PWM_2P, PIN_MODE_2MHZ_AFOD);
    SetGpioMode(PWM_1N, PIN_MODE_2MHZ_AFPP);
    SetGpioMode(PWM_2N, PIN_MODE_2MHZ_AFPP);
    
    // SPI
    SetGpioMode(SPI_NSS_PIN, PIN_MODE_10MHZ_OPP);
    SetGpioMode(SPI_SCK_PIN, PIN_MODE_10MHZ_AFPP);
    SetGpioMode(SPI_MISO_PIN, PIN_MODE_10MHZ_AFPP);
    SetGpioMode(SPI_MOSI_PIN, PIN_MODE_10MHZ_AFPP);
}
