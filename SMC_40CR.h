/**
 * @file    SMC_40CR.h
 * @brief   Header file for SMC_40CR microcontroller
 * @version 1.0
 * @date    September 2025
 *
 * This file contains all the peripheral register definitions, bits definitions
 * and memory mapping for the SMC_40CR microcontroller.
 */

#ifndef SMC_40CR_H
#define SMC_40CR_H

#ifdef __cplusplus
extern "C" 
#endif

/** @addtogroup Configuration_section_for_CMSIS
  * @{
  */

/**
 * @brief Configuration of the Cortex-M4 Processor and Core Peripherals
 */
#define __CM4_REV                 0x0001U   /*!< Core revision r0p1                            */
#define __MPU_PRESENT             1U        /*!< SMC_40CR provides an MPU                      */
#define __NVIC_PRIO_BITS          4U        /*!< SMC_40CR uses 4 Bits for the Priority Levels  */
#define __Vendor_SysTickConfig    0U        /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1U        /*!< FPU present                                   */

/**
 * @}
 */

/** @addtogroup Peripheral_interrupt_number_definition
  * @{
  */

/**
 * @brief SMC_40CR Interrupt Number Definition
 */
typedef enum
{
/******  Cortex-M4 Processor Exceptions Numbers ******************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt        */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management   */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault           */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault         */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call            */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor      */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV            */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick        */

/******  SMC_40CR specific Interrupt Numbers *********************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt       */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection */
  TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamps           */
  RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt            */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt          */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt            */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt            */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt            */
  DMA1_Stream0_IRQn           = 11,     /*!< DMA1 Stream 0 global Interrupt  */
  ADC_IRQn                    = 18,     /*!< ADC1, ADC2 global Interrupts    */
  TIM1_UP_TIM10_IRQn          = 25,     /*!< TIM1 Update and TIM10 global    */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt  */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt           */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt         */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt         */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt            */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt            */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt           */
} IRQn_Type;

/**
 * @}
 */
#include <stdint.h>

/** @addtogroup Peripheral_registers_structures
  * @{
  */

/**
  * @brief Reset and Clock Control (RCC)
  */
typedef struct
{
  uint32_t CR;            /*!< RCC clock control register,                     Address offset: 0x00 */
  uint32_t PLLCFGR;       /*!< RCC PLL configuration register,                 Address offset: 0x04 */
  uint32_t UNL;           /*!< RCC unlock register low                         Address offset: 0x08 */
  uint32_t UNH;           /*!< RCC unlock register high                        Address offset: 0x0A */
  uint32_t LOCK;          /*!< RCC lock register                               Address offset: 0x0C */
  uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,             Address offset: 0x10 */
  uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,             Address offset: 0x14 */
  uint32_t RESERVED0[2];  /*!< Reserved, 0x18-0x1C                                                  */
  uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,             Address offset: 0x20 */
  uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,             Address offset: 0x24 */
  uint32_t RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                  */
  uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock enable register,      Address offset: 0x30 */
  uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock enable register,      Address offset: 0x34 */
  uint32_t RESERVED2[2];  /*!< Reserved, 0x38-0x3C                                                  */
  uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,      Address offset: 0x40 */
  uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,      Address offset: 0x44 */
  uint32_t RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                  */
  uint32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
  uint32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
  uint32_t RESERVED4[2];  /*!< Reserved, 0x58-0x5C                                                  */
  uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
  uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
  uint32_t RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                  */
  uint32_t BDCR;          /*!< RCC Backup domain control register,             Address offset: 0x70 */
  uint32_t CSR;           /*!< RCC clock control & status register,            Address offset: 0x74 */
} RCC_TypeDef;

/**
  * @brief General Purpose I/O
  */
typedef struct
{
  uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
  uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;

/**
  * @brief Universal Synchronous Asynchronous Receiver Transmitter
  */
typedef struct
{
  uint32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
  uint32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
  uint32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
  uint32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
  uint32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
  uint32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
  uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_TypeDef;

/**
 * @}
 */

/** @addtogroup Peripheral_memory_map
  * @{
  */
#define FLASH_BASE            0x08000000UL /*!< FLASH(up to 256 KB) base address in the alias region  */
#define SRAM_BASE             0x20000000UL /*!< SRAM(up to 64 KB) base address in the alias region    */
#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region           */

#define SRAM_BB_BASE          0x22000000UL /*!< SRAM base address in the bit-band region              */
#define PERIPH_BB_BASE        0x42000000UL /*!< Peripheral base address in the bit-band region        */

/*!< Peripheral memory map */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x10000000UL)

/*!< APB1 peripherals */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800UL)
#define RTC_BASE              (APB1PERIPH_BASE + 0x2800UL)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00UL)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000UL)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800UL)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400UL)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800UL)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800UL)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000UL)

/*!< APB2 peripherals */
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000UL)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000UL)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800UL)
#define USART1_BASE           (APB2PERIPH_BASE + 0x1000UL)
#define USART6_BASE           (APB2PERIPH_BASE + 0x1400UL)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2000UL)
#define ADC_BASE              (APB2PERIPH_BASE + 0x2300UL)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000UL)
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x3800UL)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x3C00UL)

/*!< AHB1 peripherals */
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000UL)
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00UL)
#define CRC_BASE              (AHB1PERIPH_BASE + 0x3000UL)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x3C00UL)
#define DMA1_BASE             (AHB1PERIPH_BASE + 0x6000UL)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x6400UL)

/**
 * @}
 */

/** @addtogroup Peripheral_declaration
  * @{
  */
#if (MOCK_REGISTERS == 1)
  RCC_TypeDef RCC_Val;
  GPIO_TypeDef GPIOA_Val;
  GPIO_TypeDef GPIOB_Val;
  
  RCC_TypeDef * RCC = &RCC_Val;
  GPIO_TypeDef * GPIOA = &GPIOA_Val;
  GPIO_TypeDef * GPIOB = &GPIOB_Val;
#else
  #define RCC                 ((RCC_TypeDef *) RCC_BASE)
  #define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
  #define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#endif // if (MOCK_REGISTERS == 1)

/**
 * @}
 */

/** @addtogroup Exported_constants
  * @{
  */
  
/** @addtogroup RCC_Exported_Constants
  * @{
  */

/** @defgroup RCC register accessors
  * @{
  */
#if (MOCK_REGISTERS == 1)
  #define RCC_CR                           RCC->CR
  #define RCC_PLLCFGR                      RCC->PLLCFGR
  #define RCC_UNL                          RCC->UNL
  #define RCC_UNH                          RCC->UNH
  #define RCC_LOCK                         RCC->LOCK
#else
  #define RCC_CR                           (*(volatile uint32_t *)(RCC_BASE + 0x00))
  #define RCC_PLLCFGR                      (*(volatile uint32_t *)(RCC_BASE + 0x04))
  #define RCC_UNL                          (*(volatile uint16_t *)(RCC_BASE + 0x08))
  #define RCC_UNH                          (*(volatile uint16_t *)(RCC_BASE + 0x0A))
  #define RCC_LOCK                         (*(volatile uint32_t *)(RCC_BASE + 0x0C))
#endif // if (MOCK_REGISTERS == 1)

/**
  * @}
  */

/** @defgroup RCC_CR_Register_Bits RCC CR Register Positions and Bits
  * @{
  */
#define RCC_CR_HSION_Pos                 0UL                                  /*!< Internal High Speed clock enable          */
#define RCC_CR_HSIRDY_Pos                1UL                                  /*!< Internal High Speed clock ready flag      */
#define RCC_CR_HSEON_Pos                 2UL                                  /*!< External High Speed clock enable          */
#define RCC_CR_HSERDY_Pos                3UL                                  /*!< External High Speed clock ready flag      */
#define RCC_CR_CLKSEL_Pos                4UL                                  /*!< Clock Select                              */
#define RCC_CR_PLLON_Pos                 6UL                                  /*!< PLL enable                                */
#define RCC_CR_BUS_DIV_Pos               7UL                                  /*!< Bus Clock Divider                         */
#define RCC_CR_SYS_DIV_Pos               9UL                                  /*!< System Clock Divider                      */
#define RCC_CR_PLL_RDY_Pos               11UL                                 /*!< PLL Ready flag                            */
#define RCC_CR_DEF_CLOCK_Pos             31UL                                 /*!< Default Clock Enable/Disable              */


#define RCC_CR_HSION                     (1UL << RCC_CR_HSION_Pos)            /*!< Internal High Speed clock enable          */
#define RCC_CR_HSIRDY                    (1UL << RCC_CR_HSIRDY_Pos)           /*!< Internal High Speed clock ready flag      */
#define RCC_CR_HSEON                     (1UL << RCC_CR_HSEON_Pos)            /*!< External High Speed clock enable          */
#define RCC_CR_HSERDY                    (1UL << RCC_CR_HSERDY_Pos)           /*!< External High Speed clock ready flag      */
#define RCC_CR_CLKSEL_0                  (1UL << RCC_CR_CLKSEL_Pos)           /*!< Clock Select 0 bit                        */
#define RCC_CR_CLKSEL_1                  (1UL << (RCC_CR_CLKSEL_Pos + 1UL))   /*!< Clock Select 1 bit                        */
#define RCC_CR_CLKSEL                    (RCC_CR_CLKSEL_0 | RCC_CR_CLKSEL_1)  /*!< CLKSEL[4:5] Clock Select bits             */
#define RCC_CR_PLLON                     (1UL << RCC_CR_PLLON_Pos)            /*!< PLL enable                                */
#define RCC_CR_BUS_DIV_0                 (1UL << RCC_CR_BUS_DIV_Pos)          /*!< Bus Clock Divider 0 bit                   */
#define RCC_CR_BUS_DIV_1                 (1UL << (RCC_CR_BUS_DIV_Pos + 1UL))  /*!< Bus Clock Divider 1 bit                   */
#define RCC_CR_BUS_DIV                   (RCC_CR_BUS_DIV_0 | RCC_CR_BUS_DIV_1)/*!< BUS_DIV[7:8] Bus Clock Divider bits       */
#define RCC_CR_SYS_DIV_0                 (1UL << RCC_CR_SYS_DIV_Pos)          /*!< System Clock Divider 0 bit                */
#define RCC_CR_SYS_DIV_1                 (1UL << (RCC_CR_SYS_DIV_Pos + 1UL))  /*!< System Clock Divider 1 bit                */
#define RCC_CR_SYS_DIV                   (RCC_CR_SYS_DIV_0 | RCC_CR_SYS_DIV_1)/*!< SYS_DIV[9:10] Bus Clock Divider bits      */
#define RCC_CR_PLL_RDY                   (1UL << RCC_CR_PLL_RDY_Pos)          /*!< PLL_RDY flag bit                          */
#define RCC_CR_DEF_CLOCK                 (1UL << RCC_CR_DEF_CLOCK_Pos)        /*!< Default Clock Enable/Disable              */

/**
  * @}
  */

/** @defgroup RCC_PLLCFGR_Register_Bits RCC PLLCFGR Register Positions and Bits
  * @{
  */
#define RCC_PLLCFGR_DIV_Pos              0UL                                /*!< Main PLL division factor                  */
#define RCC_PLLCFGR_MUL_Pos              3UL                                /*!< Main PLL multiplication factor            */

#define RCC_PLLCFGR_DIV_0                (1UL << RCC_PLLCFGR_DIV_Pos)       /*!< PLL division factor bit 0                 */      
#define RCC_PLLCFGR_DIV_1                (1UL << (RCC_PLLCFGR_DIV_Pos + 1)) /*!< PLL division factor bit 1                 */   
#define RCC_PLLCFGR_DIV_2                (1UL << (RCC_PLLCFGR_DIV_Pos + 2)) /*!< PLL division factor bit 2                 */
#define RCC_PLLCFGR_DIV                  (RCC_PLLCFGR_DIV_0 | \
                                          RCC_PLLCFGR_DIV_1 | \
                                          RCC_PLLCFGR_DIV_2)                /*!< DIV[0:3] PLL division factor bits         */  

#define RCC_PLLCFGR_MUL_0                (1UL << RCC_PLLCFGR_MUL_Pos)       /*!< PLL multiplication factor bit 0           */      
#define RCC_PLLCFGR_MUL_1                (1UL << (RCC_PLLCFGR_MUL_Pos + 1)) /*!< PLL multiplication factor bit 1           */   
#define RCC_PLLCFGR_MUL_2                (1UL << (RCC_PLLCFGR_MUL_Pos + 2)) /*!< PLL multiplication factor bit 2           */
#define RCC_PLLCFGR_MUL                  (RCC_PLLCFGR_MUL_0 | \
                                          RCC_PLLCFGR_MUL_1 | \
                                          RCC_PLLCFGR_MUL_2)                /*!< MUL[0:3] PLL multiplication factor bits   */  

/**
  * @}
  */

/** @defgroup RCC_UNL_Register_Bits RCC UNL Register Positions and Bits
  * @{
  */
#define RCC_UNL_UNLOCK_Pos               0UL                                /*!< Unlock low register                       */
#define RCC_UNL_UNLOCK                   0xFFFFUL     

/**
  * @}
  */

/** @defgroup RCC_UNH_Register_Bits RCC UNH Register Positions and Bits
  * @{
  */
#define RCC_UNH_UNLOCK_Pos               0UL                                /*!< Unlock high register                      */
#define RCC_UNH_UNLOCK                   0xFFFFUL     

/**
  * @}
  */

/** @defgroup RCC_LOCK_Register_Bits RCC LOCK Register Positions and Bits
  * @{
  */
#define RCC_LOCK_LOCK_Pos                0UL                                /*!< Lock bit position                         */
#define RCC_LOCK_LOCK_STATUS_Pos         1UL                                /*!< Lock status bit position                  */

#define RCC_LOCK_LOCK                    (1UL << RCC_LOCK_LOCK_Pos)         /*!< RCC Lock bit                              */ 
#define RCC_LOCK_LOCK_STATUS             (1UL << RCC_LOCK_LOCK_STATUS_Pos)  /*!< RCC Lock status bit                       */ 

/**
  * @}
  */

#ifdef __cplusplus

#endif /* __cplusplus */

#endif /* SMC_40CR_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT SMC *****END OF FILE****/
