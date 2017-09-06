#include "servo_control.h"
#include "adc_driver.h"
#include "pwm_driver.h"
#include "cmsis_os.h"
#include "main.h"

uint16_t cnt = 0, cnt_limit = 100;

void set_servo_angle(int8_t angle);
void set_servo();

void set_servo_angle(int8_t ang)
{
	// 5% duty cycle is the leftmost position of the steering, 10% is the rightmost,
	// for now leftmost is -45 degrees, rightmost is 45,
	// so 1 degree equals to (5 / 90) % in duty cycle.
	// 7.5 % is 0 degrees
	int8_t angle = ang;
	// don't overdo the physical limits
	if (ang > 36)
		angle = 36;
	else if (ang < -36)
		angle = -36;
	float duty = 7.5 + ((5.0 / 90.0) * (float)angle);
	servo_pwm_set_duty(duty);

}

int8_t angle (int8_t current_bias)
{
	uint8_t p = 1, d = 0;
	int8_t a = p * current_bias + d * ( current_bias - former_bias);
	//int8_t a = current_bias;
#ifdef DEBUG_MODE
	printf("d, a:***%3d ***%3d*** \n", d * ( current_bias - former_bias), current_bias);
	printf("angle: %4d \n",a);
	printf("d comp:  %4d \n",d * ( current_bias - former_bias));
#endif
	return a;
}

void do_this_if_no_line()
{
	line_flag = 20;

	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	//set_servo_angle(angle(former_bias));
}

void set_servo()
{
	uint8_t detail = 9;
	int8_t bias = get_bias();
	//bias = 0;
	if (bias <= (detail * 4)) {
		cnt = 0;
	} else {
		cnt++;
	}
	if (bias <= (detail * 4) || cnt < cnt_limit) {
		line_flag = 10;
		if (bias > (detail * 4)) {
			set_servo_angle(angle(former_bias));
		} else {
			set_servo_angle(angle(bias));
		}
	} else {
		do_this_if_no_line();
	}
}

void servo_control_thread(void const * argument)
{
	calibrate();
	line_flag = 0;
	while(1) {
		set_servo();
		osDelay(10);
	}

	terminate_thread();
}
