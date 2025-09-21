/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:08:36 by nick              #+#    #+#             */
/*   Updated: 2025/09/20 13:58:24 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

void Server::handleDirectMsg(Client *sender, std::string target, std::string msg) {
	//findtargetClient;
	Client *receiver = NULL;
	
	for(std::map<int, Client*>::iterator it = _connected.begin(); it != _connected.end(); ++it) {
		if (it->second && it->second->getNick() == target) {
			receiver = it->second;
			break;
		}
	}
	
	std::string full_msg = ":" + sender->getNick() + " PVIVMSG " + target + " :" + msg + "\r\n";
	
	send(receiver->getSocket(), full_msg.size(), 
}

void handleChannelMsg(Client *c, std::string target, std::string msg) {
	
}

void Server::handlePrivMsg(Client *c, const ParsedCmd &data) {
	if (data.args.size() < 2) {
		err_handler("handlePrivMsg: not enough arguments");
		return;
	}
	std::string target = data.args[0];
	std::string msg = data.args[1];
	for (size_t i = 0; i < data.args.size(); i++) {
		msg += " " + data.args[i];
	}

	if (target[0] == '#')
		handleChannelMsg(c, target, msg);
	else
		handleDirectMsg(c, target, msg);
}
