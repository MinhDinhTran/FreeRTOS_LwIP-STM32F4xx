<h3>LwIP work with RTOS in STM32F4xx</h3>
<p>This is a code framework that base on STM32F4xx MCU. It combines FreeRTOS with LwIP, to make it more convenient for users to develop ether-app on STM32F4 series MCU. This project supports redevelopment, and the matching Evaluation Board has relevant interfaces</p>
<p>这是一个基于STM32F4xx系列的代码框架，在FreeRTOS上运行LwIP，目的是为了之后的相关开发提供方便的平台。这个项目支持二次开发，并且与之匹配的EVB板上也留有相关的接口。</p>
<h3>Hardware</h3> 
<a href="http://www.developerlab.cn/">[EVB schematic sheet/开发板资料]</a><br>
<h4>introduce</h4>
<p>Code can run on both EVBs, but make some changes, because the pins have some differents. Fortunately, the only pin need to change in the code that is RESRT pin on MAC PHY of LAN8720, the others are same definition. It mind can be match same base bord.</p>

![STM32F405](https://github.com/laneston/Pictures/blob/master/STM32F405EVB.jpg)

This EVB use MCU of STM32F405.

![STM32F407-A](https://github.com/laneston/Pictures/blob/master/STM32F407EVB.jpg)

![STM32F407-B](https://github.com/laneston/Pictures/blob/master/STM32F407EVB-B.jpg)
