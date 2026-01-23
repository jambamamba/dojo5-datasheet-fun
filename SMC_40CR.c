#include <stdint.h>
#include <stdbool.h>
#include "SMC_40CR.h"
#include "startup.h"
#include "logger.h"

static const uint32_t CLKSEL_SWITCH_MAX_TIME_IN_CYCLES =     500UL;
static const uint32_t HSIRDY_MAX_TIME_IN_CYCLES =            1400UL;
static const uint32_t HSERDY_MAX_TIME_IN_CYCLES =            4200UL;
static const uint32_t PLL_READY_TIME_OUT_CYCLES =            350UL;

bool validateSystemClockSpeed(System_Clock_Speeds_t sysClockSpeed) {

  return ((sysClockSpeed > SYS_CLOCK_SPEED_UNDEFINED) && (sysClockSpeed <= SYS_CLOCK_SPEED_MAX_ENUM_VAL));
}
void selectBusClockDivider(unsigned int busClockDivider) {

  switch (busClockDivider) {
    case 2: // Divide by 2
      RCC_CR |= RCC_CR_BUS_DIV_0;
      return ;

    case 4: // Divide by 4
      RCC_CR |= RCC_CR_BUS_DIV_1;
      return ;

    default:
      // No divider needed
      return ;
  }
}

bool isDefaultClock() {
  volatile uint32_t rccCrReg = RCC_CR;
  return ((rccCrReg & RCC_CR_CLKSEL) == 0);
}

bool switchDefaultClock() {
  uint32_t clockselTimeout = 0U;
  RCC_CR |= RCC_CR_DEF_CLOCK;
  do {
    volatile uint32_t rccCrReg = RCC_CR;
    clockselTimeout = incrementTimeoutCycles(clockselTimeout);
    // LOG("clockselTimeout:%i, timeoutCycles:%i", clockselTimeout, timeoutCycles);
  } while (!isDefaultClock() && (clockselTimeout < CLKSEL_SWITCH_MAX_TIME_IN_CYCLES));
  return isDefaultClock();
}

static void resetDividers() {
  RCC->CR = (RCC->CR & ~RCC_CR_BUS_DIV);
  RCC->CR = (RCC->CR & ~RCC_CR_SYS_DIV);
  RCC_PLLCFGR = 0;
}
RCC_TypeDef *getRCC() {
  return RCC;
}
bool configurePLL(System_Clock_Speeds_t sysClockSpeed) {
  resetDividers();
  switch (sysClockSpeed)
  {
    case SYS_CLOCK_SPEED_160M:
      // Set a PLL Multiplication factor of 4 to get 160M (0b010 for MUL bits)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_MUL) | RCC_PLLCFGR_MUL_1;
      return true;

    case SYS_CLOCK_SPEED_80M: 
      // Set a PLL Multiplication factor of 2 to get 80M (0b001)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_MUL) | RCC_PLLCFGR_MUL_0;
      return true;

    case SYS_CLOCK_SPEED_40M: 
      // Set a PLL Multiplication factor of 1. Nothing to do!
      return true;

    case SYS_CLOCK_SPEED_20M:
      // Set a PLL Division factor of 2 (0b001)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_DIV) | RCC_PLLCFGR_DIV_0;
      return true;

    case SYS_CLOCK_SPEED_10M: 
      // Set a PLL Division factor of 4 (0b010)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_DIV) | RCC_PLLCFGR_DIV_1;
      return true;

    case SYS_CLOCK_SPEED_5M: 
      // Set a PLL Division factor of 8 (0b100)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_DIV) | RCC_PLLCFGR_DIV_2;
      return true;

    case SYS_CLOCK_SPEED_2_5M:      
      // Set a PLL Division factor of 8 and SYS_DIV value of 2 (0b01)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_DIV) | RCC_PLLCFGR_DIV_2;
      // RCC_CR |= RCC_CR_SYS_DIV_0; //osm:bug can only set SYS_DIV after PLL_RDY, see step 4,5 in doc
      return true;

    case SYS_CLOCK_SPEED_1_25M:  
      // Set a PLL Division factor of 8 and SYS_DIV value of 4 (0b10)
      RCC_PLLCFGR = (RCC_PLLCFGR & ~RCC_PLLCFGR_DIV) | RCC_PLLCFGR_DIV_2;
      // RCC_CR |= RCC_CR_SYS_DIV_1; //osm:bug can only set SYS_DIV after PLL_RDY, see step 4,5 in doc
      return true;

    case SYS_CLOCK_SPEED_UNDEFINED:   
    default:
      LOG("Cannot configure PLL because of invalid sysClockSpeed value: %d", sysClockSpeed);
      return false;
  }
}

void selectSystemClockDivider(System_Clock_Speeds_t sysClockSpeed) {
  switch (sysClockSpeed) {

    case SYS_CLOCK_SPEED_2_5M:      
      RCC_CR |= RCC_CR_SYS_DIV_0; // Set SYS_DIV to 2 (0b01)
      break;

    case SYS_CLOCK_SPEED_1_25M:  
      RCC_CR |= RCC_CR_SYS_DIV_1; // Set SYS_DIV to 4 (0b10)
      break;

    default:
      // No divider needed
      break;
  }
}

bool checkRCCUnlocked() {
  volatile uint32_t rccLockReg = RCC_LOCK;
  return ((rccLockReg & RCC_LOCK_LOCK_STATUS) == 0U);
}

bool unlockRCC() {
  RCC_UNL = 0x56DD;
  RCC_UNH = 0xA3B2;
  if(!checkRCCUnlocked()) {
    LOG("RCC registers not unlocked");
    return false;
  }
  return true;
}
void lockRCC() {
  RCC_LOCK |= RCC_LOCK_LOCK;
}
bool turnOnPLL() {
  RCC_CR |= RCC_CR_PLLON;
  uint32_t timeoutCycles = 0U;
  bool isPllReady = false;
  do {
    volatile uint32_t rccCrReg = RCC_CR;
    isPllReady = ((rccCrReg & RCC_CR_PLL_RDY) != 0);
    timeoutCycles = incrementTimeoutCycles(timeoutCycles);
    // LOG("timeoutCycles:%i, rccCrReg:0x%x, RCC_CR_PLL_RDY:0x%x", timeoutCycles, rccCrReg, RCC_CR_PLL_RDY);
  } while (!isPllReady && (timeoutCycles < PLL_READY_TIME_OUT_CYCLES));
  return isPllReady;
}

bool setInternalClock() {
    bool isClkReady = false;
    // Internal clock!
    // Set HSION to 1 and wait for HSIRDY to be 1
    // Then set DEF_CLOCK to 0 and wait for CLKSEL to be HSI
    // RCC_CR |= RCC_CR_HSEON; //osm:bug We are doing internal clock here.
    RCC_CR |= RCC_CR_HSION; //fix
    uint32_t timeoutCycles = 0U;
    do {
      volatile uint32_t rccCrReg = RCC_CR;
      isClkReady = ((rccCrReg & RCC_CR_HSIRDY) != 0);
      timeoutCycles = incrementTimeoutCycles(timeoutCycles);
      // LOG("timeoutCycles:%i", timeoutCycles);
    } while (!isClkReady && (timeoutCycles < HSIRDY_MAX_TIME_IN_CYCLES));
    return isClkReady;
}

bool setExternalClock() {
    bool isClkReady = false;
    // External clock!
    // Set HSEON to 1 and wait for HSERDY to be 1
    RCC_CR |= RCC_CR_HSEON;
    uint32_t timeoutCycles = 0U;
    do {
      volatile uint32_t rccCrReg = RCC_CR;
      isClkReady = ((rccCrReg & RCC_CR_HSERDY) != 0);
      timeoutCycles = incrementTimeoutCycles(timeoutCycles);
      // LOG("timeoutCycles:%i", timeoutCycles);
    } while (!isClkReady && (timeoutCycles < HSERDY_MAX_TIME_IN_CYCLES));
    return isClkReady;
}

bool waitForClockSwitch(uint32_t expectedClockSel) {
  bool isClkReady = false;
  uint32_t clockselTimeout = 0U;
  do {
    volatile uint32_t rccCrReg = RCC_CR;
    // isClkReady = ((rccCrReg & RCC_CR_CLKSEL) != 0);//osm:bug - should check explicitly for RCC_CR_CLKSEL_0 or RCC_CR_CLKSEL_1, because this test will pass for (RCC_CR_CLKSEL_0|RCC_CR_CLKSEL_1) which would be wrong
    isClkReady = ((rccCrReg & RCC_CR_CLKSEL) == expectedClockSel);
    clockselTimeout = incrementTimeoutCycles(clockselTimeout);
    // LOG("clockselTimeout:%i, timeoutCycles:%i", clockselTimeout, timeoutCycles);
  } while (!isClkReady && (clockselTimeout < CLKSEL_SWITCH_MAX_TIME_IN_CYCLES));
  return isClkReady;
}

void unsetDefaultClock() {
  RCC_CR = (RCC_CR & ~RCC_CR_DEF_CLOCK);
}

bool switchClock(bool isHsiClock) {
  LOG("Set clock based on isHsiClock:%i", isHsiClock);
  if (isHsiClock) {
    if(!setInternalClock()) {
      LOG("Failed to set internal clock");
      return false;
    }
  }
  else {
    if(!setExternalClock()) {
      LOG("Failed to set external clock");
      return false;
    }
  }
  unsetDefaultClock();
  if(!waitForClockSwitch(isHsiClock ? RCC_CR_CLKSEL_0 : RCC_CR_CLKSEL_1)) {
    LOG("Timeout waiting for clock switch");
    return false;
  }
  return true;
}
bool validateBusClockDivider(unsigned int busClockDivider) {
  return ((busClockDivider == 2) || (busClockDivider == 4));
}

int cleanup(SMC_40CR_ERROR errorCode) {
    RCC_CR |= RCC_CR_DEF_CLOCK; //switch to default clock, don't wait for it here
    lockRCC();
    return errorCode;
}