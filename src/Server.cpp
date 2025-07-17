/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:40:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/16 11:57:15 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "HttpRequest.hpp"
#include "ParsingConf.hpp"

// Server::Server(): _createdTime(time(NULL))
// { 
// 	std::cout << utilsTimestamp() << "Server Created" << std::endl;
// }

Server::Server(ParsingConf& P) : _createdTime(time(NULL)), 	_serversList(P.getServers()), _defaultErrorPages(defaultErrorPages())
{
	std::cout << utilsTimestamp() << "Server Created" << std::endl;
}

Server::Server(Server const &copy) : _serversList(copy._serversList), _defaultErrorPages(copy._defaultErrorPages)
{
	*this = copy;
}

Server &Server::operator=(Server const &copy)
{
	(void)copy;
	return *this;
}

Server::~Server()
{
	std::cout << utilsTimestamp() << "Server destroyed" << std::endl;
}

time_t Server::servTimeGet() // delete??
{
	return this->_createdTime;
}

void Server::servSetup()
{
	/* creating socket */
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFd < 0)
	std::cerr << "Socket creation error" << std::endl;
	
	
	/* setting socket to non blocking as by subject 
	using fcntl() first to get the flags (F_GETFL)
	and then setting them (F_SETFL)*/
	int flags = fcntl(_socketFd, F_GETFL, 0);
	if (fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK) < 0)
	std::cerr << "Nonblocking setup error" << std::endl;
	
	
	/* using the sockaddr_in structure to store the
	socket address information */
	std::memset(&_servAddr, 0, sizeof(_servAddr));	
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_port = htons(8080);
	_servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // for now set to localhost
	
	/* binding the socket we created to the address we
	have set above */
	if (bind(_socketFd, (struct sockaddr*)&_servAddr, sizeof(_servAddr)) < 0)
	{
		std::cerr << "Socket binding error" << std::endl;
		close(_socketFd);
	}
	
	/* setup the socket to listen */
	if (listen(_socketFd, 10) < 0) // change back to SOMAXCONN?
	{
		std::cerr << "Listen socket setup error" << std::endl;
		close(_socketFd);
	}
}

void Server::servRun()
{	
	std::cout << utilsTimestamp() << "Server started" << std::endl;
	
	_pollFd[0].fd = _socketFd;
	_pollFd[0].events = POLLIN;
	
	std::cout << utilsTimestamp() << "Waiting for connection on port 8080" << std::endl;
	
	while (true)
	{
		int check = poll(_pollFd, 1, 5000);
		if (check < 0)
		{
			std::cerr << "Poll error" << std::endl;
			break ;
		}
		else if (check == 0)
		{
			std::cout << utilsTimestamp() << "Still waiting for connection" << std::endl;
			continue ;
		}
		
		if (_pollFd[0].revents & POLLIN)
		{
			socklen_t clientLen = sizeof(_clientAddr);
			_clientFd = accept(_socketFd, (struct sockaddr *)&_clientAddr, &clientLen);
			if (_clientFd >= 0)
			{
				std::cout << utilsTimestamp() << "New connection accepted" << std::endl;
				char buffer[4096];
				ssize_t bytes = recv(_clientFd, buffer, sizeof(buffer), 0);
				if (bytes > 0)
					buffer[bytes] = '\0';
		/*		std::cout << utilsTimestamp() << "Request content:\n*****\n" << std::endl;
				std::cout << buffer;
				std::cout << "*****" << std::endl;
		*/

				HttpRequest	req = HttpRequest( buffer, *this );
				
				std::string response = 
				"HTTP/1.1 200 OK\n"
				"Content-Type: text/html\n"
				"Content-Length: 85\n"
				"\n"
				"<html><body><h1>Bonjour!</h1></body></html>";
			
				send(_clientFd, response.c_str(), response.size(), 0);
			
				close(_clientFd);
			}
		}
	}
	close(_socketFd);
}

std::vector<ServerConf> const&	Server::getServersList() const
{
	return _serversList;
}

std::map<int, std::pair<std::string, std::string> > const&	Server::getDefaultErrorPages() const
{
	return _defaultErrorPages;
}


/* listening testing methods:
netstat -an | grep 8080
ss -ltn // Linux Only
telnet 127.0.0.1 8080
http://localhost:8080 // via browser
*/
