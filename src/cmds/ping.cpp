/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:55 by aroux             #+#    #+#             */
/*   Updated: 2025/10/24 15:10:06 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void	Server::handlePing(Client *c, const ParsedCmd &data) {
	if (c->getState() != REGISTERED) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), "PING"));
		return;
	}
	if (data.args.empty()) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), "PING"));
		return ;
	}
	std::string token = data.args[0];
	std::string reply = "PONG :" + token + "\r\n";
	c->sendMessage(reply);
	serverLog(c, " PING: " + token);
}
