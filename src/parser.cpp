/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:47:02 by nboer             #+#    #+#             */
/*   Updated: 2025/09/13 18:09:36 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parser.hpp"
#include <sstream>

ParsedCmd parseMsg(const std::string& raw) {
	ParsedCmd	data;
	std::string arg;
	std::istringstream iss(raw);

	iss >> data.cmd; //first word
	while (iss >> arg) {
		if (arg[0] == ':')
			//todo
			std::cout << "combine the rest as a single argument" << std::endl;
		else
			data.args.push_back(arg);
		}
	
	return data;
}
