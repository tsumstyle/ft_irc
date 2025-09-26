/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:04:43 by aroux             #+#    #+#             */
/*   Updated: 2025/09/21 17:19:51 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

// constructors_operators
Channel::Channel() : _name("defaultchannel"),
					 _reqPassword(false),
					 _inviteOnly(false),
					 _topicRestricted(true),
					 _userLimitSet(false),
					 _userLimit(0) {}

Channel::Channel(std::string channel_name) : _name(channel_name) {}

Channel::Channel(const Channel& copy) : _name(copy._name), 
										_users(copy._users),
										_operators(copy._operators),
										_reqPassword(false),
										_inviteOnly(false),
										_topicRestricted(true),
										_userLimitSet(false),
										_userLimit(0) {}

Channel::~Channel() {}

// assignment operator
Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		_name = other._name;
		_users = other._users;
		_operators = other._operators;
		_reqPassword = other._reqPassword;
		_inviteOnly = other._inviteOnly;
		_topicRestricted = other._topicRestricted;
		_userLimitSet = other._userLimitSet;
		_userLimit = other._userLimit;
	}
	return *this;
}

// other members functions
bool	Channel::hasUser(Client* user) {
	for (size_t i = 0; i < _users.size(); i++) {
		if (user == _users[i]) {
			err_handler("Client is already in the channel");
			return true; 
		}
	}
	return false;
}

void	Channel::addUser(Client* user) {
	if (!hasUser(user))
		_users.push_back(user);
	else
		err_handler("User already on channel");
}

void	Channel::removeUser(Client* user) {
	for (size_t i = 0; i < _users.size(); i++) {
		if (user == _users[i]) {
			_users.erase(_users.begin() + i);
			return ;
		}
	}
	err_handler("User is not in channel");
}

void	Channel::addOperator(Client* user) {
	for (size_t i = 0; i < _operators.size(); i++) {
		if (user == _operators[i])
			return ; //  write error msg?
	}
	_operators.push_back(user);
}

void	Channel::removeOperator(Client* user) {
	for (size_t i = 0; i < _operators.size(); i++) {
		if (user == _operators[i]) {
			_operators.erase(_operators.begin() + i);
			return ;
		}
	}
}

void	Channel::broadcast(std::string& msg, Client* sender) {
	for (size_t i = 0; i < _users.size(); i++) {
		if (_users[i] != sender) {
			int	fd = _users[i]->getSocket();
			send(fd, msg.c_str(), msg.size(), 0);
		}
	}
}

std::string	Channel::getName(){
	return (_name);
}


std::vector<Client*>	Channel::getUsers() {
	return (_users);
}

// added for chanop 26.9 -- caro
bool	Channel::isChannelFull() {
	if (this->_userLimitSet) {
		return this->_users.size() >= this->_userLimit;
	}
	return false;
}

bool	Channel::isUserLimitSet() {
	return this->_userLimitSet;
}

size_t		Channel::getUserLimit() {
	return this->_userLimit;
}

bool	Channel::isReqPassword() {
	return this->_reqPassword;
}

bool	Channel::isInviteOnly() {
	return this->_inviteOnly;
}

bool	Channel::isTopicRestricted() {
	return this->_topicRestricted;
}
