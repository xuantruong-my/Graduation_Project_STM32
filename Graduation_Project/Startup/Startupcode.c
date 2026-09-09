#include <stdint.h>

#define RAM_START           0x20000000U
#define RAM_SIZE            (10U * 1024U)
#define RAM_END             (RAM_START + RAM_SIZE)
#define START_STACK         RAM_END

extern uint32_t _estack;
extern uint32_t end_of_rodata;
extern uint32_t start_of_data;
extern uint32_t end_of_data;
extern uint32_t start_of_bss;
extern uint32_t end_of_bss;
extern uint32_t data_in_flash;

extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);

//=============================================Thiết lập các hàm thực thi ngắt=======================================//

// --- Exception ---
void NMI_Handler(void)                      __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler(void)                __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler(void)                __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler(void)               __attribute__ ((weak, alias("Default_Handler")));
void SVCall_Handler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void DebugMonitor_Handler(void)             __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler(void)                  __attribute__ ((weak, alias("Default_Handler")));

// --- System Interrupts ---            
void WWDG_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void PVD_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void TAMPER_IRQHandler(void)                __attribute__ ((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void RCC_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));

// --- RTC & Backup ---         
void RTC_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void RTCAlarm_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));

// --- EXTI ---         
void EXTI0_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));

// --- DMA ---
void DMA1_Channel1_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel1_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel2_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel3_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel4_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel5_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));

// --- ADC ---
void ADC1_2_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));

// --- CAN ---              
void CAN1_TX_IRQHandler(void)                __attribute__ ((weak, alias("Default_Handler")));
void CAN1_RX0_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void CAN2_TX_IRQHandler(void)                __attribute__ ((weak, alias("Default_Handler")));
void CAN2_RX0_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void CAN2_RX1_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void CAN2_SCE_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));

// --- Timer ---                
void TIM1_BRK_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void TIM1_UP_IRQHandler(void)                __attribute__ ((weak, alias("Default_Handler")));
void TIM1_TRG_COM_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)                __attribute__ ((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void TIM5_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void TIM6_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void TIM7_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));

// --- I2C ---
void I2C1_EV_IRQHandler(void)                __attribute__ ((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void)                __attribute__ ((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void)                __attribute__ ((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void)                __attribute__ ((weak, alias("Default_Handler")));
             
// --- SPI ---           
void SPI1_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
void SPI3_IRQHandler(void)                   __attribute__ ((weak, alias("Default_Handler")));
             
// --- UART ---          
void USART1_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void USART2_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void USART3_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void UART4_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void UART5_IRQHandler(void)                  __attribute__ ((weak, alias("Default_Handler")));
             
// --- USB & Ethernet ---
void OTG_FS_WKUP_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void OTG_FS_IRQHandler(void)                 __attribute__ ((weak, alias("Default_Handler")));
void ETH_IRQHandler(void)                    __attribute__ ((weak, alias("Default_Handler")));
void ETH_WKUP_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));

typedef void (*function)(void);


//==================================================Khởi tạo bảng vector table=====================================//

function vector_table[] __attribute__((section(".vector_table"))) = {
    (function)&_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,             
    SVCall_Handler,
    DebugMonitor_Handler,
    0,                     
    PendSV_Handler,
    SysTick_Handler,
    WWDG_IRQHandler,        
    PVD_IRQHandler,
    TAMPER_IRQHandler,
    RTC_IRQHandler,
    FLASH_IRQHandler,       
    RCC_IRQHandler,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler,
    EXTI2_IRQHandler,
    EXTI3_IRQHandler,
    EXTI4_IRQHandler,
    DMA1_Channel1_IRQHandler,
    DMA1_Channel2_IRQHandler,
    DMA1_Channel3_IRQHandler,
    DMA1_Channel4_IRQHandler,
    DMA1_Channel5_IRQHandler,
    DMA1_Channel6_IRQHandler,
    DMA1_Channel7_IRQHandler,
    ADC1_2_IRQHandler,      
    CAN1_TX_IRQHandler,     
    CAN1_RX0_IRQHandler,
    CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler,
    EXTI9_5_IRQHandler,
    TIM1_BRK_IRQHandler,
    TIM1_UP_IRQHandler,
    TIM1_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler,
    TIM2_IRQHandler,
    TIM3_IRQHandler,
    TIM4_IRQHandler,
    I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    USART2_IRQHandler,
    USART3_IRQHandler,
    EXTI15_10_IRQHandler,   
    RTCAlarm_IRQHandler,    
    OTG_FS_WKUP_IRQHandler, 
    0, 0, 0, 0, 0, 0, 0,    // Reserved
    TIM5_IRQHandler,        
    SPI3_IRQHandler,
    UART4_IRQHandler,
    UART5_IRQHandler,
    TIM6_IRQHandler,
    TIM7_IRQHandler,
    DMA2_Channel1_IRQHandler,
    DMA2_Channel2_IRQHandler,
    DMA2_Channel3_IRQHandler,
    DMA2_Channel4_IRQHandler,
    DMA2_Channel5_IRQHandler,
    ETH_IRQHandler,         
    ETH_WKUP_IRQHandler,
    CAN2_TX_IRQHandler,
    CAN2_RX0_IRQHandler,
    CAN2_RX1_IRQHandler,
    CAN2_SCE_IRQHandler,
    OTG_FS_IRQHandler       
};

//==================Copy dữ liệu từ flash sang ram, xóa các ô nhớ phân vùng .bss===================//

void Reset_Handler(void)
{
    //Copy data
    uint32_t size_of_LMA = (uint32_t)&end_of_data - (uint32_t)&start_of_data;
    uint8_t *pLMA = (uint8_t *)&data_in_flash;
    uint8_t *pVMA = (uint8_t *)&start_of_data;

    for(uint32_t i = 0; i < size_of_LMA; i++)
    {
        *pVMA++ = *pLMA++;
    }

    //Xoa bss
    uint32_t size_of_BSS = (uint32_t)&end_of_bss - (uint32_t)&start_of_bss;
    uint8_t *pBSS = (uint8_t *)&start_of_bss;

    for(uint32_t i = 0; i < size_of_BSS; i++)
    {
        *pBSS++ = 0;
    }

    main();
    while(1);
}

void Default_Handler(void)
{
    while(1)
    {
        
    }
}