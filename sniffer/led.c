/**
 * File operating on LEDs
 */

#include "pio/samd21e17a.h"
#include "instance/port.h"
#include "led.h"

void set_led(char led1, char led2, char led3, char led4)
{
	led_off();
	
	if (led1 == 1)
	{
		REG_PORT_OUT0 &= ~PORT_PA08;
	}
	else if (led1 == 0)
	{
		REG_PORT_OUT0 |= PORT_PA08;
	}
	if (led2 == 1)
	{
		REG_PORT_OUT0 &= ~PORT_PA09;
	}
	else if (led2 == 0)
	{
		REG_PORT_OUT0 |= PORT_PA09;
	}
	if (led3 == 1)
	{
		REG_PORT_OUT0 &= ~PORT_PA10;
	}
	else if (led3 == 0)
	{
		REG_PORT_OUT0 |= PORT_PA10;
	}
	if (led4 == 1)
	{
		REG_PORT_OUT0 &= ~PORT_PA11;
	}
	else if (led4 == 0)
	{
		REG_PORT_OUT0 |= PORT_PA11;
	}
}

void led_off(void)
{
	REG_PORT_OUT0 |= (PORT_PA08|PORT_PA09|PORT_PA10|PORT_PA11);
}
