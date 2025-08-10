/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:03 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/10 14:18:39 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "HttpParser.hpp"
#include "HttpParserTester.hpp"
#include "RequestLine.hpp"
#include "ServerData.hpp"
#include "ServerManager.hpp"
#include "Uri.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

class	RequestLine;
class	Uri;
class	ServerManager;
struct	ClientConnection;

class HttpRequest
{
private:
	ClientConnection								*_connection;
	RequestLine										*req_line;
	Uri												*uri;
	std::pair<std::string, std::string>				 host;
	std::pair<std::string, std::string>				 _fullPath;
	std::map<std::string, std::vector<std::string> > headers;
	std::string										 body;

public:
	HttpRequest();
	HttpRequest(std::pair<int, std::string> incoming, ClientConnection *connection,
				ServerManager &server); // FABIO paired string with fd.
	HttpRequest(HttpRequest const &src);
	~HttpRequest();

	HttpRequest &operator=(HttpRequest const &rhs);

	void		receiveRequest();
	std::string getHttpMethod() const;
	std::string getRequestUri() const;
	std::string getPath() const;
	std::string getQuery() const;
	std::string getHttpVersion() const;
	std::pair<std::string, std::string>
		 getFullPath() const; // FABIO added this because I need full path for the response
	void setFullPath(ServerData const &serv);

	//PROVISOIR
	std::map<std::string, std::vector<std::string> >::iterator getHeader(std::string const &title);
};

#endif
