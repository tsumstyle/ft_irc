/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:26:38 by aroux             #+#    #+#             */
/*   Updated: 2025/09/13 16:26:16 by nboer            ###   ########.fr       */
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

public:
// constructors
	Channel();
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
