/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:22:32 by aroux             #+#    #+#             */
/*   Updated: 2025/09/15 14:52:00 by aroux            ###   ########.fr       */
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
	
	if ((argc == 3) && isnum(argv[1])) {
		port = std::atoi(argv[1]);
		std::string	password = argv[2];
	
		Server	irc(port, password);
		irc.start(); 
		irc.run();
	}
	else
		return err_handler("Invalid input, use: ./ircserv <port> <password>\n");
	// other stuff for the argument check?
//		- Port must be in the range 1-65535.
//		- atoi must not fail
//		- password cannot be empty
	
	return 0;
}
