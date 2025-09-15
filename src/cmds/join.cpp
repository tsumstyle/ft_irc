/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:24:54 by aroux             #+#    #+#             */
/*   Updated: 2025/09/15 15:31:54 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/messages.hpp"


void	Server::handleJoin(Client *c, const ParsedCmd &data){
	std::string reply;
	if (c->getState() != REGISTERED)
		reply = "Error XX: you must register before joining a channel\r\n"; // check format
	else if (data.args.empty() || data.args[0][0] != '#')
		reply = "Error XX: invalid channel format (must start with #)\r\n";
	else {
		std::string	channel_name = data.args[0].substr(1);	// remove # char
		std::map<std::string, Channel>::iterator iter = _channels.find(channel_name);	// look for channel in map of channels
		if (iter != _channels.end()) {					// channel exists, add user
			iter->second.addUser(c);
			reply = ":" + c->getNick() + " JOIN #" + channel_name + "\r\n";
			// TODO add channel to the client's list of channels (or useless step?) : client.joinChannel(_channels[i])
		}
		else {
			Channel	newChannel(channel_name); 			// create new channel
			newChannel.addUser(c);
			newChannel.addOperator(c);
			_channels[channel_name] = newChannel;		// add it to the server
			reply = ":" + c->getNick() + " JOIN #" + channel_name + " (new channel created)\r\n";
			// TODO server output: channel created + user joined channel + made operator
		}	
	}
	send(c->getSocket(), reply.c_str(), reply.size(), 0);
}
