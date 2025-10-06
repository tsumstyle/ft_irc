/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:55 by aroux             #+#    #+#             */
/*   Updated: 2025/10/06 13:59:07 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

/* PING
Requirements:
  - Must be sent by a registered client.
  - Must include a token to respond to; otherwise behavior may vary (typically ignored).

Behavior:
  - Replies with PONG <token> to the client.
  - Keeps the connection alive (heartbeat).
  - Logs the ping request on the server.
*/
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
