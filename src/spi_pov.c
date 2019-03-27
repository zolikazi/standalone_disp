/**
  ******************************************************************************
  * @file    spi_pov.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   This file provide functions to retarget the C library printf function
  *          to the USART.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
//#include "serial_debug.h"
#include <stm32f4xx_spi.h>
#include "spi_pov.h"
#include "main.h"
#include <stdio.h>

/**
  * @brief  Initialize SPI1 interface for POV Display
  * @note   Other GPIOs handled: OE, LE
  * @param  None
  * @retval None
  */
void Pov_SPI_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	// enable clock for used IO pins
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* configure pins used by SPI2
   * PB14 = NSS
   * PB10 = SCK
	 * PC2 = MISO
	 * PC3 = MOSI
	 */

   //GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
 	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
 	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
 	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
 	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

   /*!< SPI SCK pin configuration */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
   GPIO_Init(GPIOB, &GPIO_InitStruct);

   /*!< SPI MISO pin configuration */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
   GPIO_Init(GPIOC, &GPIO_InitStruct);

   /*!< SPI MOSI pin configuration */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
   GPIO_Init(GPIOC, &GPIO_InitStruct);

   /* Connect alternate function */
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);

	/* Configure the chip select pin
	   in this case we will use PB14 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIOB->BSRRL |= GPIO_Pin_14; // set PB14 high


  /* Configure the OE pin
	   in this case we will use PA5 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_ResetBits(GPIOA, GPIO_Pin_5);    // set it



	// enable peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	/* configure SPI1 in Mode 0
	 * CPOL = 0 --> clock is low when idle
	 * CPHA = 0 --> data is sampled at the first edge
	 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
	SPI_Init(SPI2, &SPI_InitStruct);

  SPI_Cmd(SPI2, ENABLE); // enable SPI2}

}

/* This funtion is used to transmit and receive data
 * with SPI2
 * 			data --> data to be transmitted
 * 			returns received value
 */
uint8_t SPI2_send(uint8_t data){

	SPI2->DR = data; // write data to be transmitted to the SPI data register
	while( !(SPI2->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI2->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI2->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore

	return SPI2->DR; // return received data from SPI data register
}

uint8_t Pov_Send_Column(uint8_t data[12]){
  int i=0;
  GPIO_ResetBits(GPIOB, GPIO_Pin_14);

  for(i=0; i<12; i++){
    SPI2_send(data[i]);
  }

  GPIO_SetBits(GPIOB, GPIO_Pin_14);
  //Delay(1);
  GPIO_ResetBits(GPIOB, GPIO_Pin_14);
  return 0;
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
