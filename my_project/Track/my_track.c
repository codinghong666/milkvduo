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
#include"mpu.h"
#include <signal.h>
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define N 100005
static int PWM_PIN = 6; // 9
int err = 0;
float error = 0.0;           // 当前误差
float last_error = 0.0;      // 上一次误差
float integral = 0.0;        // 积分项
float derivative = 0.0;      // 微分项
float output = 0.0;  
float Kp = 2.5;   // 比例系数
float Ki = 0.01;  // 积分系数
float Kd = 1.0;   // 微分系数       
float P = 0.0, I = 0.0, D = 0.0;  
int initial_speed=20000,additional_speed=5000;
int cnt_mpu=0;
bool overflow=0;
short mpu_array[N];
/*
float Kp = 2.5;   // 比例系数
float Ki = 0.01;  // 积分系数
float Kd = 1.0;   // 微分系数     
int initial_speed=10000,additional_speed=4000;
*/
void cleanup(void) {
    printf("程序退出，执行清理操作...\n");
    digitalWrite(ain1, LOW);
    digitalWrite(ain2, LOW);
    digitalWrite(bin1, LOW);
    digitalWrite(bin2, LOW);
    FILE *fp = fopen("mem.txt", "w");
    if (fp == NULL) {
        perror("无法打开 mem.txt 写入");
    } else {
        fprintf(fp, "%d\n", overflow);
        fprintf(fp, "%d\n", cnt_mpu);
        int len = cnt_mpu;
        for (int i = 1; i <= len; ++i) {
            fprintf(fp, "%d ", mpu_array[i]);
        }
        fclose(fp);
    }
    // 关闭PWM，释放资源等
}

void sigint_handler(int sig) {
    cleanup();
    exit(0);
}

int main() {
    signal(SIGINT, sigint_handler); 
    if(wiringXSetup("milkv_duo", NULL) == -1) {
        wiringXGC();
        return -1;
    }
    pinMode(ain1, PINMODE_OUTPUT);
    pinMode(ain2, PINMODE_OUTPUT);
    pinMode(bin1, PINMODE_OUTPUT);
    pinMode(bin2, PINMODE_OUTPUT);
    mpu6050_init();
    // 舵机
    // wiringXPWMSetPeriod(PWM_PIN, 20000000);  // 20ms
    // wiringXPWMSetDuty(PWM_PIN, 1500000);     // 1.5ms stop
    // wiringXPWMSetPolarity(PWM_PIN, 0);       // 0-normal, 1-inversed
    // wiringXPWMEnable(PWM_PIN, 1);            // 1-enable, 0-disable
    printf("Reading IR sensors from I2C device 0x12, register 0x30...\n");

    while(1){
        error=read_ir_sensors();
        P=Kp*error;
        I=Ki*integral;
        D=Kd*derivative;
        output=P+I+D;
        // printf("P: %f, I: %f, D: %f, Output: %f\n", P, I, D, output);
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
            // printf("turn right\n");
            float ans=fmin(output,4.0);
            // float y=-150000*ans+1500000; 
            // float y=125000*ans+1300000;
            // printf("y=%f\n",y);
            // wiringXPWMSetDuty(PWM_PIN, y);
            int add=(int)ans*additional_speed;
            my_move(min(T,initial_speed+add),1,max(0,initial_speed-add),1);
            delayMicroseconds(50000);
        }
        else {
            // printf("turn left\n");
            float ans=fmax(output,-4.0);
            // float y=125000*ans+1300000;
            // printf("y=%f\n",y);
            // wiringXPWMSetDuty(PWM_PIN, y);
            int add=-(int)ans*additional_speed;
            my_move(max(0,initial_speed-add),1,min(T,initial_speed+add),1);
            delayMicroseconds(50000);
        }
        last_error=error;
        integral+=error;
        derivative=error-last_error;    
        usleep(10000);  // 10ms
        if(cnt_mpu<=N-5)
            mpu_array[++cnt_mpu]= (short)(read_mpu6050_yaw()*100);
        else overflow=1;
    }
    return 0;
}