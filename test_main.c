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
  int32_t retVal = SetSystemAndBusClockConfig(SYS_CLOCK_SPEED_10M, 1, false);
  printf("Run complete! Return value is: %d\n", retVal);

  return 0;
}
