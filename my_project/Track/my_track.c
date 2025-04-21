#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <wiringx.h>
#include <math.h>
#include"my_sensor.h"
#include"my_move.h"
// 定义红外传感器状态变量
static int PWM_PIN = 6; // 9
int err = 0;
float error = 0.0;           // 当前误差
float last_error = 0.0;      // 上一次误差
float integral = 0.0;        // 积分项
float derivative = 0.0;      // 微分项
float output = 0.0;  
float Kp = 1.0;   // 比例系数
float Ki = 0.01;  // 积分系数
float Kd = 0.1;   // 微分系数       
float P = 0.0;   // 比例项
float I = 0.0;   // 积分项
float D = 0.0;   // 微分项
int main() {
    if(wiringXSetup("milkv_duo", NULL) == -1) {
        wiringXGC();
        return -1;
    }
    pinMode(ain1, PINMODE_OUTPUT);
    pinMode(ain2, PINMODE_OUTPUT);
    pinMode(bin1, PINMODE_OUTPUT);
    pinMode(bin2, PINMODE_OUTPUT);
    wiringXPWMSetPeriod(PWM_PIN, 20000000);  // 20ms
    wiringXPWMSetDuty(PWM_PIN, 1500000);     // 1.5ms stop
    wiringXPWMSetPolarity(PWM_PIN, 0);       // 0-normal, 1-inversed
    wiringXPWMEnable(PWM_PIN, 1);            // 1-enable, 0-disable
    printf("Reading IR sensors from I2C device 0x12, register 0x30...\n");
    my_move(30000,1);
    while(1){
        error=read_ir_sensors();
        P=Kp*error;
        I=Ki*integral;
        D=Kd*derivative;
        output=P+I+D;
        printf("P: %f, I: %f, D: %f, Output: %f\n", P, I, D, output);
/*
        0:1500000
        左：240000
        右：900000
        [0,4]-[1500000,900000]
        y=-150000x+1500000
        [0,4]-[1500000,240000]
        y=225000x+1500000
*/
        if(output>0){
            printf("turn right\n");
            float ans=fmin(output,4.0);
            // float y=-150000*ans+1500000; 
            float y=125000*ans+1300000;
            printf("y=%f\n",y);
            wiringXPWMSetDuty(PWM_PIN, y);
            delayMicroseconds(50000);

        }
        else {
            printf("turn left\n");
            float ans=fmax(output,-4.0);
            float y=125000*ans+1300000;
            printf("y=%f\n",y);
            wiringXPWMSetDuty(PWM_PIN, y);
            delayMicroseconds(50000);
        }

        last_error=error;
        integral+=error;
        derivative=error-last_error;    
        usleep(10000);  // 10ms
    }
    return 0;
}