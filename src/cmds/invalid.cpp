/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:28:56 by aroux             #+#    #+#             */
/*   Updated: 2025/09/15 16:29:10 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/messages.hpp"

void Server::InvalidCmd(Client *c, const ParsedCmd &data) {
	std::string token = data.cmd;
	std::string reply = "Invalid command: " + token + "\r\n";
	send(c->getSocket(), reply.c_str(), reply.size(), 0);	
}