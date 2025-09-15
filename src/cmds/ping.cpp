/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:55 by aroux             #+#    #+#             */
/*   Updated: 2025/09/15 15:32:02 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/messages.hpp"

void	Server::handlePing(Client *c, const ParsedCmd &data) {
	std::string token = (data.args.empty() ? "" : data.args[0]);
	std::string reply = "PONG " + token + "\r\n";
	send(c->getSocket(), reply.c_str(), reply.size(), 0);
}
