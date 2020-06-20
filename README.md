<h3>LwIP work with RTOS in STM32F4xx</h3>
<p>This is a code framework that base on STM32F4xx MCU. It combines FreeRTOS with LwIP, to make it more convenient for users to develop ether-app on STM32F4 series MCU. This project supports redevelopment, and the matching Evaluation Board has relevant interfaces</p>
<p>这是一个基于STM32F4xx系列的代码框架，在FreeRTOS上运行LwIP，目的是为了之后的相关开发提供方便的平台。这个项目支持二次开发，并且与之匹配的EVB板上也留有相关的接口。</p>
<h3>Hardware</h3> 
<a href="http://www.developerlab.cn/">[EVB schematic sheet/开发板资料]</a><br>
<h4>introduce</h4>
<p>Code can run on both EVBs, but make some changes, because the pins have some differents. Fortunately, the only pin need to change in the code that is RESRT pin on MAC PHY of LAN8720, the others are same definition. It mind can be match same base bord.</p>
<h4>STM32F405RG-EVB</h4>

![STM32F405](https://github.com/laneston/Pictures/blob/master/STM32F405EVB.jpg)

<p>This EVB use MCU of STM32F405RGT6. It has 1 Mbyte of Flash memory, 192+4 Kbyte of SRAM. In addition to the network port of RMII, EVB also left other IO ports, including UART, SPI, IIC.

</p>

<h4>STM32F407ZG-EVB</h4>

![STM32F407-A](https://github.com/laneston/Pictures/blob/master/STM32F407EVB.jpg)

![STM32F407-B](https://github.com/laneston/Pictures/blob/master/STM32F407EVB-B.jpg)

<P>This EVB use MCU of STM32F407ZGT6.  It has 1 Mbyte of Flash memory, 192+4 Kbyte of SRAM. In addition to the network port of RMII, EVB also left other IO ports, including UART, SPI, IIC.</P>
<p>In addition to the above interface, the EVB add FSMC for NAND Flash and SRAM. It can be run larger content or complex algorithm. I designed it mainly for building FTP Server later.</p>

<h4>代码框架|code tree</h4>
<PRE>
STM32F405/407
   |__CMSIS
   |  |__system_stm32f4xx.c [STM32内核配置文件]
   |  |__startup_stm32f40_41xxx.c [STM32F4 启动文件]
   |
   |__STM32F4xx_StdPeriph_Dirver
   |
   |__STM32F4x7_ETH_Driver
   |  |__stm32f4x7_eth.c
   |
   |__FreeRTOS
   |
   |__Lwip_core
   |
   |__Lwip_api
   |
   |__Lwip_netif
   |
   |__Lwip_app
   |
   |__Lwip_port
   |
   |__User
   |  |__main.c
   |  |__stm32f4xx_it.c
   |  |__main.h
   |  |__FreeRTOSConfig.h
   |  |__delay.c
   |
   |__RTOS_Task
   |  |__main_task.c
   |  |__tcp_task.c
   |  |__uaet_task.c
   |  |__segment_task.c
   |  |__dhcp_task.c
   |
   |__Peripheral_BSP
      |__stm32f4xx_eth_bsp.c
      |__stm32f4xx_uart_bsp.c
      |__stm32f4xx_heap_bsp.c
      |__stm32f4xx_fsmc_bsp.c
      |__stm32f4xx_dht22_bsp.c

</PRE>
