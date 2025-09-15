/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:12 by aroux             #+#    #+#             */
/*   Updated: 2025/09/15 15:31:14 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/messages.hpp"

void	Server::handlePass(Client *c, const ParsedCmd &data) {
	std::string reply;

	if (c->getState() != NEW) {
		reply = "462: You may not reregister\r\n"; // ERR_ALREADYREGISTRED
		send(c->getSocket(), reply.c_str(), reply.size(), 0);
		return;
	}
	if (data.args.size() < 1) {
		reply = "461 PASS: Not enough parameters\r\n"; // ERR_NEEDMOREPARAMS
		send(c->getSocket(), reply.c_str(), reply.size(), 0);
		return;
	}
	if (data.args[0] == _server_pass) {
		c->setState(PASS_OK);
		reply = "Password accepted. Provide NICK and USER.\r\n";
	} else
		reply = "464: Password incorrect"; // ERR_PASSWDMISMATCH
	// TODO: if password is incorrect, client gets disconnected
	send(c->getSocket(), reply.c_str(), reply.size(), 0);
}
