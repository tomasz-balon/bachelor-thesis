/**
 * File including basic packet sniffer
 */

#include "pio/samd21e17a.h"
#include "instance/port.h"
#include "sniffer.h"

/**
 * Function to detect break field
 */
char detect_break(void)
{
	unsigned char bits_counter = 0, local_is_break = 0, is_break = 0;
	if ((REG_PORT_IN0&PIN_PA24) == 0)
	{
		bits_counter++;
		if (bits_counter >= 13);
		{
			local_is_break = 1;
		}
	}
	else if (local_is_break == 1 && (REG_PORT_IN0&PIN_PA24) == 1)
	{
		is_break = 1;
	}
	else
	{
		is_break = 0;
	}
	return is_break;
}

/**
 * Function to detect sync field
 */
char detect_sync(void)
{
	unsigned char bits_counter = 0;
	unsigned char bits_array[8];
	unsigned char look_up_array[8] = {0, 1, 0, 1, 0, 1, 0, 1};
	unsigned char result_array[8];
	unsigned char is_okay = 1;
	unsigned char is_break = detect_break();
	
	if (is_break == 1)
	{
		for (bits_counter = 0; bits_counter < 8; bits_counter++)
		{
			bits_array[bits_counter] = (REG_PORT_IN0&PIN_PA24);
		}
		for (bits_counter = 0; bits_counter < 8; bits_counter++)
		{
			if (bits_array[bits_counter] == look_up_array[bits_counter])
			{
				result_array[bits_counter] = 1;
			}
			else
			{
				result_array[bits_counter] = 0;
			}
		}
		for (bits_counter = 0; bits_counter < 8; bits_counter++)
		{
			if (result_array[bits_counter] == 0)
			{
				is_okay = 0;
				break;
			}
		}
		if (is_okay == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/**
 * Function to decide whether incoming data is frame
 */
char detect_frame(void)
{
	char is_frame = 0;
	
	unsigned char is_break = detect_break();
	if (is_break == 1)
	{
		unsigned char is_sync = detect_sync();
		if (is_sync == 1)
		{
			is_frame = 1;
		}
		else
		{
			is_frame = 0;
		}
	}
	else
	{
		is_frame = 0;
	}
	
	return is_frame;
}

/**
 * This function gets pid sent from frame
 * and will store it in variable.
 */
int get_pid_decimal(void)
{
	unsigned char bits_counter = 0;
	unsigned char pid_array[8];
	int result = 0;
	
	for (bits_counter = 0; bits_counter < 10; bits_counter++)
	{
		pid_array[bits_counter] = (REG_PORT_IN0&PIN_PA24);
	}
	
	int len = (sizeof(pid_array)/pid_array[0]);
	
	for (int i = 0; i < len; i++)
	{
		result |= pid_array[i];
		result <<= 1;
	}
	return result;
}

/**
 * This function checks if received pid is correct
 */
unsigned char check_pid(int pid)
{
	unsigned char result = 0;
	
	for (int i = 0; i < 60; i++)
	{
		if (lookup_table[i] == pid)
		{
			result = 1;
			break;
		}
		else
		{
			result = 0;
		}
	}
	return result;
}

/**
 * This function sends pid stored in variable.
 */
void send_pid(int pid)
{
	int pid_array[8];
	int pid_bit = 0;
	unsigned int pid_mask = 1U << 7;
	
	for (int i = 0; i < 8; i++)
	{
		pid_array[i] = (pid & pid_mask) ? 1 : 0;
		pid <<= 1;
	}
	
	for (int x = 0; x < 8; x++)
	{
		pid_bit = pid_array[x];
		
		if (pid_bit == 1)
		{
			REG_PORT_OUTSET0 = PORT_PA01;
		}
		else
		{
			REG_PORT_OUTCLR0 = PORT_PA01;
		}
	}
}
