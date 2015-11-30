#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
// Идентификаторы объектов-событий, которые используются
// для синхронизации задач, принадлежащих разным процессам
using namespace std;
//канал
HANDLE hPipe;

LPCSTR myPipe =
"\\\\.\\pipe\\Pipe";



int _tmain()
{
	DWORD mist;
	//DWORD dwRetCode;
	BOOL  fConnected;
	BOOL fSuccess;
	DWORD variable = 0;

	hPipe = CreateNamedPipe(
		(LPCSTR)myPipe, //имя
		PIPE_ACCESS_DUPLEX, //только читает
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		255,
		255,
		0,
		NULL);

	mist = GetLastError();
	if (GetLastError() != 0)
		return -1;

	if (hPipe == NULL)
	{
		fprintf(stdout, "CreatePipe: Error %ld\n",
			GetLastError());
		_getch();
		return 0;
	}


	fConnected = ConnectNamedPipe(hPipe, NULL);
	if (GetLastError() != 0)
		return -1;
	// Цикл отображения.
	char str[100];
	BOOL Success;
	DWORD byteCnt = sizeof (str), byteWrt;
	while (TRUE)
	{

		fSuccess = ReadFile(
			hPipe,        // handle to pipe 
			str,    // buffer to receive data 
			sizeof(str), // size of buffer 
			&variable, // number of bytes read 
			NULL);        // not overlapped I/O 
		if (!fSuccess)
		{
			if (GetLastError() == ERROR_BROKEN_PIPE)
			{
				_tprintf(TEXT("InstanceThread: client disconnected.\n"), GetLastError());
			}
			else
			{
				_tprintf(TEXT("InstanceThread ReadFile failed, GLE=%d.\n"), GetLastError());
			}
			break;
		}

		Success = WriteFile(hPipe, str, byteCnt, &byteWrt, NULL); //Пишем нашУ строку в канал hPipe
		FlushFileBuffers(hPipe);
		puts(str);

		//релизмьютекс
	}
	int x;
	cin >> x;
	CloseHandle(hPipe);
	return 0;
}