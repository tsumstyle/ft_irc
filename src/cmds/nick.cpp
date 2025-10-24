/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:26:43 by aroux             #+#    #+#             */
/*   Updated: 2025/10/24 14:38:25 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void	Server::handleNick(Client *c, const ParsedCmd &data) {
	if (c->getState() == NEW)
		c->sendMessage(Replies::ERR_NOTREGISTERED("*", "NICK"));
	else if (data.args.empty())
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS("*", "NICK"));
	else if (isNickTaken(data.args[0]))
		c->sendMessage(Replies::ERR_NICKNAMEINUSE(c->getNick()));
	else if (!isValidNick(data.args[0]))
		c->sendMessage(Replies::ERR_ERRONEUSNICKNAME(data.args[0]));
	else {
		c->setNick(data.args[0]);
		if (c->getState() == USERNAME_OK) {
			c->setState(REGISTERED);
			serverLog(c, " is fully registered");
			c->sendMessage(Replies::RPL_WELCOME(c->getNick(), c->getUser()));
		}
		else {
			c->setState(NICK_OK);
			serverLog(c, " set nickname to '" + c->getNick() + "'");
			c->sendMessage(Replies::RPL_YOURHOST(c->getNick()));
		}
	}	
}
