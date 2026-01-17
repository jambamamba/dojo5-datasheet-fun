### Introduction
The Superspeed Microcontroller Company (SMC) has asked you to review their code for a startup file they've written to make it easy to configure the System and Bus clocks on boot. Their goal is to have a function users can call to setup the initial clock configuration without having to dive too deep into the reference manual.
This code is for a new 32-bit microcontroller they're developing called the "SMC_40". 
<br>
<br>

### Problem
SMC is a hardware first company and their experience with C is limited. They've taken a first stab at it, but have become stuck. The code doesn't quite work correctly so they'd like you to:<br>
a) Fix it so that it works and configures the clocks as desired. 
b) Clean up the code so it uses best practices and is easy to read and understand.
c) Add any improvements (if any) as you see fit.
<br>

SMC's specific issues they are having trouble fixing are:
1) Infinite loop sometimes when switching clocks
2) CLKSEL sometimes stays on DEF_CLOCK at the end of the SetSystemAndBusClockConfig() function
3) When choosing the HSI clock, the CLKSEL flag doesn't show HSI

<br>
There are likely more errors than those above and there are a good number of readability, style, and best practices that can be improved upon in the code. See how many you can find and fix!
<br>
<br>


### More Information
Your job as mentioned above is to clean-up startup.c and startup.h so they work properly and are ready to release to customers. Please assume that the SMC_40CR_ReferenceManual and the SMC_40CR.h files are 100% correct. They should ***NOT*** be modified. 

startup.c contains a single function that at the moment is not always configuring the RCC quite correctly. You'll have to take a look through it and compare it with the reference manual and SMC_40CR header file to figure out where the errors are. 
***SetSystemAndBusClockConfig()***:
At the end of this function the RCC should be configured so that its registers are correct based on all possible system clock, bus clock, and external/internal clock select inputs. It will return 0 on success and -1 if a failure occurs. (Hint: This could be improved right off the bat to give more error information about failures).
<br>

#### Running the Program (Optional)
***The following is not required to do, however, if you find it useful in this code cleanup/review excercise, feel free to use. If you don't build and run the program as laid out below, then please ignore test_main.c - it is not required to modify for this excercise.***
To facilitate developing, you can compile with Clang or GCC and use test_main.c to develop your own tests. This is not required, but if you find running it or adding unit tests is helpful, feel free to do so! Keep in mind that because of specific hardware timing and waiting for clock registers that running on your PC will not necessarily produce the correct output. (i.e. making this program work so that SetSystemAndBusClockConfig() always returns 0 on a PC will not be the correct answer). Be careful when writing mocking tests as not mocking the clock registers correctly can result in an infinite loop. 

Inside of startup.h there are mock functions that can replace the RCC register addresses by defining MOCK_REGISTERS = 1. To build with Clang with mocking turned on, you can do:
"clang -DMOCK_REGISTERS=1 startup.c test_main.c -o smc.out"