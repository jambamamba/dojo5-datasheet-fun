/**
 * @file    test_main.c
 * @brief   Main entry point that can be used for testing startup.c
 * @author  SMC
 * @date    September 2025
 *
 * This file can be used to test different inputs into startup.c. Could also be used to make rough unit tests too.
 *
 */
#include <stdio.h>
#include "startup.h"
#include "logger.h"

int32_t main(void)
{
  // int32_t retVal = SetSystemAndBusClockConfig(SYS_CLOCK_SPEED_10M, 1, false);
  // printf("Run complete! Return value is: %d\n", retVal);

  int32_t retVal = 0;
  int bus_divider_arr[] = {0, 2, 4};

  for(int sys_clock_speed = SYS_CLOCK_SPEED_UNDEFINED; sys_clock_speed <= SYS_CLOCK_SPEED_MAX_ENUM_VAL; sys_clock_speed++) {
    for(int idx = 0; idx < sizeof(bus_divider_arr)/sizeof(bus_divider_arr[0]); ++idx) {

      int bus_divider = bus_divider_arr[idx];
      bool internal_clock = true;

      retVal = SetSystemAndBusClockConfig(sys_clock_speed, bus_divider, internal_clock);
      LOG("Run complete! Return value is: %d\n============================\n", retVal);

      internal_clock = false;
      retVal = SetSystemAndBusClockConfig(sys_clock_speed, bus_divider, internal_clock);
      LOG("Run complete! Return value is: %d\n============================\n", retVal);
    }
  }

  return 0;
}