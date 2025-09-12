/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:41:09 by aroux             #+#    #+#             */
/*   Updated: 2025/09/12 17:29:12 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <ctime>
#include "Client.hpp"

std::string	getTimeStamp() {
	std::time_t	t = std::time(NULL);
	char	buffer[20];
	std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&t));
	return std::string(buffer);
}

/* write server logs, send messages to the client if necessary */
void	logSendToClient(Client* client, const std::string& msg, bool toClient)	{
// msg written to the server: 	
	if (client)
		std::cout << "[" << getTimeStamp() << "] [CLIENT " << client->getSocket() << "] " << msg << std::endl;	// client log
	else
		std::cout << "[" << getTimeStamp() << "] [SERVER] " << msg << std::endl;  				// server log
// msg sent to client:
	if (client && toClient)
		send(client->getSocket(), msg.c_str(), msg.size(), 0);
}


