/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:04:34 by aroux             #+#    #+#             */
/*   Updated: 2025/10/27 11:50:19 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/utilities.hpp"
#include "../inc/parser.hpp"

bool	Server::isNickTaken(const std::string& nick) {
	for (std::map<int, Client*>::iterator it = _connected.begin(); it != _connected.end(); it++) {
		if (it->second && it->second->getNick() == nick)
			return true;
	}
	return false;
}

bool	Server::isValidChar(const char c) {
	if (std::isalpha(c))
		return true;
	const char symbols[] = "[]\\`^{}|_";
	for (int i = 0; symbols[i]; ++i)
		if (c == symbols[i])
			return true;
	return false;
}

bool	Server::isValidNick(const std::string& str) {
	if (str.empty() || str.size() > 9)
		return false;
	if (!isValidChar(str[0]))
		return false;
	for (size_t i = 1; i < str.length(); ++i)
		if (!isValidChar(str[i]) && !std::isdigit(str[i]) && str[i] != '-')
			return false;
	return true;
}

bool Server::isValidUsername(const std::string& username) {
	if (username.empty() || username.length() > 12)
		return false;
	for (size_t i = 0; i < username.length(); i++) {
		char c = username[i];
		if (!std::isalnum(c) && c != '-' && c != '_')
			return false;
	}
	return true;
}

Channel *Server::findChannel(std::string target) {
	std::string	lowerTarget = toLower(target);
	std::map<std::string, Channel>::iterator it = _channels.find(lowerTarget);
	if (it != _channels.end())
		return &it->second;
	return NULL;
}

Client* Server::findClientByNick(const std::string& nick) {
	for (std::map<int, Client*>::iterator it = _connected.begin(); it != _connected.end(); ++it) {
		if (it->second && it->second->getNick() == nick)
			return it->second;
	}
	return NULL;
}

void	Server::cleanupDisconnectedClients() {
	for (std::map<int, Client*>::iterator it = _connected.begin(); it != _connected.end();) {
		if (it->second->getState() == DISCONNECTED) {
			std::map<int, Client*>::iterator to_erase = it;
			++it;
			close(to_erase->first);
			for (size_t i = 0; i < _fds.size(); i++) {
				if (_fds[i].fd == to_erase->first) {
					_fds[i].fd = -1;
					break;
				}
			}
			delete to_erase->second;
			_connected.erase(to_erase);	
		}
		else
			it++;
	}
}

void	Server::cleanupEmptyChannels() {
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ) {
		if (it->second.getUsers().empty()) {
			std::cout << "Removing empty channel: " << it->first << std::endl;
			_channels.erase(it++);
		}
		else
			++it;
	}
}
