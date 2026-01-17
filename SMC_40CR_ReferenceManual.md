## Section 4: Reset and Clock Control (RCC)

The RCC is responsible for configuring the clock speed. There are two internal clocks - an HSI (High Speed Internal) and a DEF\_CLOCK (Default Clock). When the SMC\_40 boots up it will always default to using DEF\_CLOCK. This clock is a special internal clock fixed at 8Mhz. It cannot be divided down or multiplied up by the PLL and is fixed at 8MHz regardless of any other configuration. (That also means the System and Bus dividers do not affect this clock either. When DEF\_CLOCK is selected, the system and bus clocks are both fixed at 8MHz).<br>
The HSI clock on the other hand is an internal 40MHz clock which can be divided or multiplied by the PLL. When the HSI clock is selected, the system and bus clocks can be divided down further by using the SYS\_DIV and BUS\_DIV fields in RCC\_CR.<br>
Note that any invalid configuration that is set in RCC will cause a fallback to DEF\_CLOCK.

### Configuration:

An external crystal may be used, but is required to be 40Mhz. From there it can be divided down using the PLL.

The initial boot of the SMC\_40 will have DEF\_CLOCK = 1. To switch clocks you must:

1) Unlock the RCC registers  
2) Configure the RCC\_PLLCFGR register to have the desired clock divider.   
3) Turn on the PLL (set RCC\_CR\_PLLON = 1)  
4) Wait for PLL\_RDY to be 1. (Note: See registers below for information on how long to wait for PLL\_RDY and others)
5) Set BUS_DIV or SYS_DIV if needed to divide the bus and/or system clocks
6) If using an external clock:  
   1) Set HSEON to 1  
   2) Wait for HSERDY to be 1.
   3) Set DEF\_CLOCK to 0  
   4) Wait for CLKSEL to be HSE. 
7) If using an internal clock:  
   1) Set HSION to 1  
   2) Wait for HSIRDY to be 1.
   3) Set DEF\_CLOCK to 0  
   4) Wait for CLKSEL to be HSI.
8) Optionally lock the RCC registers again by writing 1 to RCC\_LOCK

The RCC\_CR register is used to configure the clock. This register is locked and protected from writes on reset. To unlock it, you must write ***0x56DD*** to RCC\_UNL and ***0xA3B2*** to RCC\_UNH (in that specific order) prior to making any changes. Writing a 1 to RCC\_LOCK\_LOCK \-or- writing any incorrect values to RCC\_UNH/UNL will re-lock the registers again.
<br>
<br>
**System and Bus Clocks:**<br>
The System and Bus clocks take their source from whatever is shown by CLKSEL. For example, if the clock source is the HSI, then the starting frequency of the clock is 40MHz. That 40MHz clock gets fed into the PLL, and then whatever the output of the PLL is gets divided down by SYS\_DIV to get the final value of the system clock. To get the final value of the bus clock, the system clock value is divided down further by BUS\_DIV.<br>
As stated above, the PLL, SYS\_DIV and BUS\_DIV dividers are only relevant when CLKSEL is HSI or HSE. IF CLKSEL is set to DEF\_CLOCK, then the PLL, SYS\_DIV, and BUS\_DIV are all ignored, and the resulting system clock and bus clock are both 8MHz. 

Further Clarification: If HSI or HSE is shown in CLKSEL, then the RCC\_PLLCFGR\_MUL and RCC\_PLLCFGR\_DIV both apply to the base 40MHz clock prior to going into the SYS\_DIV and BUS\_DIV. For example, in this configuration the base clock is 40MHz, and if the PLL has no multiplier and a divider of "2" then the resulting clock that gets divided by SYS\_DIV is 20MHz. If the SYS\_DIV is also 2 then the resulting System Clock is 10MHz. This 10MHz clock is then fed into BUS\_DIV and the result from that division is the Bus Clock frequency.
<br>
<br>
**Switching Clocks from HSE or HSI:**<br>
If the RCC register has been configured to use the HSE or HSI clock, the SMC_40 has the ability to switch back to the DEF\_CLOCK. To do this you must:
1) Unlock the RCC registers
2) Set DEF\_CLOCK in RCC_CR to 1. This will automatically set PLLON, HSERDY, HSEON, HSIRDY, and HSION to 0.
3) Wait until CLKSEL shows DEF\_CLOCK (0b00). This switch will take at max 500 CPU cycles.
<br>
<br>

### Registers
| RCC\_CR | Address: 0x400023800 |  |  |  |
| :---- | :---- | :---- | :---- | :---- |
| **Field** | **Bits** | **R/W** | **Reset Value** | **Description** |
| DEF\_CLOCK | 31 | RW | 1 | Default 8Mhz clock. Set to 0 only after a new clock has been configured |
| RESERVED | 12-30 | R | All 0 | Reserved |
| PLL\_RDY | 11 | R | 0 | PLL Ready flag. Will become 1 when the PLLON bit is set. Max time taken to be ready is 350 CPU cycles after PLLON is set to 1.
| SYS\_DIV | 9-10 | RW | 01 | System Clock Divider. Divides whichever clock is specified by CLKSEL. The output of this is the final system clock speed which is CLKSEL / SYS_DIV. <br>00 = No division <br>01 = Divided by 2 <br>10 = Divided by 4 <br>11 = Divided by 4 (Same as 0b10)
| BUS\_DIV | 7-8 | RW | 10 | Bus Clock Divider. The bus clock is clocked from whichever clock is selected by CLKSEL. The final value of bus clock is: CLKSEL / SYS_DIV / BUS_DIV. The bus clock MUST be 20MHz or less. <br>00 = No division <br>01 = Divided by 2 <br>10 = Divided by 4 <br>11 = Divided by 4 (Same as 0b10)
| PLLON | 6 | RW | 0 | Turns the PLL On/Off <br>1 = On <br>0 = Off |
| CLKSEL | 4-5 | R | 00 | Displays the currently selected clock. Max time taken to switch clocks is 500 CPU cycles.<br>00 = DEF\_CLOCK <br>01 = HSI <br>10 = HSE <br>11 = Reserved |
| HSERDY | 3 | R | 0 | External High Speed Clock Ready Flag. Max time taken to be ready is 4200 CPU cycles after HSEON is set to 1.<br>1 = Ready <br>0 = Not Ready |
| HSEON | 2 | RW | 0 | External High Speed Clock Enable. <br>1 = Enabled <br>0 = Disabled |
| HSIRDY | 1 | R | 0 | Internal High Speed Clock Ready Flag. Max time taken to be ready is 1400 CPU cycles after HSION is set to 1. <br>1 = Ready <br>0 = Not Ready |
| HSION | 0 | RW | 0 | Internal High Speed Clock Enable. <br>1 = Enabled <br>0 = Disabled |
<br>

| RCC\_PLLCFGR | Address: 0x400023804 |  |  |  |
| :---- | :---- | :---- | :---- | :---- |
| The "PLL Clock" is either the HSI <br>or the HSE, both of which are 40MHz. |
| **Field** | **Bits** | **R/W** | **Reset Value** | **Description** |
| RESERVED | 6-31 | R | All 0 | Reserved |
| MUL | 3-5 | RW | 0 | PLL Clock Multiplier. <br>000 = PLL Clock Multiplied by 1 <br>001 = PLL Multiplied by 2 <br>010 = PLL Multiplied by 4 <br>Other Values = PLL Multiplied by 1 |
| DIV | 0-2 | RW | 0 | PLL Clock Divider. <br>000 = PLL Clock Divided by 1 <br>001 = PLL Divided by 2 <br>010 = PLL Divided by 4 <br>100 = PLL Divided by 8 <br>Other Values = PLL Divided by 1 |
<br>

| RCC\_UNL | Address: 0x400023808 |  |  |  |
| :---- | :---- | :---- | :---- | :---- |
| **Field** | **Bits** | **R/W** | **Reset Value** | **Description** |
| UNLOCK | 0-15 | W | All 0 | Must write **0x56DD** followed by correct write to RCC\_UNH to unlock RCC registers. |
<br>

| RCC\_UNH | Address: 0x40002380A |  |  |  |
| :---- | :---- | :---- | :---- | :---- |
| **Field** | **Bits** | **R/W** | **Reset Value** | **Description** |
| UNLOCK | 0-15 | W | All 0 | Must write **0xA3B2** after a write to RCC\_UNL to unlock RCC registers. |
<br>

| RCC\_LOCK | Address: 0x40002380C |  |  |  |
| :---- | :---- | :---- | :---- | :---- |
| **Field** | **Bits** | **R/W** | **Reset Value** | **Description** |
| RESERVED | 2-31 | R | All 0 | Reserved |
| LOCK\_STATUS | 1 | R | 1 | Lock status of the RCC registers.  <br>1 = Locked <br>0 = Unlocked |
| LOCK | 0 | W | 0 | Writing a 1 to this bit will lock all the RCC registers. Reading this will always read 0 |

## 
