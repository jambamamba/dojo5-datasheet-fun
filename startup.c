/**
 * @file    startup.c
 * @brief   Startup functions for configuring RCC System and Bus Clock
 * @author  SMC
 * @date    September 2025
 *
 * This file contains function(s) to configure the RCC (Reset and Clock Control)
 * register to achieve a given system and bus clock.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "SMC_40CR.h"
#include "startup.h"

#define CLKSEL_SWITCH_MAX_TIME_IN_CYCLES     500UL
#define HSIRDY_MAX_TIME_IN_CYCLES            1400UL
#define HSERDY_MAX_TIME_IN_CYCLES            4200UL
#define RCC_CR_HSION                         (1UL << RCC_CR_HSION_Pos)  

uint32_t g_PllReadyTimeoutCycles = 350;

/**
 * @brief  Configures the system and bus clock config based on what is provided
 * @note   This function assumes a 40MHz starting clock. (Note: This is required 
 *         for HSE, and for HSI the internal clock is always 40MHz starting out).
 * @param sysClockSpeed Desired system clock speed enum
 * @param busClockDivider Desired bus clock divider
 * @param isHsiClock If true, then desired to use the internal HSI clock. If false, then use external HSE clock.
 * @retval 0 if success, -1 if timeout or error
 */
int32_t SetSystemAndBusClockConfig(System_Clock_Speeds_t sysClockSpeed, unsigned int BusClockDivider, bool isHsiClock)
{
  // Validate the system clock speed
  if ((sysClockSpeed > SYS_CLOCK_SPEED_MAX_ENUM_VAL) || (sysClockSpeed == SYS_CLOCK_SPEED_UNDEFINED))
  {
    return -1;
  }

  // Validate the bus clock divider
  unsigned int busClockRegVal = 0;
  switch (BusClockDivider) {
    case 0:
      busClockRegVal = 0;
      break;

    case 2: // Divide by 2
      busClockRegVal = RCC_CR_BUS_DIV_0; // Set bits 7-8 to 0b01
      break;
    
    case 4: // Divide by 4
      busClockRegVal = RCC_CR_BUS_DIV_1; // Set bits 7-8 to 0b10
      break;

    default: // Invalid value, just set busClockRegVal to 0
      busClockRegVal = 0;
      break;
  }

  // If everything is valid up to this point, unlock the RCC registers
  RCC_UNL = 0x56DD;
  RCC_UNH = 0xA3B2;

  // If the current clock is not the default clock, then we need to switch it
  // before proceeding
  volatile uint32_t rccCrReg = RCC_CR;
  if ((rccCrReg & RCC_CR_CLKSEL) != 0)
  {
    // Clock is not default clock. Fix it and wait for CLKSEL to show 0b00 indicating DEF_CLOCK is selected
    RCC_CR |= RCC_CR_DEF_CLOCK;
    while (1)
    {
      rccCrReg = RCC->CR;
      if ((rccCrReg & RCC_CR_CLKSEL) == 0)
      {
        // Exit the loop 
        break;
      }
    }
  }

  // Ensure the LOCK_STATUS bit shows unlocked
  uint32_t RccLockReg = RCC_LOCK;
  if ((RccLockReg & RCC_LOCK_LOCK_STATUS) != 0U) 
  {
    return -1;
  }

  // Configure PLL and SYS Clock divider for the desired clock speed
  // Start out by setting the SYS_DIV and BUS_DIV to 0. Also zero out the PLL register
  RCC->CR = (RCC->CR & ~RCC_CR_BUS_DIV);
  RCC->CR = (RCC->CR & ~RCC_CR_SYS_DIV);
  RCC_PLLCFGR = 0;
  switch (sysClockSpeed) 
  {
    case SYS_CLOCK_SPEED_160M:
      // Set a PLL Multiplication factor of 4 to get 160M (0b010 for MUL bits)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_MUL) | RCC_PLLCFGR_MUL_1;
      break;

    case SYS_CLOCK_SPEED_80M: 
      // Set a PLL Multiplication factor of 2 to get 80M (0b001)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_MUL) | RCC_PLLCFGR_MUL_0;
      break;

    case SYS_CLOCK_SPEED_40M: 
      // Set a PLL Multiplication factor of 1. Nothing to do!
      break;

        case SYS_CLOCK_SPEED_20M:
          // Set a PLL Division factor of 2 (0b001)
          RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_DIV) | RCC_PLLCFGR_DIV_0;
          break;

      case SYS_CLOCK_SPEED_10M: 
        // Set a PLL Division factor of 4 (0b010)
        RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_DIV) | RCC_PLLCFGR_DIV_1;
        break;

    case SYS_CLOCK_SPEED_5M: 
      // Set a PLL Division factor of 8 (0b100)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_DIV) | RCC_PLLCFGR_DIV_2;
      break;

    case SYS_CLOCK_SPEED_2_5M:      
      // Set a PLL Division factor of 8 and SYS_DIV value of 2 (0b01)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_DIV) | RCC_PLLCFGR_DIV_2;
      RCC_CR |= RCC_CR_SYS_DIV_0;
      break;

      case SYS_CLOCK_SPEED_1_25M:  
        // Set a PLL Division factor of 8 and SYS_DIV value of 4 (0b10)
        RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_DIV) | RCC_PLLCFGR_DIV_2;
        RCC_CR |= RCC_CR_SYS_DIV_1;
        break;

    case SYS_CLOCK_SPEED_UNDEFINED:   
    default:
      // Should never have got here since we already checked this. 
      // Should probably error out here
      break;
  }

  // Turn on the PLL and wait for it to be ready. Max time to be ready
  // is in CPU cycles so if we adhere to this value in a loop we'll be
  // sure to have waited that long.
  RCC_CR |= RCC_CR_PLLON;
  bool isPllReady = false;
                                  uint32_t timeoutCycles = 0U;
  do
  {
    rccCrReg = RCC_CR;
    isPllReady = ((rccCrReg & RCC_CR_PLL_RDY) != 0);
    timeoutCycles++;
  } while (!isPllReady && (timeoutCycles < g_PllReadyTimeoutCycles));

  // Set the BUS_DIV value so the bus clock is correct. 
  RCC_CR |= busClockRegVal;

  // Check if using internal or external clock
  bool isClkReady = false;
  if (isHsiClock)
  {
    // Internal clock!
    // Set HSION to 1 and wait for HSIRDY to be 1
    // Then set DEF_CLOCK to 0 and wait for CLKSEL to be HSI
    RCC_CR |= RCC_CR_HSEON;
    timeoutCycles = 0U;
    do
    {
      rccCrReg = RCC_CR;
      isClkReady = ((rccCrReg & RCC_CR_HSIRDY) != 0);
      timeoutCycles++;
    } while (!isClkReady && (timeoutCycles < HSIRDY_MAX_TIME_IN_CYCLES));
  }
  else
  {
    // External clock!
    // Set HSEON to 1 and wait for HSERDY to be 1
    RCC_CR |= RCC_CR_HSEON;
    timeoutCycles = 0U;
    do
    {
      rccCrReg = RCC_CR;
      isClkReady = ((rccCrReg & RCC_CR_HSERDY) != 0);
      timeoutCycles++;
    } while (!isClkReady && (timeoutCycles < HSERDY_MAX_TIME_IN_CYCLES));
  }

  // If neither hsi nor hse is ready then exit out!
  if (!isClkReady)
  {
    return -1;
  }

  // Finally, set DEF_CLOCK to 0 and wait for CLKSEL to be HSE or HSI
  uint8_t clockselTimeout = 0U;
  RCC_CR = (RCC_CR & ~RCC_CR_DEF_CLOCK);
  do
  {
    rccCrReg = RCC_CR;
    isClkReady = ((rccCrReg & RCC_CR_CLKSEL) != 0);
    clockselTimeout++;
  } while (!isClkReady && (clockselTimeout < CLKSEL_SWITCH_MAX_TIME_IN_CYCLES));
  
  // Verify
  if (isHsiClock)
  {
    // The clksel value should be 0b01 for HSI
    if ((rccCrReg & RCC_CR_CLKSEL) != RCC_CR_CLKSEL_0)
    {
      return -1;
    }
  }
  else
  {
    // The clksel value should be 0b10 for HSE
    if ((rccCrReg & RCC_CR_CLKSEL) != RCC_CR_CLKSEL_1)
    {
      return -1;
    }
  }

  // Success!! Re-lock the RCC registers
  RCC_LOCK |= RCC_LOCK_LOCK;

  return 0;
}
