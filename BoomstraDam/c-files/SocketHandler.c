/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include <winsock.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) 
{	int socket_desc;

	socket_desc = socket(AF_INET,SOCK_STREAM,0);
	if (socket_desc==-1)
    printf("Create socket error");

	return 1;
}
