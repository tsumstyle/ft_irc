/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:22:32 by aroux             #+#    #+#             */
/*   Updated: 2025/09/30 17:23:30 by aroux            ###   ########.fr       */
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
	if (argc != 3)
		return err_handler("Usage:  ./ircserv <port> <password>");
	if (!isnum(argv[1]))
		return err_handler("Port must be a number");
	int port = std::atoi(argv[1]);
	if (port < 1 || port > 65535)
		return err_handler("Port must be between 1-65535");
	
	std::string	password = argv[2];
	if (password.empty())
		return err_handler("Password cannot be empty");
	
	try {
		Server	irc(port, password);
		irc.start(); 
		irc.run();
	} catch (const std::exception& e) {
		std::cerr << "Server error: " << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
