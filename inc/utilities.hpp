/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:27:04 by nboer             #+#    #+#             */
/*   Updated: 2025/09/16 14:47:44 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <cctype>
#include <ctime>

class Client;

int 		err_handler(std::string msg);
bool 		isnum(std::string arg);
std::string	getTimeStamp();
void		serverLog(Client* c, const std::string& msg);

