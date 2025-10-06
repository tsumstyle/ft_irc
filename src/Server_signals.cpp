/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_signals.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:04:30 by aroux             #+#    #+#             */
/*   Updated: 2025/10/06 14:19:38 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/utilities.hpp"
#include "../inc/parser.hpp"

void	Server::setSignals() {
    std::cout << "DEBUG: Setting up signal handlers" << std::endl;
    std::cout << "DEBUG: _instance = " << _instance << std::endl;
	signal(SIGINT, Server::signalHandler);		//ctrl+C
	std::cout << "DEBUG: SIGINT handler set" << std::endl;
	signal(SIGTERM, Server::signalHandler);		//ctrl+D
	std::cout << "DEBUG: SIGTERM handler set" << std::endl;
	signal(SIGQUIT, Server::signalHandler);		//ctrl+antislash
	std::cout << "DEBUG: SIGQUIT handler set - signal setup complete" << std::endl;
}

void	Server::signalHandler(int signal) {
	std::string signalName;
	switch(signal) {
		case SIGINT: 
			signalName = "SIGINT (Ctrl+C)";
			break;
		case SIGTERM: 
			signalName = "SIGTERM";
			break;
		case SIGQUIT: 
			signalName = "SIGQUIT (Ctrl+\\)";
			break;
		default:	  
			signalName = "Unknown signal";
			break;
	}
	if (_instance)	{		// signal handlers must be in static functions. but static function cannot access non-static member variables directly, so we use a static pointer to the current instance
		std:: string msg = "Received " + signalName + " (" + toString(signal) + "), initiating clean shutdown...";
		serverLog(NULL, msg);
		_instance->stop();
	}
	std::signal(signal, SIG_DFL);		// prevent multiple signal handling
}