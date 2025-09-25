/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:24:54 by aroux             #+#    #+#             */
/*   Updated: 2025/09/25 16:17:16 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

/* JOIN
Requirements:
   - Client must be REGISTERED (NICK+USER set), otherwise send ERR_NOTREGISTERED (451).
   - Channel name must start with '#' and follow valid channel naming rules; otherwise send ERR_BADCHANNAME (476) or similar.
   - If client is already in channel, send ERR_USERONCHANNEL (443).
 
Behavior:
   - Adds client to the channel:
       - If channel exists: add client.
       - If channel does not exist: create channel, add client, and set as operator.
   - Adds channel to the client’s list of joined channels.
   - Sends JOIN message to client and optionally to all users in the channel.
   - Logs the channel join or creation on the server.
 */
void	Server::handleJoin(Client *c, const ParsedCmd &data){
	std::string reply;
	if (c->getState() != REGISTERED)
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), "JOIN"));
	else if (data.args.empty() || data.args[0][0] != '#')
		c->sendMessage(Replies::ERR_NOSUCHCHANNEL(c->getNick(), data.args.empty() ? "" : data.args[0]));
	else {
		std::string	channel_name = data.args[0].substr(1);	// remove # char
		std::map<std::string, Channel>::iterator iter = _channels.find(channel_name);	// look for channel in map of channels
		if (iter != _channels.end()) {					// if channel exists
			if (c->isOnChannel(&iter->second))			 // if user is already on channel
				c->sendMessage(Replies::ERR_USERONCHANNEL(c->getNick(), channel_name));
			else {
				iter->second.addUser(c);
				c->addChannel(&(iter->second));
				c->sendMessage(":" + c->getNick() + " JOIN #" + channel_name + "\r\n");
				serverLog(c, " joined channel #" + channel_name);
			}
		}
		else {
			Channel	newChannel(channel_name); 			// if channel doesnt exist, create new channel and add user
			newChannel.addUser(c);
			newChannel.addOperator(c);
			_channels[channel_name] = newChannel;		// add it to the server
			c->addChannel(&_channels[channel_name]);
			c->sendMessage(":" + c->getNick() + " JOIN #" + channel_name + " (new channel created)\r\n");
			serverLog(c, " created and joined channel #" + channel_name);
		}	
	}
}
