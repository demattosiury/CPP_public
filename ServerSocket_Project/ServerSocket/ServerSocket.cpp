#include "pch.h"
#include "structures.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

//UC0001
extern "C" __declspec(dllexport) USHORT StartServer(USHORT port);
//UC0001
extern "C" __declspec(dllexport) void StopServer();

//UC0002, UC0003
extern "C" __declspec(dllexport) USHORT RunServer();

//UC0004
extern "C" __declspec(dllexport) USHORT ReceiveQuotesAndReturnSize(Quote quote);

//UC0004
extern "C" __declspec(dllexport) void ReturnQuotesData(char* buffer, size_t bufferSize);
/*
extern "C" __declspec(dllexport) void Ping();
extern "C" __declspec(dllexport) void SendQuote(const char* message);
*/


WSADATA wsaData;
SOCKET serverSocket = INVALID_SOCKET;
struct sockaddr_in serverAddr;

std::vector<SOCKET> client_sockets;
//std::vector<MqlRates> quotes;
std::vector<Quote> quotes;

USHORT StartServer(USHORT port) {


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return -1; // Erro ao inicializar o Winsock // Error initializing Winsock
	}

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Configura o socket como não-bloqueante // Configure the socket as non-blocking
	u_long mode = 1; // 1 para não-bloqueante // 1 to non-blocking
	ioctlsocket(serverSocket, FIONBIO, &mode);

	if (serverSocket == INVALID_SOCKET) {
		WSACleanup();
		return -2; // Erro ao criar socket // Error creating socket
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);

	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		closesocket(serverSocket);
		WSACleanup();
		return -3; // Erro ao vincular // Error linking
	}

	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		closesocket(serverSocket);
		WSACleanup();
		return -4; // Erro ao ouvir // Error listening
	}

	return 0; // Sucesso // Success
}

void StopServer() {
	closesocket(serverSocket); // Fecha o socket de escuta // Close the listening socket
	serverSocket = INVALID_SOCKET; // Definindo como inválido // Setting as invalid
	//WSACleanup(); // Fecha os socket do Client do MT5 também. // Close the MT5 Client sockets too.
}

USHORT RunServer() {
	SOCKET client_socket;
	struct sockaddr_in client;
	int clientAddrSize = sizeof(client);

	// Aceitar uma nova conexão // Accept a new connection
	client_socket = accept(serverSocket, (struct sockaddr*)&client, &clientAddrSize);
	if (client_socket != INVALID_SOCKET) {
		// Adicionar o socket do cliente ao vetor // Add the client socket to the array
		client_sockets.push_back(client_socket);
		//std::thread(HandleClient, client_socket).detach(); // Lidar com o cliente em uma nova thread // Deal with the client in a new thread
	}

	//UC0003
	for (SOCKET client : client_sockets) {
		char buffer[1];
		int result = recv(client, buffer, sizeof(buffer), MSG_PEEK);
		if (result == 0) {
			client_sockets.erase(
				std::remove(client_sockets.begin(), client_sockets.end(), client), client_sockets.end()
			);
		}
	}



	return client_sockets.size();

}

USHORT ReceiveQuotesAndReturnSize(Quote quote) {
	if (quotes.size() == 0) quotes.push_back(quote);
	return quotes.size();
}

/*
void ReturnQuotesData(char* buffer, size_t bufferSize) {
	std::string data = "data: ";
	for (Quote quote : quotes) {


		data += " , symb ";
		data += std::to_string(quote.symb);
		data += " , time ";
	data += std::to_string(quote.time);
	data += " close ";
	data += std::to_string(quote.close);
	data += " , ";
	data += std::to_string(quote.open);
	data += " , ";
	data += std::to_string(quote.high);
	data += " , ";
	data += std::to_string(quote.low);
	data += " , ";
	data += std::to_string(quote.real_volume);
	data += " , ";
	data += std::to_string(quote.spread);
	data += " , ";
	data += std::to_string(quote.tick_volume);
	}
	snprintf(buffer, bufferSize, "%s", data.c_str());
}
*/

/*
void SendQuote(const char* message) {
	for (SOCKET client : client_sockets) {
		if (send(client, message, strlen(message), 0) == SOCKET_ERROR) {
			// Fecha o socket do cliente quando a comunicação termina // Close the client socket when communication ends
			closesocket(client);

			// Remove o socket do vetor // Remove the socket from the array
			client_sockets.erase(
				std::remove(
					client_sockets.begin(),
					client_sockets.end(),
					client),
				client_sockets.end());
		}

	}
}



void Ping() {

	char ping[8] = { 'P','I','N','G','.','.','.','\0' };

	for (SOCKET client : client_sockets) {
		if (send(client, ping, 8, 0) == SOCKET_ERROR) {
			// Fecha o socket do cliente quando a comunicação termina // Close the client socket when communication ends
			closesocket(client);

			// Remove o socket do vetor // Remove the socket from the array
			client_sockets.erase(
				std::remove(
					client_sockets.begin(),
					client_sockets.end(),
					client),
				client_sockets.end());
		}
	}

}
*/
