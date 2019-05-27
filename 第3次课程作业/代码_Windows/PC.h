#pragma once
#include<stdlib.h>
#include<iostream>
#include"buffer.h"
#include"sem.h"
using namespace std;

//生产者线程
//为便于debug，控制while循环次数为10，消费者线程同样
DWORD WINAPI producer(PVOID pvPram) {
	int i = 0;
	while (i < 10) {
		WaitForSingleObject(buffer_empty, INFINITE);
		WaitForSingleObject(the_mutex, INFINITE);

		//add item to buffer
		buffer_item item = rand();
		if (insert_item(item)) {
			cout << "producer produced item " << item << endl;
		}
		else cout << "report error condition: buffer is full!" << endl;

		ReleaseSemaphore(the_mutex, 1, NULL);
		ReleaseSemaphore(buffer_full, 1, NULL);//full+1

		i++;
	}

	return 0;
}

//消费者线程
DWORD WINAPI consumer(PVOID pvPram) {
	int j = 0;
	while (j < 10) {
		WaitForSingleObject(buffer_full, INFINITE);
		WaitForSingleObject(the_mutex, INFINITE);

		//remove item from buffer
		buffer_item item;
		if (remove_item(&item)) {
			cout << "consumer consumed item " << item << endl;
		}
		else cout << "report error condition： buffer is empty!" << endl;

		ReleaseSemaphore(the_mutex, 1, NULL);
		ReleaseSemaphore(buffer_empty, 1, NULL);//full+1

		j++;
	}

	return 0;
}