/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/10 14:38:49 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : req_line( NULL ), uri( NULL ) { 
	HttpParserTester::parseHttpMessageTest();
	HttpParserTester::parseRequestLineTest();
	HttpParserTester::parseHostTest();
	HttpParserTester::parseUriTest();

}

HttpRequest::HttpRequest(std::pair<int, std::string> incoming, ClientConnection *connection, ServerManager &server) : _connection(connection), req_line(NULL), uri(NULL) {
	try {

		// std::cout << message << std::endl;
		receiveRequest();
		std::string							tmp_host;
		std::vector<std::string>			lines = HttpParser::parseHttpMessage( _connection->fullRequest, tmp_host );
		std::vector<std::string>::iterator	it = lines.begin();
		std::vector<std::string>::iterator	ite = lines.end();

		host = HttpParser::parseHost( tmp_host ); // ACABAR EL PARSEO DE HOST CON FCT DE EMILIE  

		// std::cout << "tmp_host: \"" << tmp_host << "\" /host pair first: \"" << host.first;
		// std::cout << "\", second: \"" << host.second <<  "\"" << std::endl;

		printRaw(_connection->fullRequest);
		while ( it != ite && (*it).empty() )
			++it;

		req_line = new RequestLine( HttpParser::parseRequestLine( *it ));

		uri = new Uri( req_line->getReqTarget(), host.first );

		ServerData serv = HttpParser::checkIfServerExist( server.getServersList(), incoming );
		setFullPath(serv);
		HttpParser::checkIfPathExist( serv.getLocations(), uri->getPath()); // 404 not found si el uri no existe en servidor
		HttpParser::notAllowedMethod( serv.getItLocations( uri->getPath()), serv.getAllowedMethods(), req_line->getMethod());

		++it;
	

		//HEADERS
		while ( it != ite && !(*it).empty()) {
			std::vector<std::string>	tmp = HttpParser::split( *it, ':' );
			std::vector<std::string>	value = HttpParser::split( tmp[1], ',' );

			headers[ tmp[0] ] = value;

			++it;
		}

		//PRINTHEADERS
		std::map<std::string, std::vector<std::string> >::iterator	it_h, ite_h = headers.end();
		std::vector<std::string>::iterator							it_v, ite_v;

		for ( it_h = headers.begin(); it_h != ite_h; ++it_h ) {
			std::cout << "key: " << it_h->first << "	 values: ";
			ite_v = it_h->second.end();
			for ( it_v = it_h->second.begin(); it_v != ite_v; ++it_v )
				std::cout << *it_v << " ";
			std::cout << std::endl;
		}

		//BODY
		std::size_t found = _connection->fullRequest.rfind( "\r\n\r\n" );
		body = _connection->fullRequest.substr( found + 4 , _connection->fullRequest.length() - found + 4);

		std::cout << "Body: " << body << std::endl;


	} catch ( std::invalid_argument e ) {
		std::cout << e.what() << std::endl;
	}
}

HttpRequest::HttpRequest( HttpRequest const & src ) { *this = src; }

HttpRequest::~HttpRequest() {
	if ( req_line )
		delete req_line;
	if ( uri )
		delete uri;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& rhs) {
    if (this != &rhs) {
        // if (req_line) delete req_line; // TODO segfault here
        req_line = rhs.req_line ? new RequestLine(*rhs.req_line) : NULL;
		// if (uri) delete uri;
        uri = rhs.uri ? new Uri(*rhs.uri) : NULL;
		//anadir HOST PAIR 
		//anadir header y body
	}
    return *this;
}


std::map<std::string, std::vector<std::string> >::iterator HttpRequest::getHeader( std::string const & title ) { 
	std::map<std::string, std::vector<std::string> >::iterator it = this->headers.find( title ); 

	if ( it == this->headers.end())
		std::cout << "Be careful! " << title << " header dont exist!" << std::endl;

	return it;
}

std::string	HttpRequest::getHttpMethod() const { return this->req_line->getMethod(); }

std::string	HttpRequest::getRequestUri() const { return this->uri->getUri(); }

std::string	HttpRequest::getPath() const { return this->uri->getPath(); }

/* TODO we need to check also for the case when we get an absolute path i think
 getFullPath() just adds the root to the path in the Uri from the request */

void HttpRequest::setFullPath(ServerData const &serv)
{
	_fullPath = std::make_pair<std::string, std::string>(serv.getRoot(), uri->getPath());
}

std::pair<std::string, std::string> HttpRequest::getFullPath() const
{
	return _fullPath;
}

std::string	HttpRequest::getQuery() const { return this->uri->getQuery(); }

std::string	HttpRequest::getHttpVersion() const { return this->req_line->getVersion(); }

void	HttpRequest::receiveRequest()
{
	bool isComplete = false;

	if (_connection->clientFd >= 0)
	{
		printBoxMsg("New connection accepted");

		char	  buffer[4096]; // HACK i put 4096, but i don't know if it's right
		int		  attempts	  = 0;
		const int maxAttempts = 100;

		while (!isComplete && attempts < maxAttempts)
		{
			ssize_t bytes = recv(_connection->clientFd, buffer, sizeof(buffer) - 1, 0);
			if (bytes > 0)
			{
				buffer[bytes] = '\0';
				_connection->fullRequest += buffer;
				if (_connection->fullRequest.find("\r\n\r\n") != std::string::npos) // TODO check what happens with other bodies in POST
					isComplete = true;
			}
			else if (bytes == 0)
			{
				_connection->fullRequest = "";
				return ;
			}
			else if (bytes < 0)
			{
				usleep(10000); // wait and try again
				attempts++;
				continue;
			}
		}
	}
	// std::cout << GREEN << connection.fullRequest << RESET << std::endl; // TODO delete when done
	printRaw(_connection->fullRequest);
}
