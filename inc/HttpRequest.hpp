/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:03 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/17 13:08:44 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTTPREQUEST_HPP
#define	HTTPREQUEST_HPP


#include "Server.hpp"
#include <string>

class	RequestLine;
class	Uri;

class	HttpRequest	{
private:	
	RequestLine	*						req_line;
	Uri *								uri;
	std::pair<std::string, std::string>	host;
//	RequestHeaders	headers;
//	RequestBody		body;	

public:
	HttpRequest();
	HttpRequest( std::string const & message, Server & server );
	HttpRequest( HttpRequest const & src );
	~HttpRequest();

	HttpRequest &	operator=( HttpRequest const & rhs );	
	
	std::string	getHttpMethod() const;
	std::string	getRequestUri() const;
	std::string	getPath() const;
	std::string	getQuery() const;
	std::string	getHttpVersion() const;
};

#endif
