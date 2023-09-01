//
// Created by Chris Henry on 9/1/23.
//



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// starting on
#define rtc_index 0x70
#define rtc_data 0x71


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
