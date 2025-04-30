// /**
//  * Copyright (c) 2023 Milk-V
//  *
//  * SPDX-License-Identifier: BSD-3-Clause
//  **/

// // #include <stdio.h>
// // #include <unistd.h>

// // #include <wiringx.h>

// //  /* Duo 相关芯片的 PWM 示例代码。
// //     Sample code for PWM in Chip Duo.

// //     注意：确保设备在3.3伏而不是5伏的电压下使用。Duo GPIO 不能在5伏电平下使用。
// //     NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Duo
// //     GPIO cannot be used at 5v.

// //     如果你想在5伏的电压下使用该模块，需要额外加装电平转换器转换电平。
// //     You will need to use a level shifter on the lines if you want to run the
// //     board at 5v.

// //     请将示波器、逻辑分析仪或是任何能使用 PWM 控制的元件接到任意支持 PWM 的引脚上。
// //     Please connect an oscilloscope, logic analyzer, or any component that 
// //     can use PWM control to any PWM-supported pin.

// //     运行程序前请确保引脚复用在正确的功能上。
// //     Please make sure the pins are multiplexed to the correct functions before 
// //     running the program.

// //     运行程序后请按提示输入 [引脚号]:[占空比] 以设置对应引脚上的 PWM 信号，比如 3:500 。
// //     After running the program, please follow the prompts to enter the 
// //     [pin number]:[duty] cycle to set the PWM signal on the corresponding pin, 
// //     such as 3:500.
// //  */

// // int main() {
// //     int pwm_pin = 5;
// //     int duty = 100;

// //     // Duo:     milkv_duo
// //     // Duo256M: milkv_duo256m
// //     // DuoS:    milkv_duos
// //     if(wiringXSetup("milkv_duo", NULL) == -1) {
// //         wiringXGC();
// //         return 1;
// //     }

// //     if(wiringXValidGPIO(pwm_pin) != 0) {
// //         printf("Invalid GPIO %d\n", pwm_pin);
// //     }
// //     int ain1=14;
// //     int ain2=15;
// //     pinMode(ain1, PINMODE_OUTPUT);
// //     digitalWrite(ain1, LOW);
// //     pinMode(ain2, PINMODE_OUTPUT);
// //     digitalWrite(ain2, HIGH);
// //     pinMode(pwm_pin, PINMODE_OUTPUT);
// //     digitalWrite(pwm_pin, HIGH);
// //     // printf("PWM Period fixed to 1000ns, please set Duty in range of 0-1000.\n");
// //     // printf("Enter -> Pin:Duty\n");
// //     // scanf("%d:%d",&pwm_pin,&duty);

// //     // if(duty < 0) duty = 0;
// //     // if(duty > 1000) duty = 1000;

// //     //设置 PWM 周期为 1000 ns; Set PWM Period 1000 ns long.
// //     wiringXPWMSetPeriod(pwm_pin, 1000);

// //     //设置 PWM 占空比; Set PWM Duty.
// //     wiringXPWMSetDuty(pwm_pin, duty);

// //     //设置 PWM 极性并使能; Set PWM Polarity and enable.
// //     wiringXPWMSetPolarity(pwm_pin, 0);
// //     wiringXPWMEnable(pwm_pin, 1);

// //     printf("pin %d -> duty %d\n",pwm_pin ,duty);
// //     while(1) {
// //         printf("pin %d -> duty %d\n",pwm_pin ,duty);
// //         sleep(1);
// //     }
// //     return 0;
// // }


// // #include <stdio.h>
// // #include <unistd.h>
// // #include <wiringx.h>

// // int main() {
// //     int pwm_pin = 2;  // 默认 PWM 引脚
// //     int duty = 1000;    // 默认 50% 占空比

// //     // 初始化 wiringX
// //     if(wiringXSetup("milkv_duo", NULL) == -1) {
// //         printf("wiringX setup failed!\n");
// //         return 1;
// //     }

// //     // 初始化电机方向控制（假设 H 桥驱动）
// //     int ain1 = 14, ain2 = 15;
// //     pinMode(ain1, PINMODE_OUTPUT);
// //     pinMode(ain2, PINMODE_OUTPUT);
// //     digitalWrite(ain1, LOW);   // 方向1
// //     digitalWrite(ain2, HIGH);  // 方向2
// //     pinMode(pwm_pin, PINMODE_OUTPUT);
// //     // digitalWrite(pwm_pin, LOW);
// //     wiringXPWMSetPeriod(pwm_pin, 100000);
// //     wiringXPWMSetDuty(pwm_pin, duty);  // 初始占空比
// //     wiringXPWMSetPolarity(pwm_pin, 0);
// //     wiringXPWMEnable(pwm_pin, 1);

// //     for(int i=1;i<=100000;i+=100){
// //         duty=i;
// //         wiringXPWMSetDuty(pwm_pin, duty);
// //         usleep(10000);
// //     }
// //     // while(1) {
// //     //     printf("Enter -> Pin:Duty: ");
// //     //     scanf("%d:%d", &pwm_pin, &duty);

// //     //     // 限制占空比范围（0-100%）
// //     //     if(duty < 0) duty = 0;
// //     //     if(duty > 50000) duty = 50000;  // 100% of 50,000ns

// //     //     wiringXPWMSetDuty(pwm_pin, duty);
// //     //     printf("Pin %d -> Duty %d ns (%.1f%%)\n", 
// //     //            pwm_pin, duty, (duty / 50000.0) * 100);
// //     // }
// //     return 0;
// // }

// // #include <stdio.h>
// // #include <unistd.h>
// // #include <wiringx.h>

// // int main() {
// //     int pwm_pin = 2;    // 确认硬件PWM引脚（请确认 2 号引脚支持PWM功能）
// //     int ain1 = 14, ain2 = 15;
// //     int period = 200000; // 20,000,000 ns = 20ms -> 50Hz
// //     int duty = 100000;   // 50% 占空比

// //     if (wiringXSetup("milkv_duo", NULL) == -1) {
// //         printf("wiringX setup failed!\n");
// //         return 1;
// //     }

// //     // 初始化方向控制
// //     pinMode(ain1, PINMODE_OUTPUT);
// //     pinMode(ain2, PINMODE_OUTPUT);
// //     digitalWrite(ain1, HIGH);  // 正转方向
// //     digitalWrite(ain2, LOW);

// //     // **不要**对 PWM 引脚调用 pinMode()，以免覆盖 PWM 硬件配置
// //     // 配置PWM -- 建议先设置参数，再启用
// //     wiringXPWMSetPolarity(pwm_pin, 0);
// //     wiringXPWMSetPeriod(pwm_pin, period);
// //     wiringXPWMSetDuty(pwm_pin, duty);
// //     if (wiringXPWMEnable(pwm_pin, 1) != 0) {
// //         printf("PWM enable failed on pin %d\n", pwm_pin);
// //     }

// //     printf("Motor should be running...\n");
// //     sleep(10); // 运行10秒

// //     // 停止电机
// //     wiringXPWMEnable(pwm_pin, 0);
// //     digitalWrite(ain1, LOW);
// //     digitalWrite(ain2, LOW);
// //     return 0;
// // }



// /**
//  * Copyright (c) 2023 Milk-V
//  *
//  * SPDX-License-Identifier: BSD-3-Clause
//  **/

//  #include <stdio.h>
//  #include <unistd.h>
 
//  #include <wiringx.h>
 
//   /* Duo 相关芯片的 PWM 示例代码。
//      Sample code for PWM in Chip Duo.
 
//      注意：确保设备在3.3伏而不是5伏的电压下使用。Duo GPIO 不能在5伏电平下使用。
//      NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Duo
//      GPIO cannot be used at 5v.
 
//      如果你想在5伏的电压下使用该模块，需要额外加装电平转换器转换电平。
//      You will need to use a level shifter on the lines if you want to run the
//      board at 5v.
 
//      请将示波器、逻辑分析仪或是任何能使用 PWM 控制的元件接到任意支持 PWM 的引脚上。
//      Please connect an oscilloscope, logic analyzer, or any component that 
//      can use PWM control to any PWM-supported pin.
 
//      运行程序前请确保引脚复用在正确的功能上。
//      Please make sure the pins are multiplexed to the correct functions before 
//      running the program.
 
//      运行程序后请按提示输入 [引脚号]:[占空比] 以设置对应引脚上的 PWM 信号，比如 3:500 。
//      After running the program, please follow the prompts to enter the 
//      [pin number]:[duty] cycle to set the PWM signal on the corresponding pin, 
//      such as 3:500.
//   */
// #include <stdio.h>
// #include <unistd.h>
// #include <wiringx.h>

// int main() {
//     int pwm_pin = 5;
//     int duty = 30000;

//     // 初始化 wiringX（确认设备名正确）
//     if(wiringXSetup("milkv_duo", NULL) == -1) {
//         printf("wiringX setup failed\n");
//         return 1;
//     }

//     // 检查引脚是否有效（可选）
//     // if(wiringXValidGPIO(pwm_pin) != 0) {
//     //     printf("Invalid GPIO %d\n", pwm_pin);
//     //     return 1;
//     // }
//     int ain1 = 14, ain2 = 15;
//     pinMode(ain1, PINMODE_OUTPUT);
//     pinMode(ain2, PINMODE_OUTPUT);
//     digitalWrite(ain1, LOW);   // 方向1
//     digitalWrite(ain2, HIGH);  // 方向2
//     printf("PWM Period fixed to 1000ns, set Duty in 0-1000.\n");
//     printf("Enter -> Pin:Duty (e.g., 3:500): ");
//     int T=100000;
//     // scanf("%d:%d", &T, &duty);

//     // 限制占空比范围

//     // 配置 PWM
//     wiringXPWMSetPeriod(pwm_pin, T);  // 周期 1000ns (1kHz)
//     wiringXPWMSetDuty(pwm_pin, duty);    // 占空比
//     wiringXPWMSetPolarity(pwm_pin, 0);   // 极性（0 或 1）
//     wiringXPWMEnable(pwm_pin, 1);        // 启用 PWM

//     printf("PWM enabled on pin %d, duty %d ns\n", pwm_pin, duty);

//     // 保持程序运行（防止退出后 PWM 停止）
//     sleep(1);
//     digitalWrite(ain1, LOW);   // 方向1
//     digitalWrite(ain2, LOW);  // 方向2
//     return 0;
// }




#include <stdio.h>
#include <unistd.h>

#include <wiringx.h>

 /* Duo 相关芯片的 PWM 示例代码。
    Sample code for PWM in Chip Duo.

    注意：确保设备在3.3伏而不是5伏的电压下使用。Duo GPIO 不能在5伏电平下使用。
    NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Duo
    GPIO cannot be used at 5v.

    如果你想在5伏的电压下使用该模块，需要额外加装电平转换器转换电平。
    You will need to use a level shifter on the lines if you want to run the
    board at 5v.

    请将示波器、逻辑分析仪或是任何能使用 PWM 控制的元件接到任意支持 PWM 的引脚上。
    Please connect an oscilloscope, logic analyzer, or any component that 
    can use PWM control to any PWM-supported pin.

    运行程序前请确保引脚复用在正确的功能上。
    Please make sure the pins are multiplexed to the correct functions before 
    running the program.

    运行程序后请按提示输入 [引脚号]:[占空比] 以设置对应引脚上的 PWM 信号，比如 3:500 。
    After running the program, please follow the prompts to enter the 
    [pin number]:[duty] cycle to set the PWM signal on the corresponding pin, 
    such as 3:500.
 */
#include <stdio.h>
#include <unistd.h>
#include <wiringx.h>
#include <stdbool.h>
#define ENCODER_A 26
#define ENCODER_B 27
int pwm_pin1 = 4,pwm_pin2=5;
int duty = 30000;
int ain1 = 14, ain2 = 15;
int bin1=16,bin2=17;
int T=100000;
const int PPR = 13;                      // 编码器线数
const int PULSES_PER_REV = PPR * 4; 
volatile int pulseCount = 0,current_revs=0;  
volatile bool lastA = false, lastB = false;
typedef unsigned char uint8_t;
typedef signed char int8_t; 
int abs(int x){
    return x>0?x:-x;
}
const int8_t TRANSITIONS[16] = {
    0,  // 00 -> 00
    +1, // 00 -> 01
    -1, // 00 -> 10
    0,  // 00 -> 11
    -1, // 01 -> 00
    0,  // 01 -> 01
    0,  // 01 -> 10
    +1, // 01 -> 11
    +1, // 10 -> 00
    0,  // 10 -> 01
    0,  // 10 -> 10
    -1, // 10 -> 11
    0,  // 11 -> 00
    -1, // 11 -> 01
    +1, // 11 -> 10
    0   // 11 -> 11
};
void encoderISR() {
    bool currentA = digitalRead(ENCODER_A);
    bool currentB = digitalRead(ENCODER_B);

    // 计算状态变化
    uint8_t oldState = (lastA << 1) | lastB;
    uint8_t newState = (currentA << 1) | currentB;
    int8_t transition = (oldState << 2) | newState;

    if (transition < 16) {
        pulseCount += TRANSITIONS[transition];
    }
    // printf("ininin\n");
    // 更新状态缓存
    lastA = currentA;
    lastB = currentB;
    // printf("in");
}
void my_move(int speed, bool dir) {
    // 0 后退 1 前进
    if (dir) {
        // printf("dir = 1\n");
        digitalWrite(bin1, LOW);
        digitalWrite(bin2, HIGH);
        digitalWrite(ain1, LOW);
        digitalWrite(ain2, HIGH);
    } else {
        // printf("dir = 0\n");
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

    usleep(1);
    return;
}

int main() {


   // 初始化 wiringX（确认设备名正确）
   if(wiringXSetup("milkv_duo", NULL) == -1) {
       printf("wiringX setup failed\n");
       return 1;
   }

   // 检查引脚是否有效（可选）
   // if(wiringXValidGPIO(pwm_pin) != 0) {
   //     printf("Invalid GPIO %d\n", pwm_pin);
   //     return 1;
   // }
   pinMode(ain1, PINMODE_OUTPUT);
   pinMode(ain2, PINMODE_OUTPUT);
   pinMode(bin1, PINMODE_OUTPUT);
   pinMode(bin2, PINMODE_OUTPUT);
   pinMode(ENCODER_A, PINMODE_INPUT);
   pinMode(ENCODER_B, PINMODE_INPUT);
//    lastA = digitalRead(ENCODER_A);
//    lastB = digitalRead(ENCODER_B);

//    if (wiringXISR(ENCODER_A, ISR_MODE_RISING) < 0 ||wiringXISR(ENCODER_B, ISR_MODE_RISING) < 0) {
//         fprintf(stderr, "中断注册失败\n");
//         return 1;
//     }

//     while (1) {
//         current_revs = pulseCount / PULSES_PER_REV;
//         printf("当前圈数: %d\n", current_revs);
//         my_move(duty, 1);  // 控制电机转动
//         if (abs(current_revs) >= 30) {
//             break;
//         }
//         usleep(100000);  // 100ms
//     }

//     // 停止电机
//     digitalWrite(ain1, LOW);
//     digitalWrite(ain2, LOW);
//     digitalWrite(bin1, LOW);
//     digitalWrite(bin2, LOW);
   lastA = digitalRead(ENCODER_A);
   lastB = digitalRead(ENCODER_B);
   while(1){
        current_revs = pulseCount / (PULSES_PER_REV);
        encoderISR();
        my_move(duty,1);
        usleep(1);
        if(abs(current_revs)>30)
            break;
        // printf("%d\n",current_revs);
   }
   digitalWrite(ain1, LOW);   // 方向1
   digitalWrite(ain2, LOW);  // 方向2
   digitalWrite(bin1, LOW);   // 方向1
   digitalWrite(bin2, LOW);  // 方向2
   return 0;
}