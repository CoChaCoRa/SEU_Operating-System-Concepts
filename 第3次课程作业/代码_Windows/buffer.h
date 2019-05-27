#pragma once
#ifndef BUFFER_H
#define BUFFER_H

typedef int buffer_item;
#define BUFFER_SIZE 5

/*  buffer �Լ� ��buffer�Ĳ���
	bufferΪbuffer_item��������飬��ѭ������������
	��buffer�Ĳ���Ϊ���롢ɾ�����ֱ�����producer��consumer
*/
buffer_item buffer[BUFFER_SIZE];
int front = 0, rear = 0;     //ͷָ���βָ��
bool isFull = false;    //�����ж�buffer�Ƿ���

bool insert_item(buffer_item item) {
	//��buffer��λ����item
	//����ֵ������ɹ�����true������Ϊfalse
	if (isFull) {
		return false;
	}
	buffer[rear] = item;
	rear = (rear + 1) % BUFFER_SIZE;
	if (rear == front) {
		isFull = true;
	}
	return true;
}

bool remove_item(buffer_item *item) {
	//��buffer��ȡ��ͷ��item����������*item
	//����ֵ��ȡ���ɹ�����true������Ϊfalse
	if (front != rear || isFull) {
		*item = buffer[front];
		isFull = false;
		front = (front + 1) % BUFFER_SIZE;
		return true;
	}
	else return false;
}

#endif