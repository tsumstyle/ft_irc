/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_error_handler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:08:03 by aroux             #+#    #+#             */
/*   Updated: 2025/10/06 14:20:32 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/utilities.hpp"
#include "../inc/parser.hpp"

bool	Server::isRunning() const { return _running; }

void	Server::stop() { _running = false; }

void	Server::handleSocketError(int fd) {
	Client* client = _connected[fd];
	if (client) {
		std::cout << "Socket error on fd " << fd << " = client " << client->getNick() << std::endl;
		client->setState(DISCONNECTED);
	}
}
void	Server::cleanShutdown() {
	serverLog(NULL, "Initiating clean shutdown...");
// Notify all clients with proper QUIT messages
	for (std::map<int, Client*>::iterator it = _connected.begin(); it != _connected.end(); ++it) {
		if (it->second) {
		// send shutdown message to all connected clients
			std::string	goodbyeMsg = "ERROR :Server is shutting down\r\n";
			it->second->sendMessage(goodbyeMsg);
		// send message to all channels the client is in
			std::vector<Channel*>	channels = it->second->getChannels();
			for (size_t i = 0; i < channels.size(); i++) {
				std::string	goodbyeChannelMsg = ":" + it->second->getSource() + " QUIT : server shutdown\r\n";
				channels[i]->broadcast(goodbyeChannelMsg, NULL, NULL);
			}
		}
	}
// Delay to allow messages to be sent
	serverLog(NULL, "Waiting for clients to ne notified...");
	usleep(500000);
// Log shutdown info
	serverLog(NULL, "Shutting down with " + toString(_connected.size()) +  " clients and " + toString(_channels.size()) + " channels");
	serverLog(NULL, "Server shutdown complete.");
}
