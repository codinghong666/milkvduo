#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <wiringx.h>
#include "my_move.h"
#define PORT 12345
#define BUFFER_SIZE 1024
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
double initial_S=0.0;
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
struct PPI{
    double x,y;
};
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
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

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

    // 接收客户端发送的数据
    while (1) {
        int n = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (n <= 0) {
            printf("Connection closed or error\n");
            break;
        }
        buffer[n] = '\0';  // 使字符串正确结束
        printf("Received data: %s", buffer);  // 不需要打印多余的换行符
        struct PPI result = split(buffer);
        if (initial_S == 0.0) {
            initial_S = result.y;
        }
        error=(result.x-0.5)*8.0;
        P=Kp*error;
        I=Ki*integral;
        D=Kd*derivative;
        output=P+I+D;
        if(output>0){
            float ans=fmin(output,4.0);
            int add=(int)ans*additional_speed;
            my_move(min(T,initial_speed+add),1,max(0,initial_speed-add),1);
            delayMicroseconds(50000);
        }
        else {
            float ans=fmax(output,-4.0);
            int add=-(int)ans*additional_speed;
            my_move(max(0,initial_speed-add),1,min(T,initial_speed+add),1);
            delayMicroseconds(50000);
        }
        last_error=error;
        integral+=error;
        derivative=error-last_error; 
        usleep(10000);   
    }
    // 关闭套接字
    close(client_fd);
    close(server_fd);

    return 0;
}