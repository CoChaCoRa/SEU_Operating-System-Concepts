#include<iostream>
#include<fstream>
#include<cstdlib>
using namespace std;

int main() {
	//定义char型数组存放源文件名
	char sourcefile[200];
	ifstream file_in;
	//用户输入源文件名/路径
	cout << "input the source file name or path（no more than 200 bytes）:" << endl;
	cin.getline(sourcefile, 200);
	//读取源文件数据
	file_in.open(sourcefile, ios::in);
	//错误处理机制1：源文件打开失败
	if (!file_in) {
		cerr << "source file open error!" << endl;
		exit(1);
	}
	//定义char型数组存放目的文件名
	char c;
	char destfile[200];
	cout << "input the destination file name or path（no more than 200 bytes）:" << endl;
	cin.getline(destfile, 200);
	//错误处理机制2：目的文件打开失败
	ofstream file_out(destfile, ios::out);
	if (!file_out) {
		cerr << "destination file open error!" << endl;
		exit(1);
	}
	//写入数据
	while (file_in.get(c)) {
		file_out.put(c);
		cout << c;
	}
	cout << endl;
	file_in.close();
	file_out.close();
	//system("pause");
}



