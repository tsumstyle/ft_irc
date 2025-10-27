/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 13:24:38 by aroux             #+#    #+#             */
/*   Updated: 2025/10/27 11:11:59 by aroux            ###   ########.fr       */
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
		for (size_t	i = 2; i < data.args.size(); i++)							// if there are more args. append them to the reason
			reason += " " + data.args[i];
	}
	std::vector<std::string>	channelNames = split(data.args[0], ',');		// multiple channels
	std::vector<Channel*>		channelsToPart;
// validate each channel and collect valid ones
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
// process valid channels
	for (size_t j = 0; j < channelsToPart.size(); j++) {
		partFromChannel(c, channelsToPart[j], reason);
	}			
}

void	Server::partFromChannel(Client* c, Channel* channel, const std::string& reason) {
	serverLog(c, " parted from channel " + channel->getName());
	std::string reply = ":" + c->getSource() + " PART " + channel->getName();	// replace @host part with getSource() function
	if (!reason.empty()) 
		reply += " :" + reason;
	reply += "\r\n";
	channel->broadcast(reply, NULL, NULL);
	channel->removeUser(c);
	channel->removeOperator(c);
	c->removeChannel(channel);
	if (channel->getUsers().empty())
		_channels.erase(channel->getName());
}
