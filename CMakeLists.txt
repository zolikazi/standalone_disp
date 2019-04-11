project(standalone_disp C ASM)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_C_COMPILER   arm-none-eabi-gcc)
SET(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

add_definitions(-DSERIAL_DEBUG)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -mcpu=cortex-m4 -mlittle-endian -mthumb -Os -DSTM32F407xx --specs=nosys.specs")
SET(CMAKE_EXE_LINKER_FLAGS "-T../platform/STM32F407_FLASH.ld")

##set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -mcpu=cortex-m4 -mlittle-endian -mthumb -Os -DSTM32F407xx --specs=rdimon.specs")
##SET(CMAKE_EXE_LINKER_FLAGS "-T../platform/STM32F407_FLASH.ld -lc -lrdimon")

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

#set(STMCUBE_PATH "/home/eistmol/Documents/STM32/STM32Cube_FW_F4_V1.14.0")
#********************* Library *****************************
add_executable(${PROJECT_NAME}

src/main.c
src/spi_pov.c
src/serial_debug.c
src/tinyprintf.c
src/system_stm32f4xx.c
src/stm32f4xx_it.c
src/netconf.c
src/stm32f4x7_eth_bsp.c
src/packetizer.c

	#***          <GroupName>STM32F4 Discovery</GroupName>
 	STM32F4xx_Ethernet_Example/Utilities/STM32F4-Discovery/stm32f4_discovery_lcd.c
 	STM32F4xx_Ethernet_Example/Utilities/STM32F4-Discovery/stm32f4_discovery.c

 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/misc.c
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fsmc.c
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.c
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_sdio.c
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c
	
#***          <GroupName>STM32F4x7_ETH_Driver</GroupName>
 	STM32F4xx_Ethernet_Example/Libraries/STM32F4x7_ETH_Driver/src/stm32f4x7_eth.c
#***          <GroupName>LWIP</GroupName>
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/port/STM32F4x7/arch/ethernetif.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/api/api_lib.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/api/api_msg.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/api/err.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/api/netbuf.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/api/netdb.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/api/netifapi.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/api/sockets.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/api/tcpip.c
	STM32F4xx_Ethernet_Example/Libraries/lwip-1.4.1/src/core/def.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/dhcp.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/dns.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/init.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/mem.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/memp.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/netif.c
	STM32F4xx_Ethernet_Example/Libraries/lwip-1.4.1/src/core/packet.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/pbuf.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/raw.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/stats.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/sys.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/tcp.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/tcp_in.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/tcp_out.c
	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/timers.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/udp.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/ipv4/autoip.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/ipv4/icmp.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/ipv4/igmp.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/ipv4/inet.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/ipv4/inet_chksum.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/ipv4/ip.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/ipv4/ip_addr.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/core/ipv4/ip_frag.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/netif/etharp.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/netif/loopif.c
 	STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/netif/slipif.c

##	platform/system_stm32f4xx.c
	platform/startup_stm32f407xx.s
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC )
target_include_directories(${PROJECT_NAME} PUBLIC STM32F4xx_Ethernet_Example/Libraries/CMSIS/Include)
target_include_directories(${PROJECT_NAME} PUBLIC STM32F4xx_Ethernet_Example/Libraries/STM32F4x7_ETH_Driver/inc)
target_include_directories(${PROJECT_NAME} PUBLIC STM32F4xx_Ethernet_Example/Libraries/STM32F4xx_StdPeriph_Driver/inc)
target_include_directories(${PROJECT_NAME} PUBLIC STM32F4xx_Ethernet_Example/Utilities/STM32F4-Discovery)
target_include_directories(${PROJECT_NAME} PUBLIC STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/port/STM32F4x7/arch)
target_include_directories(${PROJECT_NAME} PUBLIC STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/port/STM32F4x7)
target_include_directories(${PROJECT_NAME} PUBLIC STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/include)
target_include_directories(${PROJECT_NAME} PUBLIC STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/include/ipv4)
target_include_directories(${PROJECT_NAME} PUBLIC STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/include/lwip)
target_include_directories(${PROJECT_NAME} PUBLIC STM32F4xx_Ethernet_Example/Utilities/lwip-1.4.1/src/include/netif)
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_include_directories(${PROJECT_NAME} PUBLIC platform)
