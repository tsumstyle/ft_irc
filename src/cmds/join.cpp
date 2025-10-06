/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:24:54 by aroux             #+#    #+#             */
/*   Updated: 2025/10/06 15:14:20 by aroux            ###   ########.fr       */
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

// 29.9 Alex: updated to handle multiple channels
void	Server::handleJoin(Client *c, const ParsedCmd &data){
	if (c->getState() != REGISTERED) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), "JOIN"));
		return;
	}
	if (data.args.empty()) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), "JOIN"));
		return ;
	}
	std::vector<std::string>	channels = split(data.args[0], ',');
	std::vector<std::string>	keys;
	if (data.args.size() > 1) {
		keys = split(data.args[1], ',');
	}
	for (size_t i = 0; i < channels.size(); i++) {
		std::string	channel_name = channels[i];
		if (channel_name.empty() || channel_name[0] != '#') {
			c->sendMessage(Replies::ERR_BADCHANMASK(c->getNick(), channel_name));
			continue;
		}
		std::string	key = (i < keys.size()) ? keys[i] : "";
		handleJoinOneChannel(c, channel_name, key);
	}
}

void	Server::handleJoinOneChannel(Client *c, const std::string& channel_name, const std::string& key) {
	std::map<std::string, Channel>::iterator iter = _channels.find(channel_name);	// look for channel in map of channels
	if (iter != _channels.end()) {					// if channel exists
		Channel& channel = iter->second;
		if (c->isOnChannel(&channel))	{
			c->sendMessage(Replies::ERR_USERONCHANNEL(c->getNick(), channel_name));
			return ;
		}
		// if channel.userlimitset && channel.full -> ERR_CHANNELISFULL
		else if (channel.isUserLimitSet() && channel.isChannelFull()) {
			c->sendMessage(Replies::ERR_CHANNELISFULL(channel.getName()));
			return ;
		}
		// if channel.inviteOnly && !in_invite_list -> ERR_INVITEONLYCHAN
		else if (channel.isInviteOnly() && !channel.isInvited(c)) {
			c->sendMessage(Replies::ERR_INVITEONLYCHAN(channel.getName()));
			return ;
		}
		// if channel.passwordSet -> ask for password -> ok or ERR_PASSWDMISMATCH
		else if (channel.isReqPassword()) {
			if (key == "" || key != channel.getLocalPass()) {
				c->sendMessage(Replies::ERR_PASSWMISMATCH()); // change this msg
				return ;
			}
		}
		// all checks passed, add use to channel:
		channel.addUser(c);
		c->addChannel(&channel);
		channel.uninvite(c);
		channel.broadcast(":" + c->getSource() + " JOIN " + channel_name + "\r\n", NULL);
		serverLog(c, " joined channel " + channel_name);
	}
	else {
		_channels[channel_name] = Channel(channel_name);
		Channel& newChannel = _channels[channel_name];
		newChannel.addUser(c);
		newChannel.addOperator(c);
		c->addChannel(&newChannel);
		c->sendMessage(":" + c->getSource() + " JOIN " + channel_name + " (new channel created)\r\n");
		serverLog(c, " created and joined channel " + channel_name);
	}
}
