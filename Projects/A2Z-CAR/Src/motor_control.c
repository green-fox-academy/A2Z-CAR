#include "motor_control.h"

//P = ?
//I = ?
//ctrl_out_min = ...
//ctrl_out_max = ...
//error = 0;
//integral = 0;
//error = required current - measured current;
//integral = integral + error;
//ctrler_out = P * err + I * integral;
//
//if (ctrler_out < ctrler_out_min) {
//    ctrler_out = ctrler_out_min;
//    integral = integral - err;
//}
//else if (ctrler_out > ctrler_out_max) {
//    ctrler_out = ctrler_out_max;
//    integral = integral - err;
//}
