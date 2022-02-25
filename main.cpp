#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "ws2_32.lib")


struct NumberPacket
{
	int Number1;
	int Number2;
};

int main()
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "WinsockError : " << GetLastError();
		exit(-1);
	}

	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "Socket Error : " << GetLastError();
		exit(-1);
	}

	SOCKADDR_IN ServerAddr;
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(50000);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "bind Error : " << GetLastError();
		exit(-1);
	}


	if (listen(ServerSocket, 0) == SOCKET_ERROR)
	{
		cout << "listen Error : " << GetLastError();
		exit(-1);
	}

	while (true)
	{
		SOCKADDR_IN ClientAddr;
		int ClientAddrSize = sizeof(ClientAddr);
		SOCKET ClientSocket;
		ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClientAddrSize);
		if (ClientSocket == SOCKET_ERROR)
		{
			cout << "accept Error : " << GetLastError();
			exit(-1);
		}

		cout << "connect ip : " << inet_ntoa(ClientAddr.sin_addr);
		cout << "connect port : " << ntohs(ClientAddr.sin_port);;

		char Buffer[1024];
		recv(ClientSocket, Buffer, 1024, 0);

		string Numbers = Buffer;
		string Number1 = Numbers.substr(0, 2);
		string Number2 = Numbers.substr(3, 2);

		cout << "client Sended : " << Number1;
		cout << "client Sended : " << Number2;

		int IntResult = stol(Number1) + stol(Number2);

		string Result = to_string(IntResult);

		send(ClientSocket, Result.c_str(), Result.length() + 1, 0);

		cout << "client send : " << Result;


		closesocket(ClientSocket);
	}
	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}

