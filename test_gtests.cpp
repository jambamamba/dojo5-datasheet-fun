
#include "gtest/gtest.h"
#include "startup.h"


extern "C" {

/**
 * Unfortunately we cannot include SMC_40CR.h directly 
 * because it creates concrete objects:   
 * RCC_TypeDef RCC_Val;
 * GPIO_TypeDef GPIOA_Val;
 * GPIO_TypeDef GPIOB_Val;
 * 
 * This results in multiple definition errors during linking.
 * We are not allowed to change this header file, otherwise we would declare those as extern in the header.
 * 
 * @file    SMC_40CR.h
 * @brief   Header file for SMC_40CR microcontroller
 * @version 1.0
 * @date    September 2025
 *
 * This file contains all the peripheral register definitions, bits definitions
 * and memory mapping for the SMC_40CR microcontroller.
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


RCC_TypeDef *getRCC();
}//extern "C" {  


TEST(SMC_40CR1, validateSystemClockSpeed) {
  ASSERT_FALSE(validateSystemClockSpeed(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_UNDEFINED-1)));
  ASSERT_FALSE(validateSystemClockSpeed(SYS_CLOCK_SPEED_UNDEFINED));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_160M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_80M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_40M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_20M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_10M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_5M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_2_5M));
  ASSERT_TRUE(validateSystemClockSpeed(SYS_CLOCK_SPEED_1_25M));
  ASSERT_FALSE(validateSystemClockSpeed(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_MAX_ENUM_VAL+1)));
}

TEST(SMC_40CR2, validateBusClockDivider) {
    ASSERT_TRUE(validateBusClockDivider(2));
    ASSERT_TRUE(validateBusClockDivider(4));
    ASSERT_FALSE(validateBusClockDivider(0));
    ASSERT_FALSE(validateBusClockDivider(999));
}

TEST(SMC_40CR2, switchDefaultClock) {
    ASSERT_TRUE(switchDefaultClock());
}
TEST(SMC_40CR2, configurePLL) {
  ASSERT_FALSE(configurePLL(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_UNDEFINED-1)));
  ASSERT_FALSE(configurePLL(SYS_CLOCK_SPEED_UNDEFINED));
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_160M));
  ASSERT_EQ(RCC_PLLCFGR_MUL_1, getRCC()->PLLCFGR & RCC_PLLCFGR_MUL);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_80M));
  ASSERT_EQ(RCC_PLLCFGR_MUL_0, getRCC()->PLLCFGR & RCC_PLLCFGR_MUL);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_40M));
  ASSERT_EQ(0, getRCC()->PLLCFGR & RCC_PLLCFGR_MUL);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_20M));
  ASSERT_EQ(RCC_PLLCFGR_DIV_0, getRCC()->PLLCFGR & RCC_PLLCFGR_DIV);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_10M));
  ASSERT_EQ(RCC_PLLCFGR_DIV_1, getRCC()->PLLCFGR & RCC_PLLCFGR_DIV);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_5M));
  ASSERT_EQ(RCC_PLLCFGR_DIV_2, getRCC()->PLLCFGR & RCC_PLLCFGR_DIV);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_2_5M));
  ASSERT_EQ(RCC_PLLCFGR_DIV_2, getRCC()->PLLCFGR & RCC_PLLCFGR_DIV);
  ASSERT_TRUE(configurePLL(SYS_CLOCK_SPEED_1_25M));
  ASSERT_EQ(RCC_PLLCFGR_DIV_2, getRCC()->PLLCFGR & RCC_PLLCFGR_DIV);
  ASSERT_FALSE(configurePLL(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_MAX_ENUM_VAL+1)));
}
selectSystemClockDivider(System_Clock_Speeds_t sysClockSpeed)

TEST(SMC_40CR2, selectSystemClockDivider) {
  selectSystemClockDivider(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_UNDEFINED-1));
  selectSystemClockDivider(SYS_CLOCK_SPEED_UNDEFINED);
  selectSystemClockDivider(SYS_CLOCK_SPEED_160M);
  selectSystemClockDivider(SYS_CLOCK_SPEED_80M);
  selectSystemClockDivider(SYS_CLOCK_SPEED_40M);
  selectSystemClockDivider(SYS_CLOCK_SPEED_20M);
  selectSystemClockDivider(SYS_CLOCK_SPEED_10M);
  selectSystemClockDivider(SYS_CLOCK_SPEED_5M);
  selectSystemClockDivider(SYS_CLOCK_SPEED_2_5M);
  selectSystemClockDivider(SYS_CLOCK_SPEED_1_25M);
  selectSystemClockDivider(static_cast<System_Clock_Speeds_t>(SYS_CLOCK_SPEED_MAX_ENUM_VAL+1));

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}