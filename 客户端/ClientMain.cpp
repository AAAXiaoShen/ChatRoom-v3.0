#include<iostream>
#include"ClientClass.h"
#pragma warning(disable:4996)
using namespace std;

//���ÿ���̨�Ĵ�С
void setsize(int col, int row) {
	char cmd[64];
	sprintf(cmd, "mode con cols=%d lines=%d", col, row);
	system(cmd);
}

int main() {

	//����̨��ز���
	SetConsoleTitle(LPCSTR("����������----�ͻ��˴�����"));

	setsize(70, 20);



	ClientClass Client;

	SOCKET ClntSock = Client.GetSocket();

	while (true) {
		Client.ConnectServer("192.168.181.1", 12306);

		Client.SendData();

		Client.RecvData();
	}
	system("pause");
	return 0;
}