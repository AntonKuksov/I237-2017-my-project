#include <avr/pgmspace.h>

const char ver_fw[] PROGMEM = "Version: " FW_VERSION " built on: " __DATE__ " " __TIME__ "\r\n";
const char ver_libc[] PROGMEM = "avr-libc version: " __AVR_LIBC_VERSION_STRING__ " avr-gcc version: " __VERSION__ "\r\n";
const char stud_name[] PROGMEM = "Edwin Weber";
const char number_ask[] PROGMEM = "Enter a number: ";
const char number_0[] PROGMEM = "zero";
const char number_1[] PROGMEM = "one";
const char number_2[] PROGMEM = "two";
const char number_3[] PROGMEM = "three";
const char number_4[] PROGMEM = "four";
const char number_5[] PROGMEM = "five";
const char number_6[] PROGMEM = "six";
const char number_7[] PROGMEM = "seven";
const char number_8[] PROGMEM = "eight";
const char number_9[] PROGMEM = "nine";
PGM_P const number_literal[] PROGMEM = {
    number_0, number_1, number_2, number_3, number_4,
    number_5, number_6, number_7, number_8, number_9
};
