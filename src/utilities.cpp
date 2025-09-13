/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:24:08 by nboer             #+#    #+#             */
/*   Updated: 2025/09/13 16:24:34 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/utilities.hpp"

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
