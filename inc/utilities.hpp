/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:27:04 by nboer             #+#    #+#             */
/*   Updated: 2025/09/28 13:50:54 by nboer            ###   ########.fr       */
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

int 						err_handler(std::string msg);
bool 						isnum(std::string arg);
std::string					getTimeStamp();
void						serverLog(Client* c, const std::string& msg);
std::string					toLower(const std::string& str);
std::vector<std::string>	split(const std::string& str, char delimiter);

