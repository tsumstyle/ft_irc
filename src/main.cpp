/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:22:32 by aroux             #+#    #+#             */
/*   Updated: 2025/09/13 18:19:28 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>		// socket()
#include <netinet/in.h>		// sockaddr_in (struct used to define the address we want to assign to the socket in the case of an Internet Protocol (IP))
#include <unistd.h>			// read()
#include <cstdlib>			// exit(), EXIT_FAILURE
#include <cerrno>			// errno
#include <poll.h>			// poll(), pollfd
#include "utilities.hpp"
#include "Server.hpp"
#include "Client.hpp"

//Client join: telnet 127.0.0.1 8080

int	main(int argc, char **argv)	{
	int port;
	
	if ((argc == 2 || argc == 3) && isnum(argv[1]))
		port = std::atoi(argv[1]); 
	else
		return err_handler("Invalid input, use: ./ircserv <port> <password> ");
	Server	irc(port);
	irc.start(); 
	irc.run();
	return 0;
}
