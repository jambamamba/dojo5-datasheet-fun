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
#include "startup.h"
#include "logger.h"

/**
 * @brief  Configures the system and bus clock config based on what is provided
 * @note   This function assumes a 40MHz starting clock. (Note: This is required 
 *         for HSE, and for HSI the internal clock is always 40MHz starting out).
 * @param sysClockSpeed Desired system clock speed enum
 * @param busClockDivider Desired bus clock divider
 * @param isHsiClock If true, then desired to use the internal HSI clock. If false, then use external HSE clock.
 * @retval 0 if success, -1 if timeout or error
 */
int32_t SetSystemAndBusClockConfig(System_Clock_Speeds_t sysClockSpeed, unsigned int busClockDivider, bool isHsiClock) {

  if (!validateSystemClockSpeed(sysClockSpeed)) {
    LOG("Invalid sysClockSpeed value: %d", sysClockSpeed);
    return cleanup(SMC_40CR_ERROR_INVALID_SYS_CLOCK_SPEED);
  }
  if (!validateBusClockDivider(busClockDivider)) {
    LOG("Invalid busClockDivider value: %d", busClockDivider);
    return cleanup(SMC_40CR_ERROR_INVALID_BUS_CLOCK_DIVIDER);
  }
  if(!unlockRCC()) { //step 1. Unlock the RCC registers
    LOG("Failed to unlock RCC registers");
    return cleanup(SMC_40CR_ERROR_RCC_UNLOCK_FAILED);
  }
  if(!isDefaultClock()) { //initial boot must have default clock
    LOG("Switch to default clock");
    if(!switchDefaultClock()) {
      LOG("Failed to select default clock");
      return cleanup(SMC_40CR_ERROR_DEFAULT_CLOCK_SWITCH_FAILED);
    }
  }
  //step 2. Configure the RCC_PLLCFGR register to have the desired clock divider
  if(!configurePLL(sysClockSpeed)) {
    LOG("Failed to configure PLL");
    return cleanup(SMC_40CR_ERROR_PLL_CONFIGURATION_FAILED);
  }
  //step 3,4. Turn on the PLL and wait for it to be ready
  if(!turnOnPLL()) {
    LOG("Failed to turn on PLL");
    return cleanup(SMC_40CR_ERROR_PLL_STARTUP_FAILED);
  }
  //step 5. set clock dividers
  selectSystemClockDivider(sysClockSpeed);
  selectBusClockDivider(busClockDivider);

  //step 6,7. select internal/external clock and wait for clock switch
  if(!switchClock(isHsiClock)) {
    LOG("Failed to set clock");
    return cleanup(SMC_40CR_ERROR_CLOCK_SWITCH_FAILED);
  }

  LOG("Success!! Re-lock the RCC registers");
  //step 8. Re-lock the RCC registers
  lockRCC();

  return SMC_40CR_SUCCESS;
}
