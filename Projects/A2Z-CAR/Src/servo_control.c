#include "servo_control.h"
#include "adc_driver.h"
#include "pwm_driver.h"
#include "cmsis_os.h"

uint16_t cnt = 0, cnt_limit = 100;
int8_t global_bias = 0;
void set_servo_angle(int8_t angle)
{
	// 5% duty cycle is the leftmost position of the steering, 10% is the rightmost,
	// for now leftmost is -45 degrees, rightmost is 45,
	// so 1 degree equals to (5 / 90) % in duty cycle.
	// 7.5 % is 0 degrees
	float duty = 7.5 + ((5.0 / 90.0) * (float)angle);
	servo_pwm_set_duty(duty);
}

void do_this_if_no_line()
{
	BSP_LED_On(LED2);
}

void set_servo()
{
	int8_t bias = get_bias();
	if (bias <= 12)
		global_bias = bias;
	printf("bias:%d\n",bias);
	if (bias <= 12 || cnt < cnt_limit) {
		set_servo_angle(global_bias * 5);
		printf("servo: %d\n", global_bias * 4);
		BSP_LED_Off(LED2);
	} else {
		printf("no line\n");
		do_this_if_no_line();
		cnt++;
	}
}

void servo_control_thread(void const * argument)
{
	while(1) {
		set_servo();
		osDelay(5);
		//BSP_LED_Toggle(LED2);
	}
	while (1) {
		/* Delete the thread */
		osThreadTerminate(NULL);
	}
}

void led_init()
{
	// Initialize D14 and D15 (PB9 and PB8) as LED output
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FAST;
	GPIO_InitDef.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
}

