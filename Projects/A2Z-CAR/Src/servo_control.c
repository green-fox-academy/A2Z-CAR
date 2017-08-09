#include "servo_control.h"
#include "pwm_driver.h"
#include "cmsis_os.h"

void set_servo_angle(int8_t angle)
{
	// 5% duty cycle is the leftmost position of the steering, 10% is the rightmost,
	// leftmost is -45 degrees now, rightmost is 45,
	// so 1 degree equals to (5 / 90) % in duty cycle.
	// 7.5 % is 0 degrees
	float duty = 7.5 + ((5.0 / 90.0) * (float)angle);
	servo_pwm_set_duty(duty);
}

void stop()
{
	BSP_LED_On(LED2);
}

void set_servo()
{
	int8_t error = get_error();
	printf("error:%d\n",error);
	if (error != 3) {
		set_servo_angle(error * 10);
		printf("servo: %d\n", error * 10);
		//BSP_LED_Off(LED2);
	} else {
		//set_servo_to(0);
		printf("stop\n");
		stop();
	}
}

void servo_control_thread(void const * argument)
{
//	while(1) {
//		for (int8_t i = -45; i < 46; i++) {
//			set_servo_angle(i);
//			osDelay(10);
//		}
//		for (int8_t i = 45; i > -46; i--) {
//			set_servo_angle(i);
//			osDelay(10);
//		}
//	}

	set_servo();

	while (1) {
		/* Delete the thread */
		osThreadTerminate(NULL);
	}
}




