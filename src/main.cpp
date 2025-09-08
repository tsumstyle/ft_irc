/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:22:32 by aroux             #+#    #+#             */
/*   Updated: 2025/09/08 16:40:38 by aroux            ###   ########.fr       */
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

int	main(void)	{
	Server	irc(8080);
	irc.start(); 
	irc.run();
	return 0;
}