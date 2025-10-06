/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:27:04 by nboer             #+#    #+#             */
/*   Updated: 2025/10/06 11:56:46 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <cctype>
#include <ctime>
#include <sstream>
#include <vector>

class Client;

template<typename T>
std::string toString(const T& value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

int 						err_handler(std::string msg);
bool 						isnum(std::string arg);
std::string					getTimeStamp();
void						serverLog(Client* c, const std::string& msg);
std::string					toLower(const std::string& str);
std::vector<std::string>	split(const std::string& str, char delimiter);

