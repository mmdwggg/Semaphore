#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 10 // ���建������СΪ 10

int buffer[BUFFER_SIZE]; // ���干������
int count = 0; // ��¼�������е����ݸ���

pthread_mutex_t mutex; // ���廥����
pthread_cond_t cond1, cond2; // �������������������ڽ���ͬ��

// �������̺߳���
void* producer(void* arg) {
    int item;
    while (1) {
        item = rand() % 1000; // �����������Ϊ����������
        pthread_mutex_lock(&mutex); // ����
        while (count == BUFFER_SIZE) { // �������������ȴ�����������
            pthread_cond_wait(&cond1, &mutex); // �ȴ��������� cond1
        }
        buffer[count++] = item; // ��������ӵ�������
        printf("Producer produced item %d\n", item);
        int i = 0;
        printf("item:");
        for (; i < count; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\n");
        pthread_cond_signal(&cond2); // ����֪ͨ��������
        pthread_mutex_unlock(&mutex); // ����
        sleep(2); // �ȴ�һ��ʱ����������һ������
    }
    pthread_exit(NULL);
}

// ������1�̺߳���
void* consumer1(void* arg) {
    int item;
    while (1) {
        pthread_mutex_lock(&mutex); // ����
        while (count == 0) { // ������Ϊ�գ��ȴ�����������
            pthread_cond_wait(&cond2, &mutex); // �ȴ��������� cond2
        }
        item = buffer[--count]; // �ӻ�������ȡ������
        printf("Consumer 1 consumed item %d\n", item);
        int i = 0;
        printf("item:");
        for (; i < count; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\n");
        pthread_cond_signal(&cond1); // ����֪ͨ��������
        pthread_mutex_unlock(&mutex); // ����
        sleep(5); // �ȴ�һ��ʱ����������һ������
    }
    pthread_exit(NULL);
}

// ������2�̺߳���
void* consumer2(void* arg) {
    int item;
    while (1) {
        pthread_mutex_lock(&mutex); // ����
        while (count == 0) { // ������Ϊ�գ��ȴ�����������
            pthread_cond_wait(&cond2, &mutex); // �ȴ��������� cond2
        }
        item = buffer[--count]; // �ӻ�������ȡ������
        printf("Consumer 2 consumed item %d\n", item);
        int i = 0;
        printf("item:");
        for (; i < count; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\n");
        pthread_cond_signal(&cond1); // ����֪ͨ��������
        pthread_mutex_unlock(&mutex); // ����
        sleep(5); // �ȴ�һ��ʱ����������һ������
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2, t3; // ���������߳�
    pthread_mutex_init(&mutex, NULL); // ��ʼ��������
    pthread_cond_init(&cond1, NULL); // ��ʼ���������� cond1
    pthread_cond_init(&cond2, NULL); // ��ʼ���������� cond2
    pthread_create(&t1, NULL, producer, NULL); // �����������߳�
    pthread_create(&t2, NULL, consumer1, NULL); // ����������1�߳�
    pthread_create(&t3, NULL, consumer2, NULL); // ����������2�߳�
    pthread_join(t1, NULL); // �ȴ��������߳̽���
    pthread_join(t2, NULL); // �ȴ�������1�߳̽���
    pthread_join(t3, NULL); // �ȴ�������2�߳̽���
    pthread_mutex_destroy(&mutex); // ���ٻ�����
    pthread_cond_destroy(&cond1); // ������������ cond1
    pthread_cond_destroy(&cond2); // ������������ cond2
    return 0;
}