/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:26:38 by aroux             #+#    #+#             */
/*   Updated: 2025/09/29 13:13:39 by aroux            ###   ########.fr       */
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
	std::string				_name;			// channel name
	std::vector<Client*>	_users;			// list of clients that are on the channel //pointing to already existing clients; could also just store the fds
	std::vector<Client*>	_operators;		// all operators (=moderators) of the channel
	std::string				_localpass;		// password on channel

	// for operator commands:
	bool					_reqPassword; // false; // if !requirePassword and command "MODE +k" -> ask for new password
	bool					_inviteOnly; // false;
	bool					_topicRestricted; // true;
	bool					_userLimitSet; // false; // if !_userLimitSet and command "MODE +l x" -> ser _userLimit = x 
	size_t					_userLimit; // 0;

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
	void					broadcast(const std::string& msg, Client* sender);
	std::string				getName();
	std::vector<Client*>	getUsers();
	Client*					findUser(const std::string& name);
	bool					isOperator(const Client* name);

	// for chanop 26.9 -- caro
	bool					isChannelFull();
	bool					isUserLimitSet();
	size_t					getUserLimit();
	bool					isReqPassword();
	bool					isInviteOnly();
	bool					isTopicRestricted();

	// for MODE - setters. 29.9 -- caro
	void					setUserLimitSet(bool desired);
	void					setReqPassword(bool desired);
	void					setTopicRestricted(bool desired);
	void					setInviteOnly(bool desired);
};
