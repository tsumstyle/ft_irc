/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:24:54 by aroux             #+#    #+#             */
/*   Updated: 2025/10/24 15:09:25 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

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
	std::map<std::string, Channel>::iterator iter = _channels.find(channel_name);
	if (iter != _channels.end()) {
		Channel& channel = iter->second;
		if (c->isOnChannel(&channel))	{
			c->sendMessage(Replies::ERR_USERONCHANNEL(c->getNick(), channel_name));
			return ;
		}
		else if (channel.isUserLimitSet() && channel.isChannelFull()) {
			c->sendMessage(Replies::ERR_CHANNELISFULL(channel.getName()));
			return ;
		}
		else if (channel.isInviteOnly() && !channel.isInvited(c)) {
			c->sendMessage(Replies::ERR_INVITEONLYCHAN(channel.getName()));
			return ;
		}
		else if (channel.isReqPassword()) {
			if (key == "" || key != channel.getLocalPass()) {
				c->sendMessage(Replies::ERR_PASSWDMISMATCH());
				return ;
			}
		}
		channel.addUser(c);
		c->addChannel(&channel);
		channel.uninvite(c);

		channel.broadcast(yellow(":" + c->getSource() + " JOIN " + channel_name + "\r\n"), NULL, NULL);
		serverLog(c, " joined channel " + channel_name);
	}
	else {
		_channels[channel_name] = Channel(channel_name);
		Channel& newChannel = _channels[channel_name];
		newChannel.addUser(c);
		newChannel.addOperator(c);
		c->addChannel(&newChannel);
		c->sendMessage(yellow(":" + c->getSource() + " JOIN " + channel_name + " (new channel created)\r\n"));
		serverLog(c, " created and joined channel " + channel_name);
	}
}
