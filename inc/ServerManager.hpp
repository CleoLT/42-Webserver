/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:30:50 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/10 14:12:54 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "HttpRequest.hpp"
#include "ParsingConf.hpp"
#include "PrintLog.hpp"
#include "Response.hpp"
#include "ServerData.hpp"
#include "Utils.hpp"

struct ClientConnection
{
	std::string		   fullRequest;
	int				   clientFd;
	struct sockaddr_in clientAddr;
	socklen_t		   clientLen;
	size_t			   socketIndex;

	ClientConnection() : clientFd(-1), clientLen(sizeof(clientAddr)), socketIndex(0)
	{
		std::memset(&clientAddr, 0, sizeof(clientAddr));
	}
};

class Response;
class ServerManager
{
private:
	bool								   _running;
	int									   _inputFd;
	std::string							   _input;
	// Response							   _response;
	std::vector<ServerData>				   _serverData;
	std::vector<int>					   _socketFd;
	std::vector<struct sockaddr_in>		   _servAddr;
	int									   _reqCount;
	int									   _rspCount;
	std::set<std::pair<int, std::string> > _uniqueListens;
	ServerManager();
	ServerManager(ServerManager const &copy);
	ServerManager &operator=(ServerManager const &copy);

public:
	ServerManager(ParsingConf &parsData);
	~ServerManager();

	void						servSetup();
	void						servRun();
	void						servListen(std::pair<int, std::string> _listens);
	std::vector<ServerData>		getServersList() const;
	struct pollfd			   *servPoll(size_t totalSocket);
	std::pair<int, std::string> getSocketData(int socketFd);
	void						servIncoming(struct pollfd *polls, const size_t socketsize);
	std::set<std::pair<int, std::string> > getUniqueListens();
	int									   getReqCount() const;
	int									   getRspCount() const;
	void								   servQuit();
	void								   servInput();
	void								   servRespond(HttpRequest req, ClientConnection connection, std::pair<int, std::string> incoming);
	// bool								   servReceive(ClientConnection &connection);
	// TODO create servQuit() to stop all servers;
};

#endif
