#pragma once
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <WinSock2.h>
#pragma comment (lib, "WS2_32.lib")
#pragma warning (disable : 4996)

using namespace std;

string SetErrorWinSockText(string msg, int errNum)
{
	string text;
	switch (errNum)
	{
	case WSAEINTR: text = "WSAEINTR - ������ ������� �������� "; break;
	case WSAEACCES: text = "WSAEACCESS - ���������� ����������"; break;
	case WSAEFAULT: text = "WSAEFAULT - ��������� �����"; break;
	case WSAEINVAL: text = "WSAEINVAL - ������ � ���������"; break;
	case WSAEMFILE: text = "WSAEMFILE - ������� ����� ������ �������"; break;
	case WSAEWOULDBLOCK: text = "WSAEWOULDBLOCK - ������ �������� ����������"; break;
	case WSAEINPROGRESS: text = "WSAEINPROGRESS - �������� � �������� ��������"; break;
	case WSAEALREADY: text = "WSAEALREADY - �������� ��� �����������"; break;
	case WSAENOTSOCK: text = "WSAENOTSOCK - ����������� ����� �����"; break;
	case WSAEDESTADDRREQ: text = "WSAEDESTADDRREQ - ��������� ����� ������������"; break;
	case WSAEMSGSIZE: text = "WSAEMSGSIZE - ��������� ������� �������"; break;
	case WSAEPROTONOSUPPORT: text = "WSAEPROTONOSUPPORT - �������� �� ��������������"; break;
	case WSAEISCONN: text = "WSAEISCONN - ����� ��� ���������"; break;
	case WSAESHUTDOWN: text = "WSAESHUTDOWN - ����� �������� ������"; break;
	case WSA_NOT_ENOUGH_MEMORY: text = "WSA_NOT_ENOUGH_MEMORY - �� ������� ������"; break;
	case WSASYSCALLFAILURE: text = "WSASYSCALLFAILURE - ��������� ���������� ���������� ������"; break;
	default: text = "���-�� ���"; break;

	}
	return msg + ": " + text;
}

enum TalkersCmd
{
	START = 1,
	STOP = 2,
	EXIT = 3,
	STATISTICS = 4,
	WAIT = 5,
	SHUTDOWN = 6,
	GETCOMAND = 7
};

struct Contact         // ������� ������ �����������       
{
	enum TE {               // ���������  ������� �����������  
		EMPTY,              // ������ ������� ������ ����������� 
		ACCEPT,             // ��������� (accept), �� �� �������������
		CONTACT             // ������� �������������� �������  
	}    ;     // ��� �������� ������ ����������� 
	enum ST {               // ��������� �������������� �������  
		WORK,               // ���� ����� ������� � ��������
		ABORT,              // ������������� ������ ���������� �� ��������� 
		TIMEOUT,            // ������������� ������ ���������� �� ������� 
		FINISH              // ������������� ������ ����������  ��������� 
	}      sthread; // ���������  �������������� ������� (������)

	SOCKET      s;         // ����� ��� ������ ������� � ��������
	SOCKADDR_IN prms;      // ���������  ������ 
	int         lprms;     // ����� prms 
	HANDLE      hthread;   // handle ������ (��� ��������) 
	HANDLE      htimer;    // handle �������

	char msg[50];           // ��������� 
	char srvname[15];       //  ������������ �������������� ������� 
	int type;

	Contact(TE t = EMPTY, const char* namesrv = "") // ����������� 
	{
		memset(&prms, 0, sizeof(SOCKADDR_IN));
		lprms = sizeof(SOCKADDR_IN);
		type = t;
		strcpy(srvname, namesrv);
		msg[0] = 0;
	};

	void SetST(ST sth, const char* m = "")
	{
		sthread = sth;
		strcpy(msg, m);
	}
};

