#pragma once
#include<process.h>
#include<Windows.h>
#include"buffer.h"
using namespace std;

HANDLE the_mutex = CreateSemaphore(NULL, 1, 1, NULL);		//�����ź�����������buffer�Ĳ����ɾ������
HANDLE buffer_empty = CreateSemaphore(NULL, BUFFER_SIZE, 1, NULL);	//��¼�ж��ٿյĻ�����
HANDLE buffer_full = CreateSemaphore(NULL, 0, 1, NULL);		//��¼�ж������Ļ�����