#ifndef SNIFFER_H
#define SNIFFER_H

enum SinfferState {INIT, IDLE, DETECT_FRAME, DETECT_PID, SEND_FRAME, ERROR};

static const int lookup_table[60] =
{
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59
};

char detect_break(void);
char detect_sync(void);
char detect_frame(void);
int get_pid_decimal(void);
unsigned char check_pid(int pid);
void send_pid(int pid);
void send_frame(long frame);

#endif /* SNIFFER_H */