/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:38:24 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/16 17:30:43 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestLine.hpp"
#include "HttpParser.hpp"
#include <string>

RequestLine::RequestLine( std::vector<std::string> const & line ) : method( line[0] ), req_target( line[1] ), version( line[2] ) {
	if ( !HttpParser::notImplementedMethod( line[0] )) throw std::invalid_argument( E_501 );
	HttpParser::parseReqTarget( req_target );
}

RequestLine::RequestLine( RequestLine const & src ) { *this = src; }

RequestLine::RequestLine() {}

RequestLine::~RequestLine() {}

RequestLine &	RequestLine::operator=( RequestLine const & rhs ) {
	if ( this != &rhs ) {
		this->method = rhs.method;
		this->req_target = rhs.req_target;
		this->version = rhs.version;
	}
	return *this;
}

std::string	RequestLine::getMethod() const { return this->method; }

std::string	RequestLine::getReqTarget() const { return this->req_target; }

std::string	RequestLine::getVersion() const { return this->version; }

