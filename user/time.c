//
// Created by Chris Henry on 9/1/23.
//

#include <mpx/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys_req.h>
#include <string.h>
#include "time.h"

// starting on
#define rtc_index 0x70
#define rtc_data 0x71
#define SECONDS 0x00
#define MINUTES 0X02
#define HOURS 0x04
#define DAY 0x07
#define MONTH 0x08
#define YEAR 0x09


void getDate(void){

	// Defining the variables for day, month and year
	int day;
	int month;
	int year;
	char* day_s = "";
	char* month_s = "";
	char* year_s = "";

	// Displaying the day
	outb(rtc_index, DAY);
	char temp = inb(0x71);
	day = ((temp >> 4) &0x0f) * 10 + (temp &0x0f);
	if(day < 10){
		sys_req(WRITE, COM1, "0", strlen("0"));
	}
	itoa(day, day_s, 10);
	sys_req(WRITE, COM1, day_s, strlen(day_s));
	sys_req(WRITE, COM1, "/", strlen("/"));

	// Displaying the month
	outb(rtc_index, MONTH);
	temp = inb(0x71);
	month = ((temp >> 4) &0x0f) * 10 + (temp &0x0f);
	if(month < 10){
		sys_req(WRITE, COM1, "0", strlen("0"));
	}
	itoa(month, month_s, 10);
	sys_req(WRITE, COM1, month_s, strlen(month_s));
	sys_req(WRITE, COM1, "/", strlen("/"));

	// Displaying the year
	outb(rtc_index, YEAR);
	temp = inb(0x71);
	year = ((temp >> 4) &0x0f) * 10 + (temp &0x0f);
	if(year < 10){
		sys_req(WRITE, COM1, "0", strlen("0"));
	}
	itoa(year, year_s, 10);
	sys_req(WRITE, COM1, year_s, strlen(year_s));
	sys_req(WRITE, COM1, "\n", strlen("\n"));

}


// converts integer to string

void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    // check if 0
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // negative work
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }


    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        if (i >= 20) {
            // if overflow
            return NULL;
        }

        num = num / base;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    // call reverse
    reverse(str, i);

    return str;
}
