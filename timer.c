// STM32F103
#include "stm32f10x.h"

//user
#include "main.h"
#include "timer.h"
#include "sound.h"

volatile uint32_t sys_tick = 0;

// SysTick timer interrupt
void SysTick_Handler(void)
{  
    sys_tick++;             // every 1ms
}
// TIM2 update interrupt
void TIM2_IRQHandler(void)
{       
    if (!(TIM2->SR & TIM_SR_UIF)) return;
    TIM2->SR &= ~TIM_SR_UIF;            // clear flag
}
// TIM1 update interrupt
void TIM1_UP_IRQHandler(void)
{       
    if (!(TIM1->SR & TIM_SR_UIF)) return;
    TIM1->SR &= ~TIM_SR_UIF;            // clear flag
    
    // Duty time from sound.c
    SetTim1DutyTime(GetNewDutyTime());
}
void InitSysTick(void)
{
    SysTick->LOAD = SYS_TIMER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL =	SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |    // interrupt enable
                    SysTick_CTRL_ENABLE_Msk;
}
void InitTim2(void)
{
    // Timer
    TIM2->CR1 = TIM_CR1_ARPE    * 0 |   // Auto-reload preload enable
                TIM_CR1_CMS_0   * 0 |   // Center-aligned mode selection : edge-aligned mode
                TIM_CR1_CMS_1   * 0 |
                TIM_CR1_DIR     * 0;    // Direction : upcounter
    TIM2->PSC = 0;                      // CK_CNT = 72MHz
    TIM2->ARR = 11;                     // 6MHz
    TIM2->EGR = TIM_EGR_UG;             // update event enable
    // DMA
    TIM2->DIER  |= TIM_DIER_UDE;        // update DMA request enable
    TIM2->CR2   |= TIM_CR2_CCDS;        // capture/compare or DMA selection
}
void SetTim2DutyTime(int duty_time)
{
    TIM2->CCR1 = duty_time;
}
void EnableTim2(void)
{
    TIM4->EGR = TIM_EGR_UG;     // update
    TIM2->CR1 |= TIM_CR1_CEN;   // start
}
void DisableTim2(void)
{
    TIM2->CR1 &= ~TIM_CR1_CEN;  // stop
}
void InitTim1(void)
/*{
    // Timer
    TIM1->CR1 =     TIM_CR1_CKD_0       * 0 |   // clock division for dead time : DTS = CK_INT
                    TIM_CR1_CKD_1       * 0 |
                    TIM_CR1_ARPE        * 0 |   // auto-reload preload enable
                    TIM_CR1_CMS_0       * 0 |   // center-aligned mode selection : edge-aligned mode
                    TIM_CR1_CMS_1       * 0 |
                    TIM_CR1_DIR         * 0;    // direction : upcounter
    
    TIM1->PSC = F_CPU / SAMPLING_RATE / BIT_DEPTH - 1;
    TIM1->ARR = BIT_DEPTH - 1;             // 32kHz
    
    // PWM
    TIM1->CCMR1 =   TIM_CCMR1_CC2S_0    * 0 |   // capture/compare 2 selection: compare
                    TIM_CCMR1_CC2S_1    * 0 |
                    TIM_CCMR1_OC2PE     * 0 |   // output compare 2 preload enable
                    TIM_CCMR1_OC2M_0    * 0 |   // output compare 2 mode: PWM mode 1 (active->inactive)
                    TIM_CCMR1_OC2M_1    * 1 |
                    TIM_CCMR1_OC2M_2    * 1;
    
    TIM1->CCMR2 =   TIM_CCMR2_CC3S_0    * 0 |   // capture/compare 3 selection: compare
                    TIM_CCMR2_CC3S_1    * 0 |
                    TIM_CCMR2_OC3PE     * 0 |   // output compare 3 preload enable
                    TIM_CCMR2_OC3M_0    * 0 |   // output compare 3 mode: PWM mode 1 (active->inactive)
                    TIM_CCMR2_OC3M_1    * 1 |
                    TIM_CCMR2_OC3M_2    * 1;
    
    TIM1->CCER =    TIM_CCER_CC2E       * 1 |   // capture/compare 2 output enable
                    TIM_CCER_CC2P       * 1 |   // capture/compare 2 output polarity: active low
                    TIM_CCER_CC3E       * 1 |   // capture/compare 3 output enable
                    TIM_CCER_CC3P       * 1;    // capture/compare 3 output polarity: active high
        
    TIM1->CCR2 = 0;             // channel 2 duty time = 0%
    TIM1->CCR3 = 0;             // channel 3 duty time = 0%
    
    TIM1->BDTR = TIM_BDTR_MOE        * 1;    // main output enable
    
    // Generate update event
    TIM1->EGR = TIM_EGR_UG;
    
    // Interrupt
    TIM1->DIER |= TIM_DIER_UIE;         // update interrupt enable
    
    // Start!
    TIM1->CR1 |= TIM_CR1_CEN;
}*/
{
    // Timer
    TIM1->CR1 =     TIM_CR1_CKD_0       * 0 |   // clock division for dead time : DTS = CK_INT
                    TIM_CR1_CKD_1       * 0 |
                    TIM_CR1_ARPE        * 0 |   // auto-reload preload enable
                    TIM_CR1_CMS_0       * 0 |   // center-aligned mode selection : edge-aligned mode
                    TIM_CR1_CMS_1       * 0 |
                    TIM_CR1_DIR         * 0;    // direction : upcounter
    
    TIM1->PSC = F_CPU / SAMPLING_RATE / BIT_DEPTH - 1;
    TIM1->ARR = BIT_DEPTH - 1;             // 32kHz
    
    // PWM
    TIM1->CCMR1 =   TIM_CCMR1_CC1S_0    * 0 |   // capture/compare 1 selection: compare
                    TIM_CCMR1_CC1S_1    * 0 |
                    TIM_CCMR1_OC1PE     * 0 |   // output compare 1 preload enable
                    TIM_CCMR1_OC1M_0    * 0 |   // output compare 1 mode: PWM mode 1 (active->inactive)
                    TIM_CCMR1_OC1M_1    * 1 |
                    TIM_CCMR1_OC1M_2    * 1 |
                        
                    TIM_CCMR1_CC2S_0    * 0 |   // capture/compare 2 selection: compare
                    TIM_CCMR1_CC2S_1    * 0 |
                    TIM_CCMR1_OC2PE     * 0 |   // output compare 2 preload enable
                    TIM_CCMR1_OC2M_0    * 0 |   // output compare 2 mode: PWM mode 1 (active->inactive)
                    TIM_CCMR1_OC2M_1    * 1 |
                    TIM_CCMR1_OC2M_2    * 1;
    
    TIM1->CCER =    TIM_CCER_CC1E       * 1 |   // capture/compare 1 output enable
                    TIM_CCER_CC1P       * 1 |   // capture/compare 1 output polarity: active low
                    TIM_CCER_CC1NE      * 1 |   // capture/compare 1 complementary output enable
                    TIM_CCER_CC1NP      * 0 |   // capture/compare 1 complementary output polarity: active high
                        
                    TIM_CCER_CC2E       * 1 |   // capture/compare 2 output enable
                    TIM_CCER_CC2P       * 1 |   // capture/compare 2 output polarity: active low
                    TIM_CCER_CC2NE      * 1 |   // capture/compare 2 complementary output enable
                    TIM_CCER_CC2NP      * 0;    // capture/compare 2 complementary output polarity: active high
        
    TIM1->CCR1 = 0;             // channel 1 duty time = 0%
    TIM1->CCR2 = 0;             // channel 2 duty time = 0%
    
    TIM1->BDTR =    0x30 |                      // dead time
                    TIM_BDTR_MOE        * 1;    // main output enable
    
    // Generate update event
    TIM1->EGR = TIM_EGR_UG;
    
    // Interrupt
    TIM1->DIER |= TIM_DIER_UIE;         // update interrupt enable
    
    // Start!
    TIM1->CR1 |= TIM_CR1_CEN;
}
void SetTim1DutyTime(uint16_t duty)
{
    if (duty >= 128) {
        TIM1->CCR1 = (duty - 128) & 0xFF;
        TIM1->CCR2 = 0;
    }
    else {
        TIM1->CCR1 = 0;
        TIM1->CCR2 = (128 - duty) & 0xFF;
    }
}

// Delay TIM4
void delay_ms(int time)
{
    TIM4->CR1 = 0;
    TIM4->SR = 0;
    TIM4->PSC = 35999;                  // 2kHz
    TIM4->ARR = time * 2 - 1;
    TIM4->EGR = TIM_EGR_UG;
    TIM4->SR &= ~TIM_SR_UIF;
    TIM4->CR1 |= TIM_CR1_CEN;
    while (!(TIM4->SR & TIM_SR_UIF))
        ;
    TIM4->CR1 &= ~TIM_CR1_CEN;
}
void delay_us(int time)
{
    TIM4->CR1 = 0;
    TIM4->SR = 0;
    TIM4->PSC = 35;                     // 2MHz
    TIM4->ARR = time * 2 - 1;
    TIM4->EGR = TIM_EGR_UG;
    TIM4->SR &= ~TIM_SR_UIF;
    TIM4->CR1 |= TIM_CR1_CEN;
    while (!(TIM4->SR & TIM_SR_UIF))
        ;
    TIM4->CR1 &= ~TIM_CR1_CEN;
}