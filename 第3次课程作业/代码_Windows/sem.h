#pragma once
#include<process.h>
#include<Windows.h>
#include"buffer.h"
using namespace std;

HANDLE the_mutex = CreateSemaphore(NULL, 1, 1, NULL);		//互斥信号量，保护对buffer的插入和删除操作
HANDLE buffer_empty = CreateSemaphore(NULL, BUFFER_SIZE, 1, NULL);	//记录有多少空的缓存区
HANDLE buffer_full = CreateSemaphore(NULL, 0, 1, NULL);		//记录有多少满的缓存区