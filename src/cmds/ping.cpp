/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:55 by aroux             #+#    #+#             */
/*   Updated: 2025/09/26 13:20:31 by aroux            ###   ########.fr       */
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
	std::string token = (data.args.empty() ? "" : data.args[0]);
	std::string reply = "PONG " + token + "\r\n";
	c->sendMessage(reply);
}
