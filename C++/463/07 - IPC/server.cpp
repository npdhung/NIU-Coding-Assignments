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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <signal.h>
#include <sstream>

/**
 * Verify the command-line. 
 * Print an "Usage" error message and terminates the program if it doesn't meet 
 * input requirements. 
 ******************************************************************************/
static void usage()
{
	std::cerr << "Usage: server [-l listener-port]" << std::endl;
	std::cerr << "    listener-port: The port number to which the server must " 
     << "listen" << std::endl;
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
 * This program creates a server socket and then begins an infinite loop. Accept
 * message from a client and send back another message to the client.
 ******************************************************************************/
int main(int argc, char *argv[])
{
   int opt;
   bool l_flag = false;
   int sock;
	socklen_t length;
	struct sockaddr_in server;
	int msgsock;
	char buf[1024];
	int rval;
   int total_count = 0;	// Store the total byte count

   signal(SIGPIPE, SIG_IGN);

   while ((opt = getopt(argc, argv, "l:")) != -1)
   {
      switch (opt)
      {
         case 'l':
         {
            l_flag = true;
            server.sin_port = htons(atoi(optarg));
         }
         break;
         default:
        usage();
      }
   }

   /* Create socket */
   sock = socket(AF_INET, SOCK_STREAM, 0);
   if (sock < 0) 
   {
	   perror("opening stream socket");
	   exit(1);
   }
   
   /* Name socket using wildcards */
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   if (!l_flag) server.sin_port = 0;
     
   if (bind(sock, (sockaddr*)&server, sizeof(server))) 
   {
	   perror("binding stream socket");
	   exit(1);
   }
   
   /* Find out assigned port number and print it out */
   length = sizeof(server);
   if (getsockname(sock, (sockaddr*)&server, &length)) 
   {
	   perror("getting socket name");
	   exit(1);
   }
   std::cout << "Socket has port #" << ntohs(server.sin_port) << std::endl;

   /* Start accepting connections */
   if (listen(sock, 5) < 0)
   {
      perror("listen");
      exit(1);
   };
   
   do 
   {
	   // used to display the address of the connection peer
      struct sockaddr_in from;
      socklen_t from_len = sizeof(from);
      msgsock = accept(sock, (struct sockaddr*)&from, &from_len);
      
      total_count = 0;	
      uint8_t buffer[1024];
      uint32_t byte_count;
      uint16_t checksum = 0; //To accumulate checksum of each byte received
	   
      if (msgsock == -1)
	      perror("accept");
	   else 
      {
         // ntop: number to presentation
         inet_ntop(from.sin_family, &from.sin_addr, buf, sizeof(buf));
         std::cout << "Accept connection from " << buf << ", port " << 
           ntohs(from.sin_port) << std::endl;
         
         // Start accepting data
         do 
         {
            if ((rval = read(msgsock, buffer, sizeof(buffer))) < 0)
               perror("reading stream message");
            if (rval == 0) std::cerr << "Ending connection" << std::endl;
            else
            {
               total_count += rval;
               // Sum up byte count
               for (int i = 0; i < rval; ++i)
               {
                  byte_count = buf[i];
                  checksum += byte_count;
               }
            }
         } while (rval != 0);

         // Prepare string output
         std::ostringstream os;
         os << "Sum: " << checksum << " Len: " 
           << total_count << ".";
         std::string str = os.str();
         const char* ch = str.c_str();

         // Write to client
         if (safe_write(msgsock, ch, str.length()-1) < 0)
            perror("writing on stream socket");

      close(msgsock);
      }
   } while (true);
   
   return 0;
}
