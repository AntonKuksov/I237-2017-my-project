#include <stdio.h>
#include <avr/pgmspace.h>
#include "andygock_avr-uart/uart.h"
#include "print_helper.h"

/* Print only text characters of ASCII chart (whitespace to tilde) */
void print_ascii_tbl(void)
{
    for (char c = ' '; c <= '~'; c++)
    {
        uart0_putc(c);
    }

    uart0_puts_p(PSTR("\r\n")); /* Insert CR+LF */
}

/* Print given array of characters in human-readable form */
void print_for_human(const unsigned char *array, const size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (array[i] >= ' ' && array[i] <= '~')
        {
            uart0_putc(array[i]);
        }
        else
        {
            /* sprintf would be fine, but it bloats the firmware */
            uart0_puts_p(PSTR("\"0x"));
            uart0_putc((array[i] >> 4) + ((array[i] >> 4) <= 9 ? 0x30 : 0x37));
            uart0_putc((array[i] & 0x0F) + ((array[i] & 0x0F) <= 9 ? 0x30 : 0x37));
            uart0_putc('"');
        }
    }
    uart0_puts_p(PSTR("\r\n"));
}
