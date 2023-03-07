/*
 Copyright (c) 1986, 1993
 The Regents of the University of California.  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. All advertising materials mentioning features or use of this software
    must display the following acknowledgement:
 This product includes software developed by the University of
 California, Berkeley and its contributors.
 4. Neither the name of the University nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

	Modifications to make this build & run on Linux by John Winans, 2021
*/

//******************************************************************************
//
//  CSCI463 - Assignment 7
//
//  Author: Hung Nguyen (Z1924897)
//
//  IPC
//  (This program uses the modification made by John Winans, 2021)
//
//******************************************************************************
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/uio.h>

/**
 * Verify the command-line. 
 * Print an "Usage" error message and terminates the program if it doesn't meet 
 * input requirements. 
 ******************************************************************************/
static void usage()
{
	std::cerr << "Usage: client [-s server-ip] server-port" << std::endl;
	std::cerr << "    -s server-ip: Specifies the server's IPv4 format (default" 
      << " 127.0.0.1)." << std::endl;
	std::cerr << "    server-port: The server port number to connect." 
      << std::endl;
	exit(1);
}

/**
 * Attempts to write len byte of data to the object referenced by the
 * descriptor fd from the buffer pointed to by buf.
 * @param fd object descriptor
 * @param buf pointer to buffer
 * @param len amount of bytes attempt to write
 ******************************************************************************/
static ssize_t safe_write(int fd, const char *buf, size_t len)
{
    while (len > 0)
    {
        ssize_t wlen = write(fd, buf, len);
        if (wlen == -1) return -1;
        len -= wlen;    // reduce the remaining number of bytes to send
        buf += wlen;    // advance the buffer pointer past the written data
    }
    return len; // get here if full requested length is sent
}

/*
 * Creates a socket and initiates a connection with the socket
 * given in the command line
 ******************************************************************************/
int main(int argc, char *argv[])
{
    int opt;
    bool s_flag = false;
    int sock;
    struct sockaddr_in server;

    while ((opt = getopt(argc, argv, "s:")) != -1)
    {
        switch (opt)
        {
            case 's':
            {
                s_flag = true;
                if (inet_pton(AF_INET, optarg, &server.sin_addr) <= 0)
                    {
                        std::cerr << optarg << ": invalid address/format" <<
                          std::endl;
                        exit(2);
                    }
                break;
            }
            case '?':
            default: usage();
        }
    }

    // Read raw bytes into character array buffer from stdin
    char buf[1024]; 
    ssize_t rval;
    ssize_t readin = 0;

    // Read in data from file
    do
    {
        if ((rval = read(fileno(stdin), buf, sizeof(buf))) < 0)
        {
            perror("reading stdin");
            exit(-1);
        }
        else readin += rval;
    } while (rval != 0);
    
    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
	    perror("opening stream socket");
	    exit(1);
    }
    /* Connect socket using name specified by command line. */
    server.sin_family = PF_INET;   // IP version 4

    if (s_flag == false)
    {
        char default_addr[] = "127.0.0.1"; 
        if (inet_pton(AF_INET, default_addr, &server.sin_addr) <= 0)
        {
            std::cerr << default_addr << ": invalid address/format" <<
              std::endl;
            exit(2);
        }
    }
    
    // Get port numner from user input
    server.sin_port = htons(atoi(argv[optind]));

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) 
    {
	    perror("connecting stream socket");
	    exit(1);
    }
    
    if (safe_write(sock, buf, readin) < 0)
    {
	    perror("writing on stream socket");
	    exit(1);
    }
    
    // half-close the socket. Let the server knows there will be no more data
    shutdown(sock, SHUT_WR);

    // Receive the message back from the server
    char buffer[1024];
    int received = 0;
    if ((received = read(sock, buffer, 1024)) < 0) {
        perror("read");
        exit(1);
    }
    buffer[received] = '\0';
    std::cout << buffer << std::endl;

    close(sock);
    return 0;
}
