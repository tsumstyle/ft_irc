/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:26:38 by aroux             #+#    #+#             */
/*   Updated: 2025/10/27 11:55:43 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

# include <string>
# include <vector>
# include "Client.hpp"
# include "utilities.hpp"
# include <sys/socket.h>	// send()

class Client; 

class Channel	{
private:
	std::string				_name;
	std::vector<Client*>	_users;
	std::vector<Client*>	_operators;
	std::vector<Client*>	_inviteList;
	std::string				_localpass;

	// for operator commands:
	bool					_reqPassword;
	bool					_inviteOnly;
	bool					_topicRestricted;
	std::string				_topic;
	bool					_userLimitSet;
	size_t					_userLimit;

public:
// constructors
	Channel();
	Channel(std::string channel_name);
	Channel(const Channel& copy);
	~Channel();
	Channel& operator=(const Channel& other);

	void					addUser(Client* user);
	void					removeUser(Client* user);
	bool					hasUser(Client* user);

	void					addOperator(Client* user);
	void					removeOperator(Client* user);
	bool					isOperator(const Client* name);

	void					broadcast(const std::string& msg, Client* sender, Channel* chan);
	std::string				getName();
	std::vector<Client*>	getUsers();
	Client*					findUser(const std::string& name);

	void					invite(Client* user);
	void					uninvite(Client* user);
	bool					isInvited(const Client* user);

	bool					isChannelFull();
	bool					isUserLimitSet();
	size_t					getUserLimit();
	bool					isReqPassword();
	std::string				getLocalPass();
	bool					isInviteOnly();
	bool					isTopicRestricted();
	std::string				getTopic();

	void					setUserLimitSet(bool desired);
	void					setUserLimit(const std::string& lim);
	void					setReqPassword(bool desired);
	void					setLocalPass(const std::string& pass);
	void					setTopicRestricted(bool desired);
	void					setTopic(const std::string& topic);
	void					setInviteOnly(bool desired);
};
