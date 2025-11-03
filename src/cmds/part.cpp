/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 13:24:38 by aroux             #+#    #+#             */
/*   Updated: 2025/11/03 13:15:08 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void	Server::handlePart(Client *c, const ParsedCmd &data) {
	if (c->getState() != REGISTERED) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), "PART"));
		return ;
	}
	if (data.args.empty()) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), "PART"));
		return ;
	}
	std::string	reason;
	if (data.args.size() > 1 && data.lastTokenHasColon) {
		reason = data.args[1];			
		for (size_t	i = 2; i < data.args.size(); i++)
			reason += " " + data.args[i];
	}
	std::vector<std::string>	channelNames = split(data.args[0], ',');
	std::vector<Channel*>		channelsToPart;
	for (size_t i = 0; i < channelNames.size(); i++) {
		std::string	channelName = channelNames[i];
		if (channelName[0] != '#') {
			c->sendMessage(Replies::ERR_NOSUCHCHANNEL(c->getNick(), data.args[i]));
			continue;
		}
		Channel *ch = findChannel(channelName);
		if (!ch) {
			c->sendMessage(Replies::ERR_NOSUCHCHANNEL(c->getNick(), channelName));
			continue ;
		}	
		if (!ch->hasUser(c)) {
			c->sendMessage(Replies::ERR_NOTONCHANNEL(c->getNick(), ch->getName()));
			continue ;
		}
		channelsToPart.push_back(ch);
	}
	for (size_t j = 0; j < channelsToPart.size(); j++) {
		partFromChannel(c, channelsToPart[j], reason);
	}
}

void	Server::partFromChannel(Client* c, Channel* channel, const std::string& reason) {
	if (channel->isOperator(c) && channel->amountOperators() == 1) {
		channel->broadcast("No moderators left in the channel. Channel deleted", NULL, channel);
		// if channel has no members it will be removed by the cleanup
		std::vector<Client *> users = channel->getUsers();
		for (size_t i = 0; i < users.size(); i++) {
			if (users[i] == c) // this user is handled further down. this way he's the last one out
				break;
			channel->removeUser(users[i]);
		}
	}
	serverLog(c, " parted from channel " + channel->getName());
	std::string reply = ":" + c->getSource() + " PART " + channel->getName();
	if (!reason.empty()) 
		reply += " :" + reason;
	channel->broadcast(reply, NULL, NULL);
	channel->removeUser(c);
	channel->removeOperator(c);
	c->removeChannel(channel);
	if (channel->getUsers().empty())
		_channels.erase(channel->getName());
}
