#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include"buffer.h"
#include"sem.h"
using namespace std;

//生产者线程
//为便于debug，控制while循环次数为10，消费者线程同样
void *producer(void *param){
    int i = 0;
    while(i<10){
        sem_wait(&buffer_empty);
        pthread_mutex_lock(&the_mutex);

        //add item to buffer
        buffer_item item = rand();
        if(insert_item(item)){
            cout<<"producer produced item "<<item<<endl;
        }
        else cout<<"report error condition: buffer is full!"<<endl;

        pthread_mutex_unlock(&the_mutex);
        sem_post(&buffer_full);
        
        i++;
        sleep(1);
    }
}

//消费者线程
void *consumer(void *param){
    int j = 0;
    while(j<10){
        sem_wait(&buffer_full);
        pthread_mutex_lock(&the_mutex);

        //remove item from buffer
        buffer_item item;
        if(remove_item(&item)){
            cout<<"consumer consumed item "<<item<<endl;
        }
        else cout<<"report error condition： buffer is empty!"<<endl;

        pthread_mutex_unlock(&the_mutex);
        sem_post(&buffer_empty);
        
        j++;
        sleep(1);
    }
}