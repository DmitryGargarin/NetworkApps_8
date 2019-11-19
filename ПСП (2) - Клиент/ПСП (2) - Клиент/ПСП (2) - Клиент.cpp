#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
#pragma warning( disable : 4996)
using namespace std;

string SetErrorMsgText(string msg, int errNum)
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

int main()


{
	setlocale(LC_ALL, "Russian");
	WSADATA wsaData;
	SOCKET cC; //служит для коннекта к серверу; все операции со стороны клиента проходят через него
	try
	{

		int count;
		//cout << "Сколько раз отправлять сообщение?\t"; cin >> count;
		//Старт клиента
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("WSAStartup", WSASYSCALLFAILURE);
		}
		//Подключение сокета
		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("Socket", WSAGetLastError());
		}


		//Коннект к серверу
		SOCKADDR_IN srv;
		srv.sin_family = AF_INET;
		srv.sin_port = htons(2000);
		srv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if (connect(cC, (sockaddr*)& srv, sizeof(srv)) == SOCKET_ERROR)
			throw SetErrorMsgText("Connect", WSAGetLastError());

		//Отправка сообщения
		char stop[1] = {'\0'};
		char bto[512] = "echo", bfrom[512]; //от сервера к клиенту
		char full[512];	
		int ito = 0;
		/*for (int i = 0; i < count; i++)
		{
			char numb[5];
			const char* temp = itoa(i, numb, 10);
			strcpy(full, bto);5
		*/	//strcat_s(full, temp);
			if ((ito = send(cC, (char*)& bto, sizeof(full), NULL)) == SOCKET_ERROR)
				throw SetErrorMsgText("Send", WSAGetLastError());
			cout << endl << "Отправлено" <<  endl; 

			//принятие сообщения
			if ((ito = recv(cC, (char*)& bfrom, sizeof(bfrom), NULL)) == SOCKET_ERROR)
				throw SetErrorMsgText("Recv", WSAGetLastError());
			cout << endl << "Получено: " << bfrom << endl;
		/*}*/
		/*if ((ito = send(cC, (char*)& stop, sizeof(stop), NULL)) == SOCKET_ERROR)
			throw SetErrorMsgText("Send", WSAGetLastError());*/

		//Закрытие сокета
		if (closesocket(cC) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("CloseSocket", WSAGetLastError());
		}
		//Выключение сервера
		if (WSACleanup() == SOCKET_ERROR)
		{
			throw SetErrorMsgText("WSACleanup", WSAGetLastError());
		};
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText << endl;
	}

	return 0;
}