// #include <wiringx.h>

// #include <stdio.h>
// #include <unistd.h>
// #include <wiringx.h>
// #include <stdbool.h>  // Add this line
// int T=100000,duty = 30000;
// int pwm_pin1 = 4,pwm_pin2=5;
// int ain1 = 14, ain2 = 15,bin1=16,bin2=17;
// void my_move(int speed,bool dir){
//     // 0后退 1前进
//     if(dir){
//         printf("dir = 1\n");
//         digitalWrite(bin1,LOW);
//         digitalWrite(bin2,HIGH);
//         digitalWrite(ain1, LOW);   
//         digitalWrite(ain2, HIGH);  
//     }else{
//         printf("dir = 0\n");
//         digitalWrite(bin1,HIGH);
//         digitalWrite(bin2,LOW);
//         digitalWrite(ain1, HIGH);   
//         digitalWrite(ain2, LOW);         
//     }
//     // printf("bin1: %d, bin2: %d, ain1: %d, ain2: %d\n", 
//     //     digitalRead(bin1), digitalRead(bin2), 
//     //     digitalRead(ain1), digitalRead(ain2));
//     wiringXPWMSetPeriod(pwm_pin1, T);  
//     wiringXPWMSetDuty(pwm_pin1, speed); 
//     wiringXPWMSetPolarity(pwm_pin1, 0);   // 极性（0 或 1）
//     wiringXPWMEnable(pwm_pin1, 1);        // 启用 PWM
//     wiringXPWMSetPeriod(pwm_pin2,T);
//     wiringXPWMSetDuty(pwm_pin2,speed);
//     wiringXPWMSetPolarity(pwm_pin2, 0);   // 极性（0 或 1）
//     wiringXPWMEnable(pwm_pin2, 1);        // 启用 PWM
//     usleep(1000);
//     return ;
// }
// int main() {
//    // 初始化 wiringX（确认设备名正确）
//    if(wiringXSetup("milkv_duo", NULL) == -1) {
//        printf("wiringX setup failed\n");
//        return 1;
//    }
//    pinMode(ain1, PINMODE_OUTPUT);
//    pinMode(ain2, PINMODE_OUTPUT);
//    pinMode(bin1,PINMODE_OUTPUT);
//    pinMode(bin2,PINMODE_OUTPUT);
//    my_move(30000,0);
//    sleep(2);
//    my_move(100000,0);
//    // 保持程序运行（防止退出后 PWM 停止）
//    sleep(2);
//    digitalWrite(bin1,LOW);
//    digitalWrite(bin2,LOW);
//    digitalWrite(ain1, LOW);   
//    digitalWrite(ain2, LOW);     
//    return 0;
// }
#include <wiringx.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

// 定义参数
int T = 100000, duty = 30000;
int pwm_pin1 = 4, pwm_pin2 = 5;
int ain1 = 14, ain2 = 15, bin1 = 16, bin2 = 17;
int encoder_a = 12, encoder_b = 13;  // 假设编码器 A、B 信号连接到 GPIO 12 和 GPIO 13
volatile int pulseCount = 0;  // 电机转动的圈数
volatile bool lastA = false;
volatile bool lastB = false;

// 编码器信号变化的中断服务程序
void encoderISR() {
    bool currentA = digitalRead(encoder_a);  // 读取 A 相的当前状态
    bool currentB = digitalRead(encoder_b);  // 读取 B 相的当前状态

    // 判断旋转方向
    if (currentA != lastA) {
        // A 信号变化时，检查 B 信号来判断旋转方向
        if (currentB != currentA) {
            pulseCount++;  // 顺时针旋转
        } else {
            pulseCount--;  // 逆时针旋转
        }
    }

    lastA = currentA;  // 更新 A 相的状态
    lastB = currentB;  // 更新 B 相的状态
}

// 电机驱动控制
void my_move(int speed, bool dir) {
    // 0 后退 1 前进
    if (dir) {
        printf("dir = 1\n");
        digitalWrite(bin1, LOW);
        digitalWrite(bin2, HIGH);
        digitalWrite(ain1, LOW);
        digitalWrite(ain2, HIGH);
    } else {
        printf("dir = 0\n");
        digitalWrite(bin1, HIGH);
        digitalWrite(bin2, LOW);
        digitalWrite(ain1, HIGH);
        digitalWrite(ain2, LOW);
    }

    wiringXPWMSetPeriod(pwm_pin1, T);
    wiringXPWMSetDuty(pwm_pin1, speed);
    wiringXPWMSetPolarity(pwm_pin1, 0);  // 极性（0 或 1）
    wiringXPWMEnable(pwm_pin1, 1);       // 启用 PWM

    wiringXPWMSetPeriod(pwm_pin2, T);
    wiringXPWMSetDuty(pwm_pin2, speed);
    wiringXPWMSetPolarity(pwm_pin2, 0);  // 极性（0 或 1）
    wiringXPWMEnable(pwm_pin2, 1);       // 启用 PWM

    usleep(1000);
    return;
}

int main() {
    // 初始化 wiringX（确认设备名正确）
    if (wiringXSetup("milkv_duo", NULL) == -1) {
        printf("wiringX setup failed\n");
        return 1;
    }

    // 设置引脚模式
    pinMode(ain1, PINMODE_OUTPUT);
    pinMode(ain2, PINMODE_OUTPUT);
    pinMode(bin1, PINMODE_OUTPUT);
    pinMode(bin2, PINMODE_OUTPUT);
    // pinMode(encoder_a, PINMODE_INPUT);
    // pinMode(encoder_b, PINMODE_INPUT);

    // // 设置编码器中断
    // wiringXISR(encoder_a, ISR_MODE_RISING);  // 在 A 相上升沿触发中断
    // wiringXISR(encoder_b, ISR_MODE_RISING);  // 在 B 相上升沿触发中断

    // 启动电机
    my_move(30000, 0);  // 逆时针旋转
    sleep(2);

    my_move(100000, 0);  // 逆时针旋转
    sleep(2);

    // 输出转动的圈数
    printf("当前旋转圈数: %d\n", pulseCount);

    // 停止电机
    digitalWrite(bin1, LOW);
    digitalWrite(bin2, LOW);
    digitalWrite(ain1, LOW);
    digitalWrite(ain2, LOW);

    return 0;
}