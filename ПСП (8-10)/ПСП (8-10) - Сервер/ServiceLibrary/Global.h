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
	case WSAEINTR: text = "WSAEINTR - Работа функции прервана "; break;
	case WSAEACCES: text = "WSAEACCESS - Разрешение отвергнуто"; break;
	case WSAEFAULT: text = "WSAEFAULT - Ошибочный адрес"; break;
	case WSAEINVAL: text = "WSAEINVAL - Ошибка в аргументе"; break;
	case WSAEMFILE: text = "WSAEMFILE - Слишком много файлов открыто"; break;
	case WSAEWOULDBLOCK: text = "WSAEWOULDBLOCK - Ресурс временно недоступен"; break;
	case WSAEINPROGRESS: text = "WSAEINPROGRESS - Операция в процессе развития"; break;
	case WSAEALREADY: text = "WSAEALREADY - Операция уже выполняется"; break;
	case WSAENOTSOCK: text = "WSAENOTSOCK - Неправильно задан сокет"; break;
	case WSAEDESTADDRREQ: text = "WSAEDESTADDRREQ - Требуется адрес расположения"; break;
	case WSAEMSGSIZE: text = "WSAEMSGSIZE - Сообщение слишком длинное"; break;
	case WSAEPROTONOSUPPORT: text = "WSAEPROTONOSUPPORT - Протокол не поддерживается"; break;
	case WSAEISCONN: text = "WSAEISCONN - Сокет уже подключён"; break;
	case WSAESHUTDOWN: text = "WSAESHUTDOWN - Сокет завершил работу"; break;
	case WSA_NOT_ENOUGH_MEMORY: text = "WSA_NOT_ENOUGH_MEMORY - Не хватает памяти"; break;
	case WSASYSCALLFAILURE: text = "WSASYSCALLFAILURE - Аварийное завершение системного вызова"; break;
	default: text = "Что-то там"; break;

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

struct Contact         // элемент списка подключений       
{
	enum TE {               // состояние  сервера подключения  
		EMPTY,              // пустой элемент списка подключений 
		ACCEPT,             // подключен (accept), но не обслуживается
		CONTACT             // передан обслуживающему серверу  
	}    ;     // тип элемента списка подключений 
	enum ST {               // состояние обслуживающего сервера  
		WORK,               // идет обмен данными с клиентом
		ABORT,              // обслуживающий сервер завершился не нормально 
		TIMEOUT,            // обслуживающий сервер завершился по времени 
		FINISH              // обслуживающий сервер завершился  нормально 
	}      sthread; // состояние  обслуживающего сервера (потока)

	SOCKET      s;         // сокет для обмена данными с клиентом
	SOCKADDR_IN prms;      // параметры  сокета 
	int         lprms;     // длина prms 
	HANDLE      hthread;   // handle потока (или процесса) 
	HANDLE      htimer;    // handle таймера

	char msg[50];           // сообщение 
	char srvname[15];       //  наименование обслуживающего сервера 
	int type;

	Contact(TE t = EMPTY, const char* namesrv = "") // конструктор 
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

