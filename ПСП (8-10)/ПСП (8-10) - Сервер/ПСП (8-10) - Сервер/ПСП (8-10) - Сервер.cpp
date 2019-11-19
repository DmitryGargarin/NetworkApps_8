// ПСП (8-10) - Сервер.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <WinSock2.h>
#include <windows.h>  
#include <ctime>
#include <list>
#pragma comment (lib, "WS2_32.lib")
#pragma warning (disable : 4996)

using namespace std;
int defaultPort = 2000; //дефолтный порт
//метод ошибок в WinSock
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

//команды работы с консолью
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
	};     // тип элемента списка подключений 
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

SOCKET sS;
CRITICAL_SECTION scListContact;
list<Contact> contacts;
HANDLE hAcceptServer, hDispathServer;

HMODULE dllModule;
HANDLE(*ts)(char*, LPVOID);

char bfrom[512] = "haptfu";
int ito = 0;

bool AcceptCycle(int squirt)
{
	bool rc = false;
	Contact c(Contact::ACCEPT, "EchoServer");
	SOCKET serv = sS;
	while (squirt-- > 0 && rc == false)
	{
		if ((c.s = accept(serv,
			(sockaddr*)& c.prms, &c.lprms)) == INVALID_SOCKET)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
				throw  SetErrorWinSockText("accept:", WSAGetLastError());
		}
		else
		{	
			cout << "--------------------------------------------------------------\nIP клиента: " << inet_ntoa(c.prms.sin_addr) << endl;
			cout << "Порт клиента: " << htons(c.prms.sin_port);
			rc = true;               // подключился
			EnterCriticalSection(&scListContact);
			contacts.push_front(c);
			LeaveCriticalSection(&scListContact);
			cout << "\nКол-во элементов списка подключений: " << contacts.size() << endl;
		}
	}
	return rc;
};
void CommandsCycle(TalkersCmd& cmd)
{
	while (cmd != EXIT)
	{
		int squirt = 10;
		switch (cmd)
		{
		case START:
		{
			cmd = GETCOMAND;
			squirt = 10;
			break;
		}
		case STOP:
		{
			cmd = GETCOMAND;
			squirt = 0;
			break;
		}
		case STATISTICS:
		{
			cmd = GETCOMAND;
			break;
		}
		case WAIT:
		{
			cmd = GETCOMAND;
			break;
		}
		case SHUTDOWN:
		{
			cmd = GETCOMAND;
			break;
		}
		default:
		{
			//cout << "Хаптьфу, бля, говно ебливое, не работает толком, иди под струю, бля.\n";
			cmd = GETCOMAND;
			break;
		}
		}
		if (AcceptCycle(squirt))   //цикл  запрос/подключение (accept) 
		{
			cmd = GETCOMAND;
			//.... запуск потока EchoServer.......................
			//.... установка ожидающего таймера для процесса EchoServer ...  
			//6.7.6
		}
		else SleepEx(0, TRUE);

	}
}

DWORD WINAPI AcceptServer(LPVOID cmdComand)
{
	DWORD rc = 0; int i = 0;
	cout << "AcceptServer-thread is in business!!\n";

	if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
	{
		throw SetErrorWinSockText("Socket", WSAGetLastError());
	}

	//привязка сокета к АйПи и порту
	SOCKADDR_IN serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(defaultPort);
	serv.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(sS, (LPSOCKADDR)& serv, sizeof(serv)) == SOCKET_ERROR)
		throw SetErrorWinSockText("Bind", WSAGetLastError());

	while (*(TalkersCmd*)cmdComand != EXIT)
	{
		//Включение прослушки
		if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
			throw SetErrorWinSockText("Listen", WSAGetLastError());

		u_long nonblk;
		if (ioctlsocket(sS, FIONBIO, &(nonblk = 1)) == SOCKET_ERROR)
			throw SetErrorWinSockText("ioctlsocket:", WSAGetLastError());
		CommandsCycle(*(TalkersCmd*)cmdComand);
	}
	if (closesocket(sS) == SOCKET_ERROR)
	{
		throw SetErrorWinSockText("CloseSocket", WSAGetLastError());
	}
	cout << "AcceptServer-thread is ended!!" << endl;
	ExitThread(rc);
}
DWORD WINAPI DispathServer(LPVOID cmdComand)
{
	DWORD rc = 0;
	cout << "DispathServer-thread is in business!!" << endl;
	while(*(TalkersCmd*)cmdComand != EXIT)
	{
		
		for (Contact &c : contacts)
		{
			
			if (c.type == (int)Contact::ACCEPT)
			{
				c.hthread = ts((char*)"Echo", (LPVOID) & (c));

				/*int lengthInputBuff = 0;
				char inputBuff[512];
				memset(inputBuff, 0, sizeof(inputBuff));

				if ((lengthInputBuff = recv(c.s, inputBuff, sizeof(inputBuff), NULL)) == SOCKET_ERROR)
					throw SetErrorWinSockText("Recv: ", WSAGetLastError());

				if (lengthInputBuff == 0)
					ExitThread(-1);

				char outputBuff[512];
				memset(outputBuff, 0, sizeof(outputBuff));
				strcpy(outputBuff, inputBuff);

				cout << "From " << inet_ntoa(c.prms.sin_addr) << " : " << inputBuff << endl;

				int lengthOutputBuff = 0;
				if ((lengthOutputBuff = send(c.s, outputBuff, sizeof(outputBuff), NULL) == SOCKET_ERROR))
					throw SetErrorWinSockText("Send: ", WSAGetLastError());*/
			}
		}
	}
	cout << "DispathServer-thread is ended!!" << endl;
	ExitThread(rc);
}

int main(int argc, TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	WSADATA wsaData;
	volatile TalkersCmd cmd = START;
	dllModule = LoadLibrary(L"ServiceLibrary");
	if (dllModule == NULL) cout << "Something goes wrong" << endl;
	ts = (HANDLE(*)(char*, LPVOID))GetProcAddress(dllModule, "SSS");

	//Старт сервера
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		throw SetErrorWinSockText("WSAStartup", WSASYSCALLFAILURE);
	}
	int mainPort;
	cout << "Установите номер порта (если нужно оставить порт по умолчанию, введите отрицательное значение):\t"; cin >> mainPort;
	if (mainPort >= 1 && mainPort <= 65535)
		defaultPort = mainPort;

	InitializeCriticalSection(&scListContact);

	hDispathServer = CreateThread(NULL, NULL, DispathServer, (LPVOID)& cmd, NULL, NULL);
	hAcceptServer = CreateThread(NULL, NULL, AcceptServer, (LPVOID)& cmd, NULL, NULL);

	WaitForSingleObject(hAcceptServer, INFINITE);
	WaitForSingleObject(hDispathServer, INFINITE);

	CloseHandle(hDispathServer);
	CloseHandle(hAcceptServer);

	//Выключение сервера
	if (WSACleanup() == SOCKET_ERROR)
	{
		throw SetErrorWinSockText("WSACleanup", WSAGetLastError());
	};

	FreeLibrary(dllModule);

	return 0;
}

