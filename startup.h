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

#include <stdint.h>
#include <stdbool.h>

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

extern uint32_t g_PllReadyTimeoutCycles;


#endif // STARTUP__H