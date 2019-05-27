#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include "buffer.h"
#include "sem.h"
#include "PC.h"
using namespace std;

int main(){
    //initialize semaphores
    cout<<"Initializing semaphores..."<<endl;
    pthread_mutex_init(&the_mutex, 0);
    sem_init(&buffer_full,0,0);
    sem_init(&buffer_empty,0,BUFFER_SIZE);
    cout<<"semaphores initialized!\n"<<endl;
    //create producer and consumer threads
    cout<<"Creating producer and consumer threads..."<<endl;
    pthread_t pro,con;
    pthread_create(&pro, 0, producer, 0);
    pthread_create(&con, 0, consumer, 0);
    //callback producer and consumer threads
    pthread_join(pro,0);
    pthread_join(con,0);
    //destroy mutex
    pthread_mutex_destroy(&the_mutex);
    cout<<"Threads finished.\nMutex destroyed."<<endl;

    cout<<"\nProgram finished!"<<endl;
    return 0;    
}