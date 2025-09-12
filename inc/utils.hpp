/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:52:08 by aroux             #+#    #+#             */
/*   Updated: 2025/09/12 17:29:31 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

/********************************/
/* 			FORMATTING 			*/
/********************************/
/* 	Text effect	*/
#define RESET	"\033[0m"
#define BOLD	"\033[1m"
#define ITALIC	"\033[3m"
#define	REVERSE	"\033[7m"
#define STRIKETHROUGH	"\033[9m"
/* 	Text colors	*/
#define BLACK	"\033[30m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define MAGENTA	"\033[35m"
#define CYAN	"\033[36m"
#define WHITE	"\033[37m"

/********************************/
/* 			STRUCTS 			*/
/********************************/

/********************************/
/* 			HEADERS 			*/
/********************************/
/* 		C++98 HEADERS 		*/
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cstdlib>			// exit(), EXIT_FAILURE, EXIT_SUCCESS

/* 		 C HEADERS 			*/
#include <errno.h>			// not sure if we're allowed to use it
#include <poll.h>
#include <unistd.h>			// read(), open(), close()
#include <sys/socket.h>		// socket(), send()
#include <netinet/in.h>		// sockaddr_in (struct used to define the address we want to assign to the socket in the case of an Internet Protocol (IP))

/* 		 CLASS HEADERS 			*/
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

/********************************/
/* 			PROTOTYPES 			*/
/********************************/

/* logs.cpp */
std::string	getTimeStamp();
void		logSendToClient(Client* client, const std::string& msg, bool toClient);
