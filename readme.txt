STM32 packetizer implementation

Cmake based GNU gcc version

Building:
cd build
cmake ..
or for debug version
cmake -DCMAKE_BUILD_TYPE=Debug ..

make

Programming via OpenOCD
sudo /usr/local/bin/openocd -f stm32f4discovery.g -f flash.cfg -c "flash_stm32f4 ../build/packetizer_stm32"

Debugging:
arm-none-eabi-gdb --tui --eval "target remote localhost:3333" packetizer_stm32

For SWD debugging console, uncomment the following lines in the CMakeLists.txt, and comment the previous ones.

##set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -mcpu=cortex-m4 -mlittle-endian -mthumb -Os -DSTM32F407xx --specs=rdimon.specs")
##SET(CMAKE_EXE_LINKER_FLAGS "-T../platform/STM32F407_FLASH.ld -lc -lrdimon")
