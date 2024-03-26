#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 10 // 定义缓冲区大小为 10

int buffer[BUFFER_SIZE]; // 定义共享缓冲区
int count = 0; // 记录缓冲区中的数据个数

pthread_mutex_t mutex; // 定义互斥锁
pthread_cond_t cond1, cond2; // 定义两个条件变量用于进程同步

// 生产者线程函数
void* producer(void* arg) {
    int item;
    while (1) {
        item = rand() % 1000; // 生成随机数作为生产的数据
        pthread_mutex_lock(&mutex); // 加锁
        while (count == BUFFER_SIZE) { // 缓冲区已满，等待消费者消费
            pthread_cond_wait(&cond1, &mutex); // 等待条件变量 cond1
        }
        buffer[count++] = item; // 将数据添加到缓冲区
        printf("Producer produced item %d\n", item);
        int i = 0;
        printf("item:");
        for (; i < count; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\n");
        pthread_cond_signal(&cond2); // 发送通知给消费者
        pthread_mutex_unlock(&mutex); // 解锁
        sleep(2); // 等待一段时间再生产下一个数据
    }
    pthread_exit(NULL);
}

// 消费者1线程函数
void* consumer1(void* arg) {
    int item;
    while (1) {
        pthread_mutex_lock(&mutex); // 加锁
        while (count == 0) { // 缓冲区为空，等待生产者生产
            pthread_cond_wait(&cond2, &mutex); // 等待条件变量 cond2
        }
        item = buffer[--count]; // 从缓冲区中取出数据
        printf("Consumer 1 consumed item %d\n", item);
        int i = 0;
        printf("item:");
        for (; i < count; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\n");
        pthread_cond_signal(&cond1); // 发送通知给生产者
        pthread_mutex_unlock(&mutex); // 解锁
        sleep(5); // 等待一段时间再消费下一个数据
    }
    pthread_exit(NULL);
}

// 消费者2线程函数
void* consumer2(void* arg) {
    int item;
    while (1) {
        pthread_mutex_lock(&mutex); // 加锁
        while (count == 0) { // 缓冲区为空，等待生产者生产
            pthread_cond_wait(&cond2, &mutex); // 等待条件变量 cond2
        }
        item = buffer[--count]; // 从缓冲区中取出数据
        printf("Consumer 2 consumed item %d\n", item);
        int i = 0;
        printf("item:");
        for (; i < count; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\n");
        pthread_cond_signal(&cond1); // 发送通知给生产者
        pthread_mutex_unlock(&mutex); // 解锁
        sleep(5); // 等待一段时间再消费下一个数据
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2, t3; // 定义三个线程
    pthread_mutex_init(&mutex, NULL); // 初始化互斥锁
    pthread_cond_init(&cond1, NULL); // 初始化条件变量 cond1
    pthread_cond_init(&cond2, NULL); // 初始化条件变量 cond2
    pthread_create(&t1, NULL, producer, NULL); // 创建生产者线程
    pthread_create(&t2, NULL, consumer1, NULL); // 创建消费者1线程
    pthread_create(&t3, NULL, consumer2, NULL); // 创建消费者2线程
    pthread_join(t1, NULL); // 等待生产者线程结束
    pthread_join(t2, NULL); // 等待消费者1线程结束
    pthread_join(t3, NULL); // 等待消费者2线程结束
    pthread_mutex_destroy(&mutex); // 销毁互斥锁
    pthread_cond_destroy(&cond1); // 销毁条件变量 cond1
    pthread_cond_destroy(&cond2); // 销毁条件变量 cond2
    return 0;
}