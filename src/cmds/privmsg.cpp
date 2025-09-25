/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:08:36 by nick              #+#    #+#             */
/*   Updated: 2025/09/25 16:18:40 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

void Server::handleDirectMsg(Client *sender, std::string target, std::string msg) {
	Client *receiver = findClientByNick(target);
	if (!receiver) 
		sender->sendMessage(Replies::ERR_NOSUCHNICK(sender->getNick(), target));
	else 
		receiver->sendMessage(Replies::RPL_PRIVMSG(sender->getNick(), target, msg));
}

void Server::handleChannelMsg(Client *c, std::string target, std::string msg) {
	Channel *ch = findChannel(target.substr(1));
	if (!ch)
		c->sendMessage(Replies::ERR_NOSUCHCHANNEL(c->getNick(), target));
	else
		ch->broadcast(msg, c);	
}

void Server::handlePrivMsg(Client *c, const ParsedCmd &data) {
	if (data.args.size() < 2) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), "PRIVMSG"));
		return;
	}
	std::string target = data.args[0];
	std::string msg;
	for (size_t i = 1; i < data.args.size(); i++) {
		msg += " " + data.args[i];
	}

	if (target[0] == '#')
		handleChannelMsg(c, target, msg);
	else
		handleDirectMsg(c, target, msg);
}
