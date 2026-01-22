#!/bin/bash
set -xe

: <<'COMMENT_BLOCK'
2 clocks:
  def: 8mhz
  hsi: 40mhz
    can be divided or multiplied by pll, and divided further by using the sys_div and bus_div in rcc_cr
    invalid setting will reset hsi to def clock

2 dividers:
  sys - 8mhz with def clock
  bus - 8mhz with def clock
  

at boot:
if the clock source is the HSI, then the starting frequency of the clock is 40MHz

That 40MHz clock gets fed into the PLL, 
and then output of the PLL is gets divided down by SYS_DIV to get the final value of the system clock. 

To get the final value of the bus clock, the system clock value is divided down further by BUS_DIV.


 
external crystal if used is required to be 40Mhz
it can be divided to PLL

algo:

Unlock the RCC registers
Configure the RCC_PLLCFGR register to have the desired clock divider.
Turn on the PLL (set RCC_CR_PLLON = 1)
Wait for PLL_RDY to be 1. (Note: See registers below for information on how long to wait for PLL_RDY and others)
Set BUS_DIV or SYS_DIV if needed to divide the bus and/or system clocks
If using an external clock:
Set HSEON to 1
Wait for HSERDY to be 1.
Set DEF_CLOCK to 0
Wait for CLKSEL to be HSE.
If using an internal clock:
Set HSION to 1
Wait for HSIRDY to be 1.
Set DEF_CLOCK to 0
Wait for CLKSEL to be HSI.
Optionally lock the RCC registers again by writing 1 to RCC_LOCK

..............

switchClock(clock_type:external|internal):  
  unlockRCC()
  clock_divider = readCLKSEL()
  if setBusOrSysDivider(clock_divider):
    turnOnPLL()
    setHighSpeedClock(clock_type)
    resetDEF_CLOCK(clock_divider:HSE|HSI):

readCLKSEL():
  if CLKSEL == 00:
  	return DEF_CLOCK
  else if CLKSEL == 01:
  	return HSI
  else if CLKSEL == 10
  	return HSE
  else
     error
  
bool setBusOrSysDivider(clock_divider):
  if clock_divider == HSI or clock_divider == HSE:
    clk_speed = 40

    if RCC_PLLCFGR_MUL == 000:
       clk_speed *= 1
    else if RCC_PLLCFGR_MUL == 001:
       clk_speed *= 2
    else if RCC_PLLCFGR_MUL == 010:
       clk_speed *= 4
    else 
    	clk_speed *= 1

    if RCC_PLLCFGR_DIV == 000:
       clk_speed /= 1
    else if RCC_PLLCFGR_DIV == 001:
       clk_speed /= 2
    else if RCC_PLLCFGR_DIV == 010:
       clk_speed /= 4
    else if RCC_PLLCFGR_DIV == 100:
       clk_speed /= 8
    else 
    	clk_speed /= 1

    if SYS_DIV == 00:
       pass
    else if SYS_DIV == 01
       clk_speed /= 2
    else if SYS_DIV == 10
       clk_speed /= 4
    else if SYS_DIV == 11
       clk_speed /= 4
       
    sys_clk = clk_speed

    if BUS_DIV == 00:
       pass
    else if BUS_DIV == 01
       clk_speed /= 2
    else if BUS_DIV == 10
       clk_speed /= 4
    else if BUS_DIV == 11
       clk_speed /= 4

    bus_clk = clk_speed

  else if clock_divider == DEF_CLOCK:
    bus_clk = sys_clk = 8
  else
    error

  if bus_clk > 20:
    error
    

unlockRCC():
  RCC_UNL = 0x56DD 
  RCC_UNH = 0xA3B2
  
  
lockRCC():
   RCC_LOCK_LOCK = 1
   
setDEF_CLOCK():
  unlockRCC()
  RCC_DEF_CLOCK = 1
  while true:
    if CLKSEL == DEF_CLOCK (000)
       break

resetDEF_CLOCK(clock_divider:HSE|HSI):
  unlockRCC()
  RCC_DEF_CLOCK = 0
  while true:
    if CLKSEL == clock_divider
       break

turnOnPLL():
  RCC_CR_PLLON = 1
  cycles = 0
  while true:
    if PLL_RDY == 1
       break
    cycles++
    if cycles > 350:
      error

setHighSpeedClock(clock_type:external|internal):
  if clock_type == external:
    HSEON = 1
    cycles = 0
    while true:
      if HSERDY == 1
         break
      cycles++
      if cycles > 4200:
        error
        
  if clock_type == internal:
    HSION = 1
    cycles = 0
    while true:
      if HSIRDY == 1
         break
      cycles++
      if cycles > 1400:
        error
  

  gcc -DMOCK_REGISTERS=1 -O3 -g startup.c test_main.c 
  ./a.out
COMMENT_BLOCK

mkdir -p .build
pushd .build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j
# gdb \
# ./smc40cr
./smc40cr_test
popd