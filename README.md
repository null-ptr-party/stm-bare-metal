# stm-bare-metal: Repository for STM32H723ZG
## Summary
Created by Landon Murray, this repo was created to get experience writing bare metal code to gain a better understanding of how the STM32H723ZG chip works at a low level.
## Known Issues
PLL config struct cannot have zeros in multiplication/division factors: This issue is caused by the fact that the number written to the register for some arbitrary division/multiplication factor is one greater than the actual multiplication factor. This is frusterating to have to remember, so the code was modified so the user enters the desired multiplication/division factor, which was then reduced by one before writing to the register. The side effect of this is that configuration structs cannot have 0 for any of the multiplication/division factors as this will cause an underflow/rollover since the pll config numbers are unsigned.
