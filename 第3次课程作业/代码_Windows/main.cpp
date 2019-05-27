#include<iostream>
#include<process.h>
#include<Windows.h>
#include"buffer.h"
#include"sem.h"
#include"PC.h"
using namespace std;

int main(){
	//create producer and consumer threads
	cout << "Creating producer and consumer threads..." << endl;
	HANDLE pro = CreateThread(NULL, 0, producer, NULL, 0, NULL);
	HANDLE con = CreateThread(NULL, 0, consumer, NULL, 0, NULL);
	//close threads
	cout << "Threads finished.\nClosing threads..." << endl;
	CloseHandle(pro);
	CloseHandle(con);

	system("pause");
	return 0;
}