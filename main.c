#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

void ConfigureUART(void) {
	// Enable the GPIO Peripheral used by the UART.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	// Enable UART0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	// Configure GPIO Pins for UART mode.
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Use the internal 16MHz oscillator as the UART clock source.
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

	// Initialize the UART for console I/O.
	UARTStdioConfig(0, 115200, 16000000);
}

/*
 * hello.c
 */
int main(void) {
	volatile uint32_t ui32Loop;

	// Set the clocking to run directly from the crystal.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
	               SYSCTL_OSC_MAIN);

	SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
	
	ui32Loop = SYSCTL_RCGC2_R;

	GPIO_PORTF_DIR_R = 0x08;
	GPIO_PORTF_DEN_R = 0x08;

	ConfigureUART();
	UARTprintf("Hello World\n");

	while(1)
	{
		GPIO_PORTF_DATA_R |= 0x08;

		for (ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
		{
		}

		GPIO_PORTF_DATA_R &= ~(0x08);
		for (ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
		{
		}
	}

	return 0;
}
