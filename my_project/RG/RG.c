#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <wiringx.h>

#include <signal.h>
#include "my_move.h"
#define PORT 12345
#define BUFFER_SIZE 1024
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
double initial_S=0.0;
int err[2] ={0,0};
float error[2] ={0.0,0.0};           // 当前误差
float last_error[2] ={0.0,0.0};      // 上一次误差
float integral[2] = {0.0,0.0};        // 积分项
float derivative[2] = {0.0,0.0};      // 微分项
float output[2] = {0.0,0.0};  
float Kp[2] = {2.5,1};   // 比例系数
float Ki[2] = {0.01,0.0};  // 积分系数
float Kd[2] = {1.0,0.0};   // 微分系数       
float P[2] = {0.0,0.0}, I[2] = {0.0,0.0}, D[2] = {0.0,0.0}; // PID 输出
int initial_speed=10000,additional_speed=2500;
struct PPI{
    double x,y;
};
int server_fd, client_fd;
void cleanup(void) {
    printf("程序退出，执行清理操作...\n");
    digitalWrite(ain1, LOW);
    digitalWrite(ain2, LOW);
    digitalWrite(bin1, LOW);
    digitalWrite(bin2, LOW);
    // FILE *fp = fopen("mem.txt", "w");
    // if (fp == NULL) {
    //     perror("无法打开 mem.txt 写入");
    // } else {
    //     fprintf(fp, "%d\n", overflow);
    //     fprintf(fp, "%d\n", cnt_mpu);
    //     int len = cnt_mpu;
    //     for (int i = 1; i <= len; ++i) {
    //         fprintf(fp, "%d ", mpu_array[i]);
    //     }
    //     fclose(fp);
    // }
    // 关闭PWM，释放资源等
}

void sigint_handler(int sig) {
    cleanup();
    close(client_fd);
    close(server_fd);
    exit(0);
}
struct PPI split(char *str) {
    struct PPI result;
    char *token = strtok(str, " ");
    if (token != NULL) {
        result.x = atof(token);
        token = strtok(NULL, " ");
        if (token != NULL) {
            result.y = atof(token);
        }
    }
    return result;
}
int main() {

    signal(SIGINT, sigint_handler); 
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char last_buffer[BUFFER_SIZE];

    // 创建 TCP 套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // 设置服务器地址结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // 监听所有网卡
    server_addr.sin_port = htons(PORT);

    // 绑定套接字到地址
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(server_fd);
        exit(1);
    }

    // 监听连接
    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // 接受客户端连接
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }

    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));
    if(wiringXSetup("milkv_duo", NULL) == -1) {
        wiringXGC();
        return -1;
    }
    pinMode(ain1, PINMODE_OUTPUT);
    pinMode(ain2, PINMODE_OUTPUT);
    pinMode(bin1, PINMODE_OUTPUT);
    pinMode(bin2, PINMODE_OUTPUT);
    // 接收客户端发送的数据
    while (1) {
        int n = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (n <= 0) {
            printf("Connection closed or error\n");
            break;
        }
        buffer[n] = '\0';  // 使字符串正确结束
        // printf("Received data: %s", buffer);  // 不需要打印多余的换行符
        if(buffer[0]=='1'){
            my_move(initial_speed,1,initial_speed,1);
        }else {
            my_move(0,0,0,0);
        }
    }
    // 关闭套接字
    close(client_fd);
    close(server_fd);

    return 0;
}