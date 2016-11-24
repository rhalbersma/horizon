/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


// try_socket.cpp : Defines the entry point for the console application.
//
#include "BoomstraDam.v1.2.h"
#include "winsock2.h"
#include "stdafx.h"

char msgbuf[128] = "";
int msgPnt = 0;
SOCKET ListenSocket;

void analyse_board_position(char * msgbuf)
{	compare_calc_move_with_played_move(msgbuf, 0);
}

void analyse_move(char * msgbuf)
{	compare_calc_move_with_played_move(msgbuf, 1);
}

void analyse_and_dispatch()
{	int sendBytes;
	printf("%s\n", msgbuf);
	switch (msgbuf[0])
	{	case 'I':
			init_game(msgbuf+1);
			break;
		case 'G':
			get_best_move(msgbuf+1);
			sendBytes = send(ListenSocket, result, result_length, 0);
			break;
		case 'B':
			get_move_from_opening_book(msgbuf+1);
			sendBytes = send(ListenSocket, result, result_length, 0);
			break;
		case 'E':
			evaluate_board_position(msgbuf+1);
			break;
		case 'A':
			//test_move_generator(msgbuf+1);
			analyse_board_position(msgbuf+1);
			break;
		case 'M':
			analyse_move(msgbuf+1);
			break;
		case 'T':
			analyse_timing(msgbuf+1);
			sendBytes = send(ListenSocket, result, result_length, 0);
			break;
		case 'R':
			create_final_report();
			break;
		default:
			printf("Engine -- Illegal message received\n");
			break;

	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	  //----------------------
	  // Initialize Winsock
	  WSADATA wsaData;
	  int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	  if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");
	  
	  //----------------------
	  // Create a SOCKET for listening for 
	  // incoming connection requests
	  ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	  if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 0;
	  }
	  //----------------------
	  // The sockaddr_in structure specifies the address family,
	  // IP address, and port for the socket that is being bound.
	  sockaddr_in service;
	  service.sin_family = AF_INET;
	  service.sin_addr.s_addr = inet_addr("127.0.0.1");
	  service.sin_port = htons(27016);

	  //----------------------
	  // Connect to the socket.
	  if (connect( ListenSocket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR) 
	  {
		printf("connect() failed.\n");
		closesocket(ListenSocket);
		return 0;
	  }
		
	 //----------------------
	  // Declare and initialize variables.
	  //int bytesSent;
	  int bytesRecv = 0;
	  char sendbuf[32] = "Client: Sending data.";
	  char recvbuf[128] = "";
	  int recPnt = 0;
	  int i;

	  //----------------------
	  // Receive data.
	  //bytesSent = send( ConnectSocket, sendbuf, strlen(sendbuf), 0 );
	  //printf( "Bytes Sent: %ld\n", bytesSent );

	  while( bytesRecv != SOCKET_ERROR ) {
	    printf("Engine -- waiting for message\n");
		bytesRecv = recv( ListenSocket, recvbuf, 128, 0 );
		if ( bytesRecv == 0 ) {
		  printf( "Connection Closed.\n");
		  break;
		}
		//
		// copy recvbuf to msgbuf till \0 or bytes received
		//
		for (i=0; i<bytesRecv; i++)
		{	if (!(msgbuf[msgPnt] = recvbuf[i]))
			{	analyse_and_dispatch();
				msgPnt = -1;						// reset message buffer
			}

			msgPnt++;
		}

		printf( "Bytes Recv: %ld\n", bytesRecv );
	  } // end receive loop

	  if ( bytesRecv == SOCKET_ERROR )
	  {
		 printf("recv failed: %d\n", WSAGetLastError());
	  }







	printf("Closing....\n");
	WSACleanup();
	return 0;
}

