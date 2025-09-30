/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:12 by aroux             #+#    #+#             */
/*   Updated: 2025/09/30 16:13:37 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

/*  PASS
Requirements:
   - Must be the first command sent by the client (state == NEW), otherwise ignore or send ERR_ALREADYREGISTERED (462).
   - Must match server password; otherwise send ERR_PASSWDMISMATCH (464). 
Behavior:
   - If valid, sets client state to PASS_OK.
   - Logs the successful password verification on the server. */
   
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
	}
	else {
		c->sendMessage(Replies::ERR_PASSWMISMATCH());
		c->setState(DISCONNECTED);
	}
}
