/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:24:08 by nboer             #+#    #+#             */
/*   Updated: 2025/10/27 11:54:56 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/utilities.hpp"
#include "../inc/colors.hpp"
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
	if (c)
		std::cout << "[" << getTimeStamp() << "] " << GREEN << "[SERVER] " << RESET << c->getNick() << msg << std::endl;
	else
		std::cout << "[" << getTimeStamp() << "] " << GREEN << "[SERVER] " << RESET << msg << std::endl;
}

std::string toLower(const std::string& str) {
	std::string lowered = str;
	for (size_t i = (str[0] == '#' ? 1 : 0); i < str.size(); i++)
		lowered[i] = std::tolower(static_cast<unsigned char>(str[i]));
	return lowered;
}

std::vector<std::string>	split(const std::string& str, char delimiter) {
	std::vector<std::string>	result;
	std::stringstream			ss(str);
	std::string					item;

	while (std::getline(ss, item, delimiter)) {
		result.push_back(item);
	}
	return result;
}
