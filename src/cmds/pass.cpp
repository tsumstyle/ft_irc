/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:12 by aroux             #+#    #+#             */
/*   Updated: 2025/09/26 13:13:28 by aroux            ###   ########.fr       */
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

	if (c->getState() != NEW)
		reply = Replies::ERR_ALREADYREGISTERED();
	if (data.args.size() < 1)
		reply = Replies::ERR_NEEDMOREPARAMS(c->getNick(), "PASS");
	if (data.args[0] == _server_pass) {
		c->setState(PASS_OK);
		reply = "Password accepted. Provide NICK and USER.\r\n";
	} else
		reply = Replies::ERR_PASSWMISMATCH();
	// TODO: if password is incorrect, client gets disconnected
	c->sendMessage(reply);
}
