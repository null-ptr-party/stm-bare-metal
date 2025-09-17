# stm-bare-metal: Repository for STM32H723ZG
## Summary
Created by Landon Murray, this repo was created to get experience writing bare metal code to gain a better understanding of how the STM32H723ZG chip works at a low level. This code uses no libraries or existing drivers of any kind, including cmsis. Note that this code is inspired by the tutorial here: https://github.com/cpq/bare-metal-programming-guide , but targets the STM32H723XX MCU.
## Toolchain Setup
1. Install the msys2 platform available here: https://www.msys2.org/
2. Open a Msys UCRT64 terminal and install make ```pacman -S make```
3. Open a Msys UCRT64 terminal and install the arm eabi toolchain using the command: ```pacman -S mingw-w64-x86_64-arm-none-eabi-gcc```
4. Add mingw to path with the command ```export PATH=$PATH:/c/msys64/mingw64/bin ```
## Known Issues
PLL config struct cannot have zeros in multiplication/division factors: This issue is caused by the fact that the number written to the register for some arbitrary division/multiplication factor is one greater than the actual multiplication factor. This is frusterating to have to remember, so the code was modified so the user enters the desired multiplication/division factor, which was then reduced by one before writing to the register. The side effect of this is that configuration structs cannot have 0 for any of the multiplication/division factors as this will cause an underflow/rollover since the pll config numbers are unsigned.
