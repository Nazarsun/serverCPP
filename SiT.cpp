#pragma comment(lib, "ws2_32.lib")

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <ctime>

int wsadata_handler()
{
	WSADATA WsaData;
	int err = WSAStartup(0x0101, &WsaData);
	if (err == SOCKET_ERROR) std::cout << "WSAStartup failed : " << GetLastError();
	return 0;
}

auto curr_time()
{
	auto start = std::chrono::system_clock::now();
	// Some computation here
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	//std::cout<<ctime(&end_time);

	return std::ctime(&end_time);
}

int main()
{
	int err = wsadata_handler();

	int port = 80;
	int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = INADDR_ANY;

	err = bind(s, (LPSOCKADDR)&sin, sizeof(sin));
	if (err < 0)
	{
		std::cout << "bind error:" << WSAGetLastError() << "\n";
		return EXIT_FAILURE;
	}
	err = listen(s, SOMAXCONN);
	std::cout << "listen: " << err<<"\n";


	int s1;
	SOCKADDR_IN from;
	int fromlen = sizeof(from);
	

	s1 = accept(s, (struct sockaddr*)&from, &fromlen);
	for (;;)
	{
		char buf[64] = "";
		char sbuf[64] = "";
		
		std::cout << "Server IP: " << inet_ntop(AF_INET, &(sin.sin_addr), buf, sizeof(buf)) << ":" << ntohs(sin.sin_port) << "\n";
		std::cout << "Client IP: " << inet_ntop(AF_INET, &(from.sin_addr), buf, sizeof(buf)) << ":" << ntohs(from.sin_port) << "\n";
		
		err = recv(s1, sbuf, sizeof(sbuf), 0);
		err = send(s1, curr_time(), 25, 0);		

		while (1)//good
		{
			err = recv(s1, sbuf, sizeof(sbuf), 0);

			if (sbuf[0] == 'Y')
			{
				break;
			}
			else if (sbuf[0] == 'N')
			{
				shutdown(s, SD_BOTH);
				closesocket(s);
				closesocket(s1);
				exit(0);
			}
		}

	}
}