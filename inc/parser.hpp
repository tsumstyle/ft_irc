/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:51:25 by nboer             #+#    #+#             */
/*   Updated: 2025/09/29 14:56:22 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>

struct ParsedCmd{
	std::string					cmd;
	std::vector<std::string>	args;
	std::string					prefix;
	bool						lastTokenHasColon;
};

ParsedCmd	parseMsg(const std::string& raw);
void		toUpperCmd(ParsedCmd *data);

