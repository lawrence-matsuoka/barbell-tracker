#include <msp430.h> 
#include "uart.h" // import the UART library


int main(void)
{

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    uart_init();
    uart_send_string("uart works\n");
	
	return 0;
}
