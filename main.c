// STM32F103
#include "stm32f10x.h"

// cstd
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//#include <stdlib.h>

//user
#include "main.h"
#include "init.h"
#include "GPIO.h"
#include "timer.h"
#include "UART.h"
#include "debug_led.h"
#include "button.h"
#include "sound.h"
#include "SPI.h"
#include "W25Q64JV_memory.h"

// Sprintf buffer
char text_buf[50];

// Returns true, if jumper set left - need to rewrite memory
bool GetJumperState(void);

// Messages
const char mes1[] = "Press button to erase memory\n\r";
const char mes2[] = "Erasing...\n\r";
const char mes3[] = "Erasing done. Send file\n\r";
const char mes4[] = "Received %i bytes\n\r";

// Buffers for UART -> memory (SPI) data transmitting
uint8_t buf1[256];
uint8_t buf2[256];

// Starting external memory address of audio data
const int mem_start_addr = 0;


int main()
{    
    
    // System init
    InitRcc();
    InitGpio();
    InitSysTick();
    InitUart();
    InitInterrupt();
    InitTim2();
    InitTim1();
    InitSpi();
    
    // Debug
    //SetPin(USART_RX_PIN);
    //TurnLedOff();
    
    
    // Buttons
    btn_t Btn = {BTN_PIN, 0, 20, false, false, false};
    
    // Timings
    uint32_t BtnCheckCounter;
    
    TurnLedOff();
    
    // Rewrite memory from audio file
    if (GetJumperState()) {
        
        // "Press button to erase memory\n\r"
        TurnLedOn();
        TransmitUart((uint8_t*)mes1, strlen(mes1));
        while (! TransmittedUart())
            ;
        
        do {
            CheckBtn(&Btn);
            delay_ms(1);
        }
        while (! WasPressed(&Btn));

        // "Erasing...\n\r"
        TurnLedOff();
        TransmitUart((uint8_t*)mes2, strlen(mes2));
        while (! TransmittedUart())
            ;
        
        EraseMemory();
        WaitForNotBusy();
        
        // "Erasing done. Send file\n\r"
        TurnLedOn();
        TransmitUart((uint8_t*)mes3, strlen(mes3));
        while (! TransmittedUart())
            ;
        
        // "Received %i bytes\n\r"
        sprintf(text_buf, mes4, AudioFromUartToMem(buf1, buf2, mem_start_addr));
        TurnLedOff();
        TransmitUart((uint8_t*)text_buf, strlen(text_buf));
        while (! TransmittedUart())
            ;
        
    }

    // Audio file reading
    
    /*const int mem_start_addr = 0;
    uint8_t buf1[256];
    uint8_t buf2[256];
    
    PrintStringLnLcd("erasing...");
    
    EraseMemory();
    WaitForNotBusy();
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
    PrintStringLnLcd("start");

    number = AudioFromUartToMem(buf1, buf2, mem_start_addr);*/
    
    // Verification
    /*uint8_t mem_read;
    int i;
    
    for (i = 0; i < 32; i++) {
        ReadMemory(&mem_read, i, 1);
        
        TransmitUart(&mem_read, 1);
        while (! TransmittedUart())
            ;
    }*/
    
    // End of audio file reading
    
    
    // Start buttons check
    BtnCheckCounter = sys_tick;
    
  
    while (true) {
        
        // Buttons check
        if (sys_tick - BtnCheckCounter > 1) {
            BtnCheckCounter = sys_tick;
            
            CheckBtn(&Btn);
        }
        
        // Sound
        if (WasPressed(&Btn))
            StartTrack(mem_start_addr);
        UpdateSample();
        
        /*if (on_off)
            TurnLedOn();
        else
            TurnLedOff();*/
    }

}


bool GetJumperState()
{
    return ! GetPinState(JUMPER_PIN);
}

