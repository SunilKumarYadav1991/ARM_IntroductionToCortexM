Compiler: arm-none-eabi-gcc.exe
Absolute Path: C:\gcc-arm-none-eabi-10.3\bin\arm-none-eabi-gcc.exe

gcc manual: https://gcc.gnu.org/onlinedocs/gcc-9.3.0/gcc/

Sample compile command: 
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -I..\Inc  -c main.c


OpenOCD insallted in below location and used https://github.com/xpack-dev-tools/openocd-xpack/releases to download win64.zip
C:\xpack-openocd-0.11.0-5