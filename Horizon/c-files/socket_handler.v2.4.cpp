/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


// try_socket.cpp : Defines the entry point for the console application.
//
#include "BoomstraDam.v2.5.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close */
#include <memory.h>
#include <pthread.h>


char msgbuf[128] = "";
int sendBytes;
int msgPnt = 0;
int ListenSocket;
struct hostent *h;
bool surpress_message = false;

void * analyse_board_position(void *msgbuf)
{	external_stop = 0; // reset external stop
	get_all_moves_with_value((char *)msgbuf);
	sendBytes = send(ListenSocket, result, result_length, 0);
	pthread_exit(0);
}

void analyse_move(char * msgbuf)
{
}

int analyse_and_dispatch()
/*****************************************************************
 * used codes:
 * 'A' get all moves with their values
 * 'B' get move from opening book
 * 'C' find hash entry from position
 * 'D' find hash entry hash_key
 * 'E' evaluate board position (test)
 * 'F' get best move fixed depth
 * 'G' get best move (this is the competition driver)
 * 'H' setup hash
 * 'I' init_game
 * 'K' follow_up_position
 * 'L' evaluate board position (2)
 * 'M' analyze move (obsolete)
 * 'T' analyze timing
 * 'P' ponder
 * 'Q' quit engine
 * 'R' create final report (for timing)
 * 'S' set stop flag
 */
{
	//pthread_t p_thread;
	//printf("%s\n", msgbuf);
	external_stop = 0; // reset external stop
	surpress_message = false;
	switch (msgbuf[0])
	{	case 'I':
			init_game(msgbuf+1);
			printf("engine -- init game finished\n");
			break;
		case 'G':
			get_best_move(msgbuf+1);
			sendBytes = send(ListenSocket, result, strlen(result)+1, 0);
			break;
/*		case 'B':
			get_move_from_opening_book(msgbuf+1);
			printf("engine -- send move from opening book: %s, len= %d\n", result, strlen(result)+1);
			sendBytes = send(ListenSocket, result, strlen(result)+1, 0);
			break;
*/		case 'E':
			evaluate_board_position(msgbuf+1);
			//count_moves_at_level(msgbuf+1);
			sendBytes = send(ListenSocket, result, strlen(result) + 1, 0);
			break;
		case 'K':
			follow_up_position(msgbuf+1);
			break;
		case 'L':
			evaluate_board_position_2(msgbuf+1);
			sendBytes = send(ListenSocket, result, strlen(result) + 1, 0);
			break;
		case 'A':
			external_stop = 0; // reset external stop
			get_all_moves_with_value((char *)(msgbuf+1));
			sendBytes = send(ListenSocket, result, strlen(result)+1, 0);
			//printf("sendBytes = %d, result_length = %d, strlen = %d\n", sendBytes, result_length, strlen(result));

			break;
		case 'M':
			analyse_move(msgbuf+1);
			break;
		case 'H':
			setup_hash(msgbuf+1);
			surpress_message = true;
			break;
		case 'C':
			find_hash_position(msgbuf+1);
			sendBytes = send(ListenSocket, result, strlen(result) + 1, 0);
			surpress_message = true;
			break;
		case 'D':
			find_hash_hashkey(msgbuf+1);
			sendBytes = send(ListenSocket, result, strlen(result) + 1, 0);
			surpress_message = true;
			break;
/*		case 'T':
			analyse_timing(msgbuf+1);
			sendBytes = send(ListenSocket, result, strlen(result)+1, 0);
			break;
*/		case 'R':
			create_final_report();
			sendBytes = send(ListenSocket, result, strlen(result)+1, 0);
			break;
		case 'S':
			stop_pondering();
			break;
		case 'F':
			printf("Engine -- Get best move fixed depth\n");
			get_best_move_fixed_depth(msgbuf+1);
			sendBytes = send(ListenSocket, result, strlen(result)+1, 0);
			break;
		case 'P':
			//printf("Engine -- Start Pondering\n");
			ponder(msgbuf+1); // startup the ponder thread and return to the wait loop
			break;
		case 'Q':		// quit
			printf("Engine -- Quit received\n");
			return 0;
		default:
			printf("Engine -- Illegal message received\n");
			return 0;

	}
	return 1;
}


//int main(int argc, char* argv[])
int main()
{
	 	int retry_count = 0;


	h = gethostbyname("127.0.0.1");
	if(h==NULL) {
	printf("unknown host '%s'\n","127.0.0.1");
	return 0;
	}

	  //----------------------
	  // Create a SOCKET for listening for
	  // incoming connection requests
	  ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	  if (ListenSocket < 0) {
		printf("Error at socket(): %d\n", ListenSocket);
		return 0;
	  } else printf("engine -- socket created\n");
	  //----------------------
	  // The sockaddr_in structure specifies the address family,
	  // IP address, and port for the socket that is being bound.
	  struct sockaddr_in service;
	  service.sin_family = AF_INET;
	  memcpy((char *) &service.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
	  service.sin_port = htons(27013);

	  //----------------------
	  // Connect to the socket.
retry_connect:
	  if (connect( ListenSocket, (struct sockaddr *) &service, sizeof(service)) < 0)
	  {
		printf("engine -- connect() failed.\n");
		if (retry_count > 3) return 0;
		sleep(1);
		retry_count++;
		// retry
		goto retry_connect;
		//closesocket(ListenSocket);
		return 0;
	  } else printf("engine -- socket connected\n");

	 //----------------------
	  // Declare and initialize variables.
	  //int bytesSent;
	  int bytesRecv = 0;

	  char recvbuf[128] = "";

	  int i;

	  //----------------------
	  // Receive data.
	  //bytesSent = send( ConnectSocket, sendbuf, strlen(sendbuf), 0 );
	  //printf( "Bytes Sent: %ld\n", bytesSent );

	  while(1) {
	    //if (surpress_message == false)
	    printf("engine -- waiting for message\n");
		bytesRecv = read( ListenSocket, recvbuf, 128);
		if ( bytesRecv == 0 ) {
		  printf( "engine -- Connection Closed by GUI.\n");
		  break;
		}
		if ( bytesRecv < 0 ) {
		  printf( "Error in Connection.\n");
		  break;
		}
		//
		// copy recvbuf to msgbuf till \0 or bytes received
		//
		for (i=0; i<bytesRecv; i++)
		{	if (!(msgbuf[msgPnt] = recvbuf[i])) {
				if (analyse_and_dispatch() != 0){
					msgPnt = -1;						// reset message buffer
				}
				else {
					goto close_engine;
				}
			}

			msgPnt++;
		}

		//printf( "Bytes Recv: %d\n", bytesRecv );
	  } // end receive loop

	  if ( bytesRecv < 0 )
	  {
		 printf("engine -- recv failed: %d\n", bytesRecv);
	  }





close_engine:


	printf("engine -- Closing....\n");
	close(ListenSocket);
	exit(0);
	return 0;
}

