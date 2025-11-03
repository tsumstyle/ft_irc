/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:12 by aroux             #+#    #+#             */
/*   Updated: 2025/11/03 12:40:31 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void	Server::handlePass(Client *c, const ParsedCmd &data) {
	std::string reply;
	if (c->getState() != NEW) {
		c->sendMessage(Replies::ERR_ALREADYREGISTERED());
		return ;
	}
	if (data.args.size() < 1) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS("*", "PASS"));
		return ;
	}
	if (data.args[0] == _server_pass) {
		c->setState(PASS_OK);
		c->sendMessage("Password accepted. Provide NICK and USER.\r\n");
		serverLog(c, " entered right password");
	}
	else
		c->sendMessage(Replies::ERR_PASSWDMISMATCH());
}
