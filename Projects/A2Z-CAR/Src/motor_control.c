#include "motor_control.h"

/*
	P = ?
	I = ?
	ctrl_out_min = 0 (% PWM)
	ctrl_out_max = 100 (% PWM)
	error = 0;
	integral = 0;

	error = required current - measured current;
	integral = integral + error;
	ctrler_out = P * error + I * integral;

	if (ctrler_out < ctrler_out_min) {
		ctrler_out = ctrler_out_min;
		integral = integral - error;
	}
	else if (ctrler_out > ctrler_out_max) {
		ctrler_out = ctrler_out_max;
		integral = integral - error;
	}
*/

float p_value = 0.1;
float i_value = 0.1;
float error = 0.0;
float integral = 0.0;
float required_current = 0.0;
float measured_current = 0.0;
float ctrler_out = 0.0;

float pi_control() {
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

	while(1) {
		motor_pwm_set_duty(pi_control());
	}

	while (1) {
		/* Delete the thread */
		osThreadTerminate(NULL);
	}
}
