/******************** (C) xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx ********************
* File Name:        startup_stm32f10x_hd.c
* Author:           Renhaibo
* Version:          V1.0.0
* Date:             2009.11.08
* Description:      STM32F10x High Density Devices vector table for EWARM5.x 
*                   toolchain.
*                   This module performs:
*                   - Set the initial SP
*                   - Set the initial PC == __iar_program_start,
*                   - Set the vector table entries with the exceptions ISR address,
*                   - Configure FSMC,can add by the user
*                   After Reset the Cortex-M3 processor is in Thread mode,
*                   priority is Privileged, and the Stack is set to Main.
*******************************************************************************/
//*****************************************************************************
//
// Enable the IAR extensions for this source file.
//
//*****************************************************************************
#pragma language=extended

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

//*****************************************************************************
//
// External declarations for the interrupt handlers used by the application.
//
//*****************************************************************************


//*****************************************************************************
//
// The entry point for the application startup code.
//
//*****************************************************************************
extern void __iar_program_start(void);
extern void OS_CPU_SysTickHandler (void);

//*****************************************************************************
//
// A union that describes the entries of the vector table.  The union is needed
// since the first entry is the stack pointer and the remainder are function
// pointers.
//
//*****************************************************************************
typedef union
{
    void (*pfnHandler)(void);
    void * __ptr;
}
uVectorEntry;

#pragma segment="CSTACK"

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address aglian 256 bytes.
//
//*****************************************************************************
__root const uVectorEntry __vector_table[] @ ".intvec" =
{
    { .__ptr = __sfe( "CSTACK" ) },             //  The initial stack pointer
    __iar_program_start,                        //  The reset handler
    NmiSR,                                      //  NMI Handler
    FaultISR,                                   //  Hard Fault Handler
    IntDefaultHandler,                          //  MPU Fault Handler
    IntDefaultHandler,                          //  Bus Fault Handler
    IntDefaultHandler,                          //  Usage Fault Handler
    0,                                          //  Reserved
    0,                                          //  Reserved
    0,                                          //  Reserved
    0,                                          //  Reserved
    IntDefaultHandler,                          //  SVCall Handler
    IntDefaultHandler,                          //  Debug Monitor Handler
    0,                                          //  Reserved
    IntDefaultHandler,                          //  PendSV Handler
    OS_CPU_SysTickHandler,                          //  SysTick Handler
    
    // External Interrupts
    IntDefaultHandler,                          //  Window Watchdog
    IntDefaultHandler,                          //  PVD through EXTI Line detect
    IntDefaultHandler,                          //  Tamper
    IntDefaultHandler,                          //  RTC
    IntDefaultHandler,                          //  Flash
    IntDefaultHandler,                          //  RCC
    IntDefaultHandler,                          //  EXTI Line 0
    IntDefaultHandler,                          //  EXTI Line 1
    IntDefaultHandler,                          //  EXTI Line 2
    IntDefaultHandler,                          //  EXTI Line 3
    IntDefaultHandler,                          //  EXTI Line 4
    IntDefaultHandler,                          //  DMA1 Channel 1
    IntDefaultHandler,                          //  DMA1 Channel 2
    IntDefaultHandler,                          //  DMA1 Channel 3
    IntDefaultHandler,                          //  DMA1 Channel 4
    IntDefaultHandler,                          //  DMA1 Channel 5
    IntDefaultHandler,                          //  DMA1 Channel 6
    IntDefaultHandler,                          //  DMA1 Channel 7
    IntDefaultHandler,                          //  ADC1 & ADC2
    IntDefaultHandler,                          //  USB High Priority or CAN1 TX
    IntDefaultHandler,                          //  USB Low  Priority or CAN1 RX0
    IntDefaultHandler,                          //  CAN1 RX1
    IntDefaultHandler,                          //  CAN1 SCE
    IntDefaultHandler,                          //  EXTI Line 9..5
    IntDefaultHandler,                          //  TIM1 Break
    IntDefaultHandler,                          //  TIM1 Update
    IntDefaultHandler,                          //  TIM1 Trigger and Commutation
    IntDefaultHandler,                          //  TIM1 Capture Compare
    IntDefaultHandler,                          //  TIM2
    IntDefaultHandler,                          //  TIM3
    IntDefaultHandler,                          //  TIM4
    IntDefaultHandler,                          //  I2C1 Event
    IntDefaultHandler,                          //  I2C1 Error
    IntDefaultHandler,                          //  I2C2 Event
    IntDefaultHandler,                          //  I2C2 Error
    IntDefaultHandler,                          //  SPI1
    IntDefaultHandler,                          //  SPI2
    IntDefaultHandler,                          //  USART1
    IntDefaultHandler,                          //  USART2
    IntDefaultHandler,                          //  USART3
    IntDefaultHandler,                          //  EXTI Line 15..10
    IntDefaultHandler,                          //  RTC Alarm through EXTI Line
    IntDefaultHandler,                          //  USB Wakeup from suspend
    IntDefaultHandler,                          //  TIM8 Break
    IntDefaultHandler,                          //  TIM8 Update
    IntDefaultHandler,                          //  TIM8 Trigger and Commutation
    IntDefaultHandler,                          //  TIM8 Capture Compare
    IntDefaultHandler,                          //  ADC3
    IntDefaultHandler,                          //  FSMC
    IntDefaultHandler,                          //  SDIO
    IntDefaultHandler,                          //  TIM5
    IntDefaultHandler,                          //  SPI3
    IntDefaultHandler,                          //  UART4
    IntDefaultHandler,                          //  UART5
    IntDefaultHandler,                          //  TIM6
    IntDefaultHandler,                          //  TIM7
    IntDefaultHandler,                          //  DMA2 Channel1
    IntDefaultHandler,                          //  DMA2 Channel2
    IntDefaultHandler,                          //  DMA2 Channel3
    IntDefaultHandler                           //  DMA2 Channel4 & Channel5
};
//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void
NmiSR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
FaultISR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while(1)
    {
    }
}
/*******************************END OF FILE**********************************/
