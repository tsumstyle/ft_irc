/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:08:36 by nick              #+#    #+#             */
/*   Updated: 2025/10/24 17:02:50 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

void Server::handleDirectMsg(Client *sender, std::string target, std::string msg) {
	Client *receiver = findClientByNick(target);
	if (!receiver || (receiver->getState() != REGISTERED)) 
		sender->sendMessage(Replies::ERR_NOSUCHNICK(sender->getNick(), target));
	else 
		receiver->sendMessage(Replies::RPL_PRIVMSG(sender->getNick(), target, msg));
}

void Server::handleChannelMsg(Client *c, std::string target, std::string msg) {
	Channel *ch = findChannel(target);
	if (!ch)
		c->sendMessage(Replies::ERR_NOSUCHCHANNEL(c->getNick(), target));
	else if (!ch->hasUser(c))
		c->sendMessage(Replies::ERR_NOTONCHANNEL(ch->getName()));
	else
		ch->broadcast(msg, c, ch);
}

void Server::handlePrivMsg(Client *c, const ParsedCmd &data) {
	if (c->getState() != REGISTERED) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), "PRIVMSG"));
		return ;
	}
	if (data.args.empty() || data.args.size() < 2) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), "PRIVMSG"));
		return;
	}
	std::string target = data.args[0];
	std::string msg;
	for (size_t i = 1; i < data.args.size(); i++) {
		if (i != 1)
			msg += " ";
		msg += data.args[i];
	}
	msg += "\r\n";

	if (target[0] == '#')
		handleChannelMsg(c, target, msg);
	else
		handleDirectMsg(c, target, msg);
}
