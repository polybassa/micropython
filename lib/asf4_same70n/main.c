#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	cdcd_acm_example();

	/* Replace with your application code */
	while (1) {
	}
}
