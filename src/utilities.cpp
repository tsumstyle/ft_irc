/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:24:08 by nboer             #+#    #+#             */
/*   Updated: 2025/09/16 15:13:53 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/utilities.hpp"
#include "../inc/Client.hpp"

int	err_handler(std::string msg) {
	std::cerr << "Error: " << msg << std::endl;
	return (1);
}

bool isnum(std::string arg) {
	std::string::const_iterator it = arg.begin();
	while (it != arg.end() && std::isdigit(*it))
		++it;
	return !arg.empty() && it == arg.end();
}

std::string	getTimeStamp() {
	std::time_t	t = std::time(NULL);
	char	buffer[20];
	std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&t));
	return std::string(buffer);
}

void	serverLog(Client* c, const std::string& msg) {
	std::cout << "[" << getTimeStamp() << "] [SERVER] " << c->getNick() << msg << std::endl;
}


