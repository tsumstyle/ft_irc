/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:08:36 by nick              #+#    #+#             */
/*   Updated: 2025/09/21 17:19:59 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

void Server::handleDirectMsg(Client *sender, std::string target, std::string msg) {
	Client *receiver = findClientByNick(target);
	if (!receiver) {
		err_handler("target nickname not found");
		return;
	}
	std::string full_msg = ":" + sender->getNick() + " PRIVMSG " + target + " :" + msg + "\r\n";
	send(receiver->getSocket(), full_msg.c_str(), full_msg.size(), 0); 
}

void Server::handleChannelMsg(Client *c, std::string target, std::string msg) {
	Channel *ch = findChannel(target);
	if (!ch) {
		std::string reply = "403" + target + " :No such channel \r\n";
		send(c->getSocket(), reply.c_str(), reply.size(), 0);
		return;
	}
	ch->broadcast(msg, c);	
}

void Server::handlePrivMsg(Client *c, const ParsedCmd &data) {
	if (data.args.size() < 2) {
		err_handler("handlePrivMsg: not enough arguments");
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
