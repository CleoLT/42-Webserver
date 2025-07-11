/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:54 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/08 15:21:33 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>

#include "Utils.hpp"

class Server
{
	private:
		time_t _createdTime;
		int _socketFd;
		int _clientFd;
		struct sockaddr_in _servAddr;
		struct sockaddr_in _clientAddr;
		struct pollfd _pollFd[1];
		
	public:
		Server();
		Server(Server const &copy);
		Server &operator=(Server const &copy);
		~Server();

		void servRun();
		void servSetup();
		
		time_t servTimeGet();
};

#endif