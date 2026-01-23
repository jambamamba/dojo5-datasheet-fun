/**
 * @file    startup.h
 * @brief   Startup definitions
 * @author  SMC
 * @date    September 2025
 *
 * This file contains function definitions and enums to configure the RCC (Reset and Clock Control)
 * register to achieve a given system and bus clock.
 *
 */

#ifndef STARTUP__H
#define STARTUP__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum {
  SMC_40CR_SUCCESS = 0,
  SMC_40CR_ERROR_INVALID_SYS_CLOCK_SPEED = -1,
  SMC_40CR_ERROR_INVALID_BUS_CLOCK_DIVIDER = -2,
  SMC_40CR_ERROR_RCC_UNLOCK_FAILED = -3,
  SMC_40CR_ERROR_DEFAULT_CLOCK_SWITCH_FAILED = -4,
  SMC_40CR_ERROR_PLL_CONFIGURATION_FAILED = -5,
  SMC_40CR_ERROR_PLL_STARTUP_FAILED = -6,
  SMC_40CR_ERROR_BUS_CLOCK_DIVIDER_SELECTION_FAILED = -7,
  SMC_40CR_ERROR_CLOCK_SWITCH_FAILED = -8
} SMC_40CR_ERROR;

typedef enum {
  SYS_CLOCK_SPEED_UNDEFINED = 0,
  SYS_CLOCK_SPEED_160M =      1,
  SYS_CLOCK_SPEED_80M =       2,
  SYS_CLOCK_SPEED_40M =       3,
  SYS_CLOCK_SPEED_20M =       4,
  SYS_CLOCK_SPEED_10M =       5,
  SYS_CLOCK_SPEED_5M =        6,
  SYS_CLOCK_SPEED_2_5M =      7, // 2.5M
  SYS_CLOCK_SPEED_1_25M =     8, // 1.25M
  SYS_CLOCK_SPEED_MAX_ENUM_VAL = SYS_CLOCK_SPEED_1_25M
} System_Clock_Speeds_t;

int32_t SetSystemAndBusClockConfig(System_Clock_Speeds_t sysClkSped, unsigned int BusClockDivider, bool isHsiClock);

// #ifdef TEST_STATIC_METHODS
// #define STATIC
// #else
// #define STATIC static
// #endif

int cleanup(SMC_40CR_ERROR errorCode);
bool validateSystemClockSpeed(System_Clock_Speeds_t sysClockSpeed);
bool validateBusClockDivider(unsigned int busClockDivider);
bool switchDefaultClock();
bool configurePLL(System_Clock_Speeds_t sysClockSpeed);
bool turnOnPLL();
void selectSystemClockDivider(System_Clock_Speeds_t sysClockSpeed);
void selectBusClockDivider(unsigned int busClockDivider);
bool switchClock(bool isHsiClock);
bool isDefaultClock();
bool checkRCCUnlocked();
bool unlockRCC();
void lockRCC();
void foo();

uint32_t incrementTimeoutCycles(uint32_t timeoutCycles);

#ifdef __cplusplus
}
#endif

#endif // STARTUP__H