/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:22:32 by aroux             #+#    #+#             */
/*   Updated: 2025/09/12 15:49:46 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>		// socket()
#include <netinet/in.h>		// sockaddr_in (struct used to define the address we want to assign to the socket in the case of an Internet Protocol (IP))
#include <unistd.h>			// read()
#include <cstdlib>			// exit(), EXIT_FAILURE
#include <cerrno>			// errno
#include <poll.h>			// poll(), pollfd
#include "utils.hpp"
#include "Server.hpp"
#include "Client.hpp"

int	main(int argc, char** argv)	{
	if (argc != 3) {
		std::cout << "Wrong number of arguments.\nUsage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
// other stuff for the argument check?
//		- Port must be in the range 1-65535.
//		- atoi must not fail
//		- password cannot be empty
	int			port = std::atoi(argv[1]);
	std::string	password = argv[2];
	
	Server	irc(port, password);
	irc.start(); 
	irc.run();
	
	return 0;
}