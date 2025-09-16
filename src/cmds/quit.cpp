/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:16 by aroux             #+#    #+#             */
/*   Updated: 2025/09/16 15:33:37 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

/* QUIT

Requirements:
  - Can be sent at any time.
  - Optional quit message may be included.

Behavior:
  - Disconnects the client from the server.
  - Removes client from all joined channels.
  - Sends PART/QUIT messages to other users in shared channels.
  - Closes client socket and cleans up resources.
  - Logs the client disconnection on the server.
*/