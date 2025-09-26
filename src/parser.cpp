/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:47:02 by nboer             #+#    #+#             */
/*   Updated: 2025/09/26 14:30:39 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parser.hpp"
#include <sstream>

void	toUpperCmd(ParsedCmd *data) {	
	for (size_t i = 0; i < data->cmd.size(); i++)
		data->cmd[i] = std::toupper(static_cast<unsigned char>(data->cmd[i]));
}

ParsedCmd	parseMsg(const std::string& raw) {
	ParsedCmd	data;
	std::string token;
	std::istringstream iss(raw);

	if (!raw.empty() && raw[0] == ':') {
			iss >> token;
			data.prefix = token.substr(1);
			// check prefix?
			std::cout << "prefix: " << data.prefix << std::endl;
	}
	iss >> data.cmd;
	while (iss >> token) {
		if (!token.empty() && token[0] == ':') {
			std::string rest_line;
			std::getline(iss, rest_line);
			data.args.push_back(token.substr(1) + rest_line);
			break;
		}
		else
			data.args.push_back(token);
	}
	return data;
}
