/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:26:38 by aroux             #+#    #+#             */
/*   Updated: 2025/09/11 15:05:11 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

# include <sys/socket.h>	// send()
# include <string>
# include <vector>
# include "Client.hpp"


class Client; 

class Channel	{
private:
	std::string				_name;			// channel name
	std::vector<Client*>	_users;			// list of clients that are on the channel //pointing to already existing clients; could also just store the fds
	std::vector<Client*>	_operators;		// all operators (=moderators) of the channel

public:
// constructors
	Channel();
	Channel(std::string channel_name);
	Channel(const Channel& copy);
	~Channel();
	Channel& operator=(const Channel& other);

	void	addUser(Client* user);
	void	removeUser(Client* user);
	bool	hasUser(Client* user);
	void	addOperator(Client* user);
	void	removeOperator(Client* user);
	void	broadcast(std::string& msg, Client* sender);
};
