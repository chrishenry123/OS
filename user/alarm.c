// This will be the file for the alarm command.

#include <alarm.h>
#include <pcb.h>
#include <processes.h>
#include <mpx/io.h>
#include <sys_req.h>
#include <string.h>
#include <time.h>

#define RTC_INDEX_PORT 0x70
#define RTC_DATA_PORT 0x71
#define RTC_SECONDS_REG 0x00
#define RTC_MINUTES_REG 0x02
#define RTC_HOURS_REG 0x04


//void alarm(struct time trigger, char* msg) {

//	int current_s, current_m, current_h;
	
	// Read seconds, minutes, and hours from RTC registers
//	current_s = bcdToDecimal(readRTC(RTC_SECONDS_REG));
//	current_m = bcdToDecimal(readRTC(RTC_MINUTES_REG));
//	current_h = bcdToDecimal(readRTC(RTC_HOURS_REG));
	
//	if(trigger.hours > current_h && trigger.minutes > current_m && trigger.seconds > current_s) {
//	sys_req(WRITE, COM1, msg, strlen(msg));
//	}
//}
