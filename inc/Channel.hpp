/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:26:38 by aroux             #+#    #+#             */
/*   Updated: 2025/09/08 12:06:07 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

#include <string>
#include <vector>
#include "Client.hpp"

class Channel	{
private:
	std::string			_name;			// channel name
	std::vector<Client>	clients_list;	// list of client fds that are on the channel 
	std::vector<Client>	operators_list;	// all operators (=moderators) of the channel

public:
// constructors
	Channel();
	Channel(const Channel& copy);
	~Channel();
	Channel& operator=(const Channel& other);
};

Channel::Channel(/* args */)
{
}

Channel::~Channel()
{
}
