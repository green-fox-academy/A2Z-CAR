#include "motor_control.h"
#include "cmsis_os.h"
#include "pwm_driver.h"
#include "adc_driver.h"

float ctrler_out_min = 0;
float ctrler_out_max = 100;

float p_value = 0.001;
float i_value = 0.001;
float error = 0.0;
float integral = 0.0;
float required_current = 0.0;
float measured_current = 0.0;
float ctrler_out = 0.0;

void print_float(float value, int decimal_digits)
{
	int i = 1;
	int int_part, fract_part;
	for ( ; decimal_digits != 0; i *= 10, decimal_digits--);
	int_part = (int)value;
	fract_part = (int)((value - (float)(int)value) * i);
	if (fract_part < 0)
		fract_part *= -1;
	printf("%d.%d", int_part, fract_part);
}

void set_direction(int8_t dir)
{
	if (dir == 1) {				// set forward
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	} else if (dir == -1) {		// set reverse
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
	}
}

float pi_control()
{
	error = required_current - measured_current;
	integral += error;
	ctrler_out = p_value * error + i_value * integral;
	if (ctrler_out < ctrler_out_min) {
		ctrler_out = ctrler_out_min;
		integral -= error;
	}
	else if (ctrler_out > ctrler_out_max) {
		ctrler_out = ctrler_out_max;
		integral -= error;
	}

	return ctrler_out;
}

void motor_control_thread(void const * argument)
{
	// Initialize pin D6 and D8 (PB1 and PB2) as output for directional control
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FAST;
	GPIO_InitDef.Pin = GPIO_PIN_1 | GPIO_PIN_2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);

	// set forward
	set_direction(1);

//	for (uint8_t i = 100; i > 15; i -= 5) {
//		motor_pwm_set_duty(i);
//		osDelay(125);
//	}

	required_current = 24.0;

	while(1) {
		measured_current = (float)adc_current_measure() / 161;
//		printDouble(measured_current, 1);
//		printf("   ");
//		printDouble(pi_control(), 1);
//		printf("\n");
//		osDelay(125);
		motor_pwm_set_duty(pi_control());
	}

	while (1) {
		/* Delete the thread */
		osThreadTerminate(NULL);
	}
}
