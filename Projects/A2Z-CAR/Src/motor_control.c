#include "motor_control.h"
#include "cmsis_os.h"
#include "pwm_driver.h"
#include "adc_driver.h"
#include "proximity_driver.h"
#include "servo_control.h"
#include "wifi_functions.h"
#include "main.h"

float ctrler_out_min = 0;
float ctrler_out_max = 100;

float p_value = 0.1;
float i_value = 0.1;
int16_t error = 0;
int16_t integral = 0;
int16_t required_rpm = 0;
int16_t measured_rpm = 0;
float ctrler_out = 0.0;
float duty;

float pi_control();
void print_float(float value, int decimal_digits);


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


void pin_init()
{
	// Initialize pin D6 and D8 (PB1 and PB2) as output for motor direction control
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FAST;
	GPIO_InitDef.Pin = GPIO_PIN_1 | GPIO_PIN_2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
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
	error = required_rpm - measured_rpm;
	integral += error;
	ctrler_out = p_value * (float)error + i_value * (float)integral;
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


void stop_drive()
{
	started = 0;
	HAL_TIM_PWM_Stop(&motor_pwm_handle, TIM_CHANNEL_1);
	printf("Stopping car\n");
}


void disable_drive()
{
	stop_drive();
	HAL_TIM_PWM_DeInit(&motor_pwm_handle);
	// Disable output pin
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);
}


void go()
{
	if (started == 0) {
		printf("Starting car\n");
		for (uint8_t i = 25; i > 15; i--) {
			duty = i;
			motor_pwm_set_duty(duty);
			osDelay(30);
		}
		started = 1;
	}
}


void accelerate()
{
	if (started == 1) {
		duty *= 1.2;
		motor_pwm_set_duty(duty);
	}
}


void decelerate()
{
	if (started == 1) {
		duty *= 0.8;
		motor_pwm_set_duty(duty);
	}
}


void motor_control_thread(void const * argument)
{
	started = 0;
	pin_init();			// initialize direction pins
	set_direction(1);	// set forward


//	required_rpm = 2000;

	while(1) {
		process_all_data();
//		measured_rpm = adc_rpm_measure();
//		motor_pwm_set_duty(pi_control());
		osDelay(10);
	}

	terminate_thread();
}

void process_all_data()

{
	//wifi_flag:			10 = data send ok,	20 = Failed to send data
	//object_flag:			10 = go, 			20 = decelerate, 								40 = stop
	//line_flag:			10 = line ok, 		20 = no line
	//user_command_flag: 	10 = go, 			20 = decelerate, 			30 = accelerate,	40 = stop

	//if no connection, or stop button pressed, or object in dangerous proximity --> STOP
	if ((wifi_flag == 20) || (user_command_flag == 40) || (object_flag == 40)) {
		stop_drive();

	//if connection ok and decelerate button pressed, or object is in mid range --> decelerate
	} else if ((wifi_flag == 10) && ((user_command_flag == 20) || (object_flag == 20))) {
		decelerate();

	//if connection ok and accelerate button is pressed, or object is out of mid range --> accelerate
	} else if ((wifi_flag == 10) && (user_command_flag == 30) && (object_flag == 10)) {
		accelerate();

	//if connection ok and  start button is pressed and no object in range --> go
	} else if ((wifi_flag == 10) && (user_command_flag == 10) && (object_flag == 10)) {
		go();
	}
//	printf("wifi: %2d      object: %2d      line: %2d      user: %2d\n", wifi_flag, object_flag, line_flag, user_command_flag);
//	osDelay(200);
}

