#ifndef BUFFER_H
#define BUFFER_H

typedef int buffer_item;
#define BUFFER_SIZE 5

/*  buffer 以及 对buffer的操作
    buffer为buffer_item对象的数组，按循环队列来处理
    对buffer的操作为插入、删除，分别用于producer和consumer
*/
buffer_item buffer[BUFFER_SIZE];
int front=0,rear=0;     //头指针和尾指针
bool isFull = false;    //用于判断buffer是否满

bool insert_item(buffer_item item){
//向buffer空位插入item
//返回值：插入成功返回true，否则为false
    if(isFull){
        return false;
    }
    buffer[rear]=item;
    rear = (rear+1)%BUFFER_SIZE;
    if(rear==front){
        isFull=true;
    }
    return true;
}

bool remove_item(buffer_item *item){
//从buffer中取出头部item并赋给参数*item
//返回值：取出成功返回true，否则为false
    if(front != rear || isFull){
        *item = buffer[front];
        isFull = false;
        front = (front+1)%BUFFER_SIZE;
        return true;
    }
    else return false;
}

#endif