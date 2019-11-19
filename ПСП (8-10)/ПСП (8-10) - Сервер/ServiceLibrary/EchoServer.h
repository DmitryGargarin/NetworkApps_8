#pragma once
#include "windows.h"
#include "Global.h"

DWORD WINAPI EchoServer(LPVOID lParam)
{
	DWORD rc = 0;
	Contact cont = *(Contact*)lParam;
	cont.type = (int)Contact::CONTACT;
	
	char bfrom[512];
	memset(bfrom, 0, sizeof(bfrom));
	int ito = 0;
	if ((ito = recv(cont.s, bfrom, sizeof(bfrom), NULL)) == SOCKET_ERROR)
		throw SetErrorWinSockText("Recv", WSAGetLastError());
	
	cout << endl << "Получено: " << bfrom << endl;

	//Отправка сообщения
	if ((ito = send(cont.s, bfrom, sizeof(bfrom), NULL)) == SOCKET_ERROR)
		throw SetErrorWinSockText("Send", WSAGetLastError());
	cout << endl << "Отправлено" << endl;
	ExitThread(rc);





	/*DWORD rc = 0;

	Contact c = *(Contact*)lParam;

	c.type = (int)Contact::CONTACT;

	int lengthInputBuff = 0;
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
		throw SetErrorWinSockText("Send: ", WSAGetLastError());

	ExitThread(rc);*/
}
