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
