#ifndef SEM_H
#define SEM_H

#include <pthread.h>
#include <semaphore.h>
using namespace std;

pthread_mutex_t the_mutex;  //互斥信号量，保护对buffer的插入和删除操作
sem_t buffer_empty;         //记录有多少空的缓存区
sem_t buffer_full;          //记录有多少满的缓存区

#endif