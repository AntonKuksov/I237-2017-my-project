#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "cli_microrl.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "andygock_avr-uart/uart.h"
#include "helius_microrl/microrl.h"
#include "hd44780_111/hd44780.h"

#define ASCII_CHARS 128
#define BLINK_DELAY_MS 100
#define LED_PORT PORTA0 /* Arduino Mega digital pin 22 */
#define UART_BAUD 9600
#define UART_STATUS_MASK 0x00FF

/* Create a command line interface instance */
static microrl_t rl;
static microrl_t *prl = &rl;


static inline void init_leds(void)
{
    /* The following pin configuration is given for Arduino Mega. */
    /* Set port A pins 0, 2, 4 for output (digital pins 22, 24, 26) */
    DDRA |= _BV(DDA0) | _BV(DDA2) | _BV(DDA4);
    DDRB |= _BV(DDB7); /* Set port B pin 7 for output (dig. pin 13) */
    PORTB &= ~_BV(PORTB7); /* Turn onboard LED off */
}


/* Init console as stdout/stdin in UART0 */
static inline void init_stdcon(void)
{
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
}


/* Init error console as stderr in UART1 and print user code info */
static inline void init_errcon(void)
{
    uart1_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart1_puts_p(ver_fw);
    uart1_puts_p(ver_libc);
}


/* Initialize system timer to count seconds */
static inline void init_sys_timer(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12); // Turn on CTC (Clear Timer on Compare)
    TCCR1B |= _BV(CS12); // fCPU/256
    OCR1A = 62549; // Note that it is actually two registers OCR5AH and OCR5AL
    TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
}


/* Initialize LCD display and display student's name */
static inline void init_display(void)
{
    lcd_init();
    /* Prepare LCD screen for usage */
    lcd_clrscr(); /* Clear LCD screen and move cursor into the start */
    lcd_puts_P(stud_name); /* Print student's name on LCD */
}


/* The following code is taken from a snippet
 * (https://gitlab.com/I237-2017/code-snippets/snippets/1686121) */
static inline void heartbeat(void)
{
    static time_t prev_time;
    char ascii_buf[11] = {0x00};
    time_t now = time(NULL);

    if (prev_time != now)
    {
        /* Print uptime to uart1 */
        ltoa(now, ascii_buf, 10);
        uart1_puts_p(PSTR("Uptime: "));
        uart1_puts(ascii_buf);
        uart1_puts_p(PSTR(" s.\r\n"));
        PORTA ^= _BV(LED_PORT); /* Toggle LED */
        prev_time = now;
    }
}

void main(void)
{
    init_sys_timer(); /* Initialize system timer */
    init_leds(); /* Configure LED ports */
    init_errcon(); /* Initialize error console */
    init_stdcon(); /* Initialize main console */
    init_display(); /* Initialize LCD display */
    sei(); /* Enable interrupts */
    
    uart0_puts_p(stud_name); /* Print student's name in console */
    uart0_puts_p(PSTR("\r\n"));
    /* Call init with pointer to microrl instance and print callback */
    microrl_init(prl, uart0_puts);
    /* Set callback for execute */
    microrl_set_execute_callback(prl, cli_execute);
    

    while (1)
    {
        /* CLI commands are handled in cli_execute() */
        microrl_insert_char(prl, (uart0_getc() & UART_STATUS_MASK));
        heartbeat();
    }
}

/* Counter 1 ISR (interrupt service routine) */
ISR(TIMER1_COMPA_vect)
{
    system_tick();
}
