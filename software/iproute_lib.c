/*
 * iproute_lib.c		"ip route shared library".
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	SMesh Creators
 *
 *
 * Description:
 * Allows you to call "ip route bla" directly from a C program for
 * modifying the routing tables on the fly. 
 *
 * Building:
 * To build, copy file into the directory with the iproute code
 * and create a shared library by adding to Makefile the following
 * $(CC) -shared -o iproute_lib.so iproute_lib.o iproute.o rtm_map.o ../lib/*.o 
 *
 * Usage: 
 * iproute("ip route add 10.0.1.2 table 99 nexthop via 10.0.22.44");
 * 
 * So if shared library is not present, the same command can be used on the shell :-)
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#include "SNAPSHOT.h"
#include "utils.h"
#include "ip_common.h"

#define MAX_ARGS 100 

int preferred_family = AF_UNSPEC;
int show_stats = 0;
int resolve_hosts = 0;
int oneline = 0;
int timestamp = 0;
char * _SL_ = NULL;
char *batch_file = NULL;
int force = 0;
struct rtnl_handle rth;

int iproute(char *cmd)
{
    int ret, items;
    char *str; 
    char *buff[MAX_ARGS];

    items=0;
    str = strtok(cmd, " ");
    while (str != NULL && items < MAX_ARGS) {
        buff[items++] = str;
        str = strtok(NULL, " ");
    }

    if (str != NULL) {
        return -1;
    }

	if (rtnl_open(&rth, 0) < 0) {
        return -1;
    }
	ret = do_iproute(items-2, buff+2);
	rtnl_close(&rth);
    return ret;
}

