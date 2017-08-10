#include "servo_control.h"
#include "adc_driver.h"
#include "pwm_driver.h"
#include "cmsis_os.h"

void set_servo_angle(int8_t angle)
{
	// 5% duty cycle is the leftmost position of the steering, 10% is the rightmost,
	// for now leftmost is -45 degrees, rightmost is 45,
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
	//printf("valami ");

	int8_t error = get_error();
	//printf("error:%d\n",error);

	if (error != 5) {
		set_servo_angle(error * 5);
		//printf("servo: %d\n", error * 5);
		//BSP_LED_Off(LED2);
	} else {
		//printf("stop\n");
		stop();
	}

}

void servo_control_thread(void const * argument)
{
	while(1) {
		set_servo();
		osDelay(100);
		BSP_LED_Toggle(LED2);
	}
	while (1) {
		/* Delete the thread */
		osThreadTerminate(NULL);
	}
}




