/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <sniffer.h>
#include "pio/samd21e17a.h"
#include "instance/port.h"
#include "core_cmFunc.h"
#include "led.h"
#include "uart.h"
#include "../../../../../Programy_na_uczelnie/Atmel Toolchain/ARM GCC/Native/4.8.1443/CMSIS_Atmel/Device/ATMEL/samd21/include/instance/eic.h"
#include "../../../../../Programy_na_uczelnie/Atmel Toolchain/ARM GCC/Native/4.8.1443/CMSIS_Atmel/Device/ATMEL/samd21/include/component/eic.h"
#include "instance/eic.h"

void Sniffer(void)
{
	enum SinfferState PreviousErrorState, PreviousState, CurrentState = INIT;
	unsigned char is_frame = 0;
	unsigned char is_pid_correct = 0;
	int pid = 0;
	while(1)
	{
		PreviousState = CurrentState;
		if(PreviousState != CurrentState)
		{
			switch(CurrentState)
			{
				case INIT:
					set_led(1, 1, 1, 1);
					CurrentState = IDLE;
					
				case IDLE:
					switch(PreviousState)
					{
						case INIT:
							set_led(0, 0, 0, 1);
							break;
						case SEND_FRAME:
							set_led(1, 0, 0, 0);
							break;
						case ERROR:
							break;
						default:
							break;
					}
					if (UART1_data_incoming())
					{
						CurrentState = DETECT_FRAME;
					}
					else
					{
						CurrentState = IDLE;
					}
					break;
					
				case DETECT_FRAME:
					set_led(0, 0, 1, 0);
					is_frame = detect_frame();
					if (is_frame == 1)
					{
						CurrentState = DETECT_PID;
					}
					else
					{
						CurrentState = ERROR;
						PreviousErrorState = DETECT_FRAME;
					}
					break;
					
				case DETECT_PID:
					set_led(0, 1, 0, 0);
					pid = get_pid_decimal();
					is_pid_correct = check_pid(pid);
					
					if (is_pid_correct == 1)
					{
						CurrentState = SEND_FRAME;
					}
					else
					{
						CurrentState = ERROR;
						PreviousErrorState = DETECT_PID;
					}
					break;
					
				case SEND_FRAME:
					set_led(1, 0, 0, 0);
					send_pid(pid);
					CurrentState = IDLE;
					break;
					
				case ERROR:
					if (PreviousErrorState == DETECT_FRAME)
					{
						set_led(0, 0, 1, 1);
					}
					else if (PreviousErrorState == DETECT_PID)
					{
						set_led(0, 1, 0, 1);
					}
					break;
				default:
					break;
			}
		}
	}
}

void delayMs(int miliseconds)
{
	int iterator;
	for (; miliseconds>0; miliseconds--)
	{
		for (iterator=0; iterator<199; iterator++)
		{
			__asm("nop");
		}
	}
}

void write(void)
{
	SERCOM1->USART.DATA.reg = "1";
}

int main (void)
{
	system_init();
	__enable_irq();
	//Sniffer();
	UART1_init(9600);
	//UART3_init(9600);
	set_led(1, 1, 1, 1);
	//PORT->Group[0].PINCFG[0].bit.PMUXEN = 1;
	//PORT->Group[0].PMUX[0].reg |= PORT_PMUX_PMUXE_D;
	system_interrupt_enable(SERCOM1_IRQn);
	delayMs(1000);
	
	while(1)
	{
		set_led(0, 0, 0, 1);
		delayMs(2000);
		//char character = "18535\0";
		//UART1_write_str(character);
		//REG_EIC_INTENSET = EIC_INTENSET_EXTINT0;
		//write();
		//REG_EIC_INTFLAG = EIC_INTFLAG_EXTINT0;
		set_led(0, 0, 0, 0);
		delayMs(2000);
		
		if (UART1_data_incoming())
		{
			set_led(1, 1, 1, 1);
			char c = UART1_read();
			UART1_write_str(c);
			delayMs(2000);
		}
		
	}
}
