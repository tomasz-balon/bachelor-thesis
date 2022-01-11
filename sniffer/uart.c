#include "uart.h"
//#include "pio/samd21e17a.h"
//#include "instance/port.h"
#include "system_interrupt.h"
#include "../../../../../Programy_na_uczelnie/Atmel Toolchain/ARM GCC/Native/4.8.1443/CMSIS_Atmel/Device/ATMEL/samd21/include/samd21e17a.h"
#include "../../../../../Programy_na_uczelnie/Atmel Toolchain/ARM GCC/Native/4.8.1443/CMSIS_Atmel/Device/ATMEL/samd21/include/component/sercom.h"
#include "../../../../../Programy_na_uczelnie/Atmel Toolchain/ARM GCC/Native/4.8.1443/CMSIS_Atmel/Device/ATMEL/samd21/include/instance/sercom1.h"
//#include "component/port.h"

void UART1_init(uint32_t baud)
{
	// Set the PMUX for SERCOM1 and turn module on
	REG_PM_APBCMASK |= PM_APBCMASK_SERCOM1;
	
	// Select correct USART clock (SERCOM3_GCLK_ID_CORE), enable the clock, set GCLK GEN(0)
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM1_GCLK_ID_CORE) | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);
	
	// SET PIN 00 as TX (Read from FT230) (input)
	PORT->Group[0].DIRCLR.reg = PORT_PA00;							// Set PIN direction to input
	//PORT->Group[0].PINCFG[0].reg |= PORT_PINCFG_INEN;				//
	PORT->Group[0].PINCFG[0].reg &= ~PORT_PINCFG_PULLEN;			// Enable pull down resistor
	PORT->Group[0].PINCFG[0].reg |= PORT_PINCFG_PMUXEN;				// Enable PMUX
	PORT->Group[0].PMUX[0>>1].bit.PMUXE = PORT_PMUX_PMUXE_D_Val;	// Enable PMUX and set PMUX bit
	
	// Set Pin 01 as RX (Transmit to FT230) (output)
	PORT->Group[0].DIRSET.reg = PORT_PA01;							// Set PIN direction to output
	//PORT->Group[0].PINCFG[1].reg |= PORT_PINCFG_INEN;				//
	PORT->Group[0].PINCFG[1].reg |= PORT_PINCFG_PMUXEN;				// Enable PMUX
	PORT->Group[0].PMUX[1>>1].bit.PMUXO = PORT_PMUX_PMUXO_D_Val;	// Enable PMUX and set PMUX bit
	
	// Configure USART Control A: Asynchronous, transmit LSB first, set internal clock, use pad 1 for receiving data, use pad 0 for transmitting data
	SERCOM1->USART.CTRLA.reg = SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE_USART_INT_CLK | SERCOM_USART_CTRLA_RXPO(0) | SERCOM_USART_CTRLA_TXPO(1);
	
	// Configure USART Control B: not using parity, enable receive when USART enabled, enable transmit when USART enabled, set character size to 8 bits
	SERCOM1->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_CHSIZE(0);
	
	// Set Baud Rate
	uint64_t baud_rate = (uint64_t)65536 * (F_CPU - 16 * baud) / F_CPU;
	SERCOM1->USART.BAUD.reg = (uint32_t)baud_rate;
	
	// Enable USART
	SERCOM1->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
}

void UART3_init(uint32_t baud)
{
	// Set PIN 24 as RX (Receive from LIN) (input)
	PORT->Group[0].DIRCLR.reg = PORT_PA24;							// Set PIN direction to input
	PORT->Group[0].PINCFG[24].reg |= PORT_PINCFG_INEN;				//
	PORT->Group[0].PINCFG[24].reg &= ~PORT_PINCFG_PULLEN;			// Enable pull down resistor
	PORT->Group[0].PINCFG[24].reg |= PORT_PINCFG_PMUXEN;			// Enable PMUX
	PORT->Group[0].PMUX[24>>1].bit.PMUXE = PORT_PMUX_PMUXE_C_Val;	// Enable PMUX and set PMUX bit
	
	// Set PIN 25 as TX (Transmit to LIN) (output)
	PORT->Group[0].DIRSET.reg = PORT_PA25;							// Set PIN direction to output
	PORT->Group[0].PINCFG[25].reg |= PORT_PINCFG_INEN;				// 
	PORT->Group[0].PINCFG[25].reg |= PORT_PINCFG_PMUXEN;			// Enable PMUX
	PORT->Group[0].PMUX[25>>1].bit.PMUXO = PORT_PMUX_PMUXE_C_Val;	// Enable PMUX and set PMUX bit
	
	// Set the PMUX for SERCOM3 and turn module on 
	REG_PM_APBCMASK |= PM_APBCMASK_SERCOM3;
	
	// Select correct USART clock (SERCOM3_GCLK_ID_CORE), enable the clock, set GCLK GEN(0)
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM3_GCLK_ID_CORE) | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);
	
	// Configure USART Control A: Asynchronous, transmit LSB first, set internal clock, use pad 1 for receiving data, use pad 0 for transmitting data
	SERCOM3->USART.CTRLA.reg = SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE_USART_INT_CLK | SERCOM_USART_CTRLA_RXPO(1) | SERCOM_USART_CTRLA_TXPO(0);
	
	// Configure USART Control B: not using parity, enable receive when USART enabled, enable transmit when USART enabled, set character size to 8 bits
	SERCOM3->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_CHSIZE(0);
	
	// Set Baud Rate
	uint64_t baud_rate = (uint64_t)65536 * (F_CPU - 16 * baud) / F_CPU;
	SERCOM3->USART.BAUD.reg = (uint32_t)baud_rate;
	
	// Enable USART
	SERCOM3->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
}

/*
 * Function to check whether there is incoming data
 */
bool UART1_data_incoming(void)
{
	if ((SERCOM1->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC) >> SERCOM_USART_INTFLAG_RXC_Pos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * Function to write single character to the UART
 */
void UART1_write_char(char character)
{
	while(!(REG_SERCOM1_USART_INTFLAG /*& SERCOM1->USART.INTFLAG.reg*/) & 1) // wait for interrupt flag
	{}
	
	SERCOM1->USART.DATA.reg = character;
	//REG_SERCOM1_USART_DATA = character;
}

/*
 * Function to write whole string to the UART
 */
void UART1_write_str(char *str)
{
	for (int iterator=0; str[iterator]!='\0'; iterator++)
	{
		UART1_write_char(str[iterator]);
	}
}

/*
 * Read data from UART and store it in USART data register
 */
char UART1_read(void)
{
	return SERCOM1->USART.DATA.reg;
}
