/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:04:43 by aroux             #+#    #+#             */
/*   Updated: 2025/09/29 13:13:29 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

// constructors_operators
Channel::Channel() : _name("defaultchannel"),
					 _localpass(""),
					 _reqPassword(false),
					 _inviteOnly(false),
					 _topicRestricted(true),
					 _userLimitSet(false),
					 _userLimit(0) {}

Channel::Channel(std::string channel_name) : _name(channel_name),
											 _localpass(""),
											 _reqPassword(false),
											 _inviteOnly(false),
											 _topicRestricted(true),
											 _topic(""),
											 _userLimitSet(false),
											 _userLimit(0) {}

Channel::Channel(const Channel& copy) : _name(copy._name), 
										_users(copy._users),
										_operators(copy._operators),
										_localpass(copy._localpass),
										_reqPassword(copy._reqPassword),
										_inviteOnly(copy._inviteOnly),
										_topicRestricted(copy._topicRestricted),
										_topic(copy._topic),
										_userLimitSet(copy._userLimitSet),
										_userLimit(copy._userLimit) {}

Channel::~Channel() {}

// assignment operator
Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		_name = other._name;
		_users = other._users;
		_operators = other._operators;
		_localpass = other._localpass;
		_reqPassword = other._reqPassword;
		_inviteOnly = other._inviteOnly;
		_topicRestricted = other._topicRestricted;
		_topic = other._topic;
		_userLimitSet = other._userLimitSet;
		_userLimit = other._userLimit;
	}
	return *this;
}

// other members functions
bool	Channel::hasUser(Client* user) {
	for (size_t i = 0; i < _users.size(); i++) {
		if (user == _users[i]) {
			//err_handler("Client is already in the channel");		// TODO: This should go and be replace by an error code where the function is called
			return true; 
		}
	}
	return false;
}

void	Channel::addUser(Client* user) {
	if (!hasUser(user))
		_users.push_back(user);
	else
		err_handler("User already on channel"); 		// TODO: This should go and be replace by an error code where the function is called
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

void	Channel::broadcast(const std::string& msg, Client* sender) {
	for (size_t i = 0; i < _users.size(); i++) {
		if (_users[i] != sender)
			_users[i]->sendMessage(msg);
	}
}

std::string	Channel::getName(){
	return (_name);
}

std::vector<Client*>	Channel::getUsers() {
	return (_users);
}

// added 29.9 -- caro
Client*	Channel::findUser(const std::string& name) {
	for (size_t i = 0; i < this->_users.size(); i++) {
		if (this->_users[i]->getNick() == name)
			return (this->_users[i]);
	}
	return (NULL);
}

bool	Channel::isOperator(const Client* name) {
	for (size_t i = 0; i < this->_operators.size(); i++) {
		if (this->_operators[i] == name)
			return true;
	}
	return false;
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

std::string	Channel::getLocalPass() {
	return this->_localpass;
}

bool	Channel::isInviteOnly() {
	return this->_inviteOnly;
}

bool	Channel::isTopicRestricted() {
	return this->_topicRestricted;
}

std::string	Channel::getTopic() {
	return this->_topic;
}

// added 29.9 -- caro
void	Channel::setUserLimitSet(bool desired) {
	this->_userLimitSet = desired;
}

void	Channel::setUserLimit(const std::string& lim) {
	this->_userLimit = atoi(lim.c_str()); // is this the best way?
	// check number is valid, return 0 on fail?
}

void	Channel::setReqPassword(bool desired) {
	this->_reqPassword = desired;
}

void	Channel::setLocalPass(const std::string& pass) {
	this->_localpass = pass;
}

void	Channel::setTopicRestricted(bool desired) {
	this->_topicRestricted = desired;
}

void	Channel::setTopic(const std::string& topic) {
	this->_topic = topic;
}

void	Channel::setInviteOnly(bool desired) {
	this->_inviteOnly = desired;
}
