/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:51:25 by nboer             #+#    #+#             */
/*   Updated: 2025/09/14 14:47:48 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>

struct ParsedCmd{
	std::string					cmd;
	std::vector<std::string>	args;
	std::string					prefix;
};

ParsedCmd	parseMsg(const std::string& raw);
void		toUpperCmd(ParsedCmd *data);

