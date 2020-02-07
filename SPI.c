// STM32F103
#include "stm32f10x.h"

// cstd
#include <stdbool.h>

//user
#include "SPI.h"
#include "main.h"
#include "GPIO.h"

// Local variables
// Data
static uint8_t* transmit_buf;
static uint8_t* receive_buf;
// Number of data
static int to_transmit = 0;
static int to_receive = 0;

// Interrupt
void SPI2_IRQHandler(void)
{
    // Transmit data register empty
    if ( (SPI2->SR & SPI_SR_TXE) != 0 ) {
        if (to_transmit != 0) {
            SPI2->DR = transmit_buf[0];
            transmit_buf++;
            to_transmit--;
        }
        else
            SPI2->CR2 &= ~SPI_CR2_TXEIE;    // TX buffer empty interrupt disable
    }
    // Received data register not empty
    if ((SPI2->SR & SPI_SR_RXNE) != 0) {
        if (to_receive != 0) {
            receive_buf[0] = SPI2->DR;
            receive_buf++;
            to_receive--;
        }
        else {
            (void)SPI2->DR;     // clear interrupt flag
        }
    }
}

void InitSpi(void)
{
    // Configuring SPI2 in master mode
    SPI2->CR1 =     SPI_CR1_BR_0        * 1 |   // baud rate = f_PCLK / (2 * 2^BR)
                    SPI_CR1_BR_1        * 1 |
                    SPI_CR1_BR_2        * 0;
    SPI2->CR1 |=    SPI_CR1_CPOL        * 0 |   // clock polarity = 0
                    SPI_CR1_CPHA        * 0 |   // clock phase = 0
                    SPI_CR1_DFF         * 0 |   // 8-bit data frame format
                    SPI_CR1_LSBFIRST    * 0 |   // MSB transmitted first
                    SPI_CR1_MSTR        * 1 |   // master mode
                    SPI_CR1_SSM         * 1 |   // software NSS enable
                    SPI_CR1_SSI         * 1;    // software NSS up
    SPI2->CR2 =     SPI_CR2_SSOE        * 1;    // SS output enable
    
    SPI2->CR1 |=    SPI_CR1_SPE;        // SPI enable
    SPI2->CR2 |=    SPI_CR2_RXNEIE;     // RX buffer not empty interrupt enable
    
    DeselectChipSpi();
}
void TransmitSpi(uint8_t* buf, int num)
{
    transmit_buf = buf;
    to_transmit = num - 1;
    
    SelectChipSpi();
    
    SPI2->DR = transmit_buf[0];
    transmit_buf++;
    
    SPI2->CR2 |= SPI_CR2_TXEIE;     // TX buffer empty interrupt enable
}
void ReceiveSpi(uint8_t* buf, int num)
{
    receive_buf = buf;
    to_receive = num;
}
bool TransmittedSpi(void)
{
    return ((SPI2->SR & SPI_SR_BSY) == 0);
}
bool ReceivedSpi(void)
{
    return ((SPI2->SR & SPI_SR_BSY) == 0);
}
void SelectChipSpi(void)
{
    ResetPin(SPI_NSS_PIN);
}
void DeselectChipSpi(void)
{
    SetPin(SPI_NSS_PIN);
}
