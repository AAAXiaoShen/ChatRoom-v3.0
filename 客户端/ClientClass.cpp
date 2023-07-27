#include "ClientClass.h"
#pragma warning(disable:4996)
#include<windows.h>
using namespace std;




void ShowRoom(ClientClass& Client) {
	MsgType* msgtype;//��Ϣ����

	MsgShow msgshow;
	msgtype = &msgshow;
	send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//����Ϣ���������

}

void CreateRoom(ClientClass& Client) {
	MsgType* msgtype;//��Ϣ����
	string roomid;//�����
	string name;//�ǳ�

	cout << "�����뷿��ţ�" << endl;
	cin >> roomid;
	cout << "���������ķ����Ϊ��" << roomid << endl;
	Client.SetRoomID(roomid);

	//�����ǳƣ����ÿͻ��˵��ǳ�
	cout << "�������ǳƣ�" << endl;
	
	cin >> name;
	Client.SetName(name);

	MsgCreate msgcreate(roomid, name);
	msgtype = &msgcreate;//����ָ��ָ������

	send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//����Ϣ���������
}

void JoinRoom(ClientClass& Client) {
	MsgType* msgtype;//��Ϣ����	
	string roomid;//�����
	string name;//�ǳ�

	//���ÿͻ��˵����������
	cout << "�����뷿��ţ�" << endl;
	cin >> roomid;
	Client.SetRoomID(roomid);

	//���ÿͻ��˵��ǳ�
	cout << "�������ǳƣ�" << endl;
	cin >> name;
	Client.SetName(name);

	MsgJoin msgjoin(roomid, name);//����join��
	msgtype = &msgjoin;

	send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//����Ϣ���������
}

bool IsLeave(ClientClass& Client, char* buff) {
	if (strcmp(buff, "//leave") == 0) {//�������//leave,���뿪����
		MsgType* msgtype;//��Ϣ����
		MsgLeave msgleave(Client.GetRoomID(), Client.GetName());
		msgtype = &msgleave;
		send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//����Ϣ���������
		return 1;
	}
	return 0;
}

void Chat(ClientClass& Client) {

	char buff[256];

	while (!IsLeave(Client, buff)) {

		memset(buff, 0, sizeof(buff));

		cout << "�����룺" << endl;
		cin >> buff;

		send(Client.GetSocket(), buff, strlen(buff), NULL);//����Ϣ��������
	}
}

void Send(LPVOID lp) {

	ClientClass Client = *(ClientClass*)lp;//�ͻ��˶���

	while (true) {

		char buff[256];

		cout << "+------------------------------------------------+" << endl;
		cout << "+        ��ӭ����C�Ӽ��ֻ���������               +" << endl;
		cout << "+        1.����show�鿴����������                +" << endl;
		cout << "+        2.����join�����������                  +" << endl;
		cout << "+        3.����create����������                  +" << endl;
		cout << "+        4.����exit�˳�����                      +" << endl;
		cout << "+------------------------------------------------+" << endl;
		cout << endl;

		cout << "������ָ�" << endl;
		cin >> buff;

		//��������
		if (strcmp(buff, "create") == 0) {

			CreateRoom(Client);
			
			Chat(Client);

			
		}
		
		//���뷿��
		else if (strcmp(buff, "join") == 0) {

			JoinRoom(Client);
			
			Chat(Client);

		}

		//��ʾ���з���
		else if (strcmp(buff, "show") == 0) {

			ShowRoom(Client);

		}

		//�˳�����
		else if (strcmp(buff, "exit") == 0) {
			exit(0);
		}
	}
	
}

void ClientClass::SendData() {

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Send, (LPVOID)this, 0, 0);

}

void ClientClass::RecvData() {
	char buff[256];

	int n = 0;

	int Strlen;

	initgraph(300, 400, 1);//������Ϣ�Ĵ���

	while (true) {
		Strlen = recv(ClntSock, buff, sizeof(buff) - 1, NULL);
		if (Strlen > 0) {
			//��ʾ��ͼ�ν��洰��
			buff[Strlen] = 0;
			outtextxy(1, n * 20, buff);
			n++;
			/*
				�����¼���浽�ĵ�
			*/
		}

		/*
			����յ���������ʧ�ܻ���ȡ�ǳ�ʧ�ܣ�����ô����?
		*/
	}

}




ClientClass::ClientClass() {
	//���������׽���
	ClntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//����TCP�׽���
	if (ClntSock == SOCKET_ERROR) {
		cout << "�����׽��ִ���ʧ��" << endl;
		return;
	}
	cout << "�����׽��ִ����ɹ�" << endl;

}

ClientClass::~ClientClass() {
	closesocket(ClntSock);
}



void ClientClass::ConnectServer(const char* ip, unsigned short port) {//���ӷ�����

	sockaddr_in sock_in;
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(port);
	sock_in.sin_addr.S_un.S_addr = inet_addr(ip);

	if (connect(ClntSock, (const sockaddr*)&sock_in, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		cout << "���ӷ�����ʧ��" << GetLastError() << endl;
		return;
	}

	cout << "�����˽�������" << endl;

}

SOCKET ClientClass::GetSocket() {
	return ClntSock;
}

void ClientClass::SetRoomID(string id) {
	m_RoomID = id;
}

string ClientClass::GetRoomID() {
	return m_RoomID;
}

void ClientClass::SetName(string name) {
	m_Name = name;
}

string ClientClass::GetName() {
	return m_Name;
}