// #include <stdio.h>
// #include <unistd.h>

// #include <wiringx.h>

// int main() {
//     // Duo/Duo256M: LED = 25
//     // DuoS:        LED =  0
//     int DUO_LED = 25;

//     // Duo:     milkv_duo
//     // Duo256M: milkv_duo256m
//     // DuoS:    milkv_duos
//     if(wiringXSetup("milkv_duo", NULL) == -1) {
//         wiringXGC();
//         return 1;
//     }

//     if(wiringXValidGPIO(DUO_LED) != 0) {
//         printf("Invalid GPIO %d\n", DUO_LED);
//     }

//     pinMode(DUO_LED, PINMODE_OUTPUT);

//     while(1) {
//         printf("Duo LED GPIO (wiringX) %d: High\n", DUO_LED);
//         digitalWrite(DUO_LED, HIGH);
//         sleep(1);
//         printf("Duo LED GPIO (wiringX) %d: Low\n", DUO_LED);
//         digitalWrite(DUO_LED, LOW);
//         sleep(1);
//     }

//     return 0;
// }

#include <stdio.h>
#include <unistd.h>
#include <wiringx.h>

void softwarePWM(int pin, int period_us, int duty_cycle) {
    digitalWrite(pin, HIGH);
    usleep(duty_cycle);
    digitalWrite(pin, LOW);
    usleep(2*period_us - duty_cycle);
}

int main() {
    int DUO_LED = 25;
    
    if(wiringXSetup("milkv_duo", NULL) == -1) {
        wiringXGC();
        return 1;
    }

    pinMode(DUO_LED, PINMODE_OUTPUT);

    while(1) {
        // 渐亮
        for(int i = 0; i <= 100; i++) {
            for(int j = 0; j < 10; j++) {
                softwarePWM(DUO_LED, 100, i);
            }
        }
        
        // 渐灭
        for(int i = 100; i >= 0; i--) {
            for(int j = 0; j < 10; j++) {
                softwarePWM(DUO_LED, 100, i);
            }
        }
    }
    
    return 0;
}