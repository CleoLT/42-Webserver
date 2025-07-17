/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:59:58 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/17 15:50:14 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParser.hpp"
#include "RequestLine.hpp"
#include "ServerConf.hpp"
#include "Utils.hpp"
#include <iostream>
#include <cctype>
#include <cstring>

std::vector<std::string>	HttpParser::split( std::string const & str, char const delimiter ) {

	std::vector<std::string>	tokens;
	std::string					t;

	int	i = 0;
	int init = 0;
	int count = 0;

	std::string::const_iterator	it;
	std::string::const_iterator	ite = str.end();

	for ( it = str.begin(); it != ite; ++it ) {
		i++;
		count++;
		if ( *it == delimiter || it == ite - 1 ) {
			init = count - i;
			if ( it == ite - 1 && *it != delimiter ) i++;
			t = str.substr( init, i - 1);
			if ( !t.empty() ) tokens.push_back( t );
			i = 0;
		}
	}
	return tokens;
}

std::vector<std::string>	HttpParser::crlfSplit( std::string const & str ) {
	std::vector<std::string>	lines;
	size_t						start = 0, end;

    while ((end = str.find( "\r\n", start)) != std::string::npos) {
        lines.push_back(str.substr(start, end - start));
        start = end + 2;
    }
    return lines;
}

std::vector<std::string>	HttpParser::isspaceSplit( std::string const & str ) {

	std::vector<std::string>	tokens;
	std::string					t;

	int	i = 0;
	int init = 0;
	int count = 0;

	std::string::const_iterator	it;
	std::string::const_iterator	ite = str.end();

	for ( it = str.begin(); it != ite; ++it ) {
		i++;
		count++;
		if ( std::isspace( *it ) || it == ite - 1 ) {
			init = count - i;
			if ( it == ite - 1 && *it != std::isspace( *it )) i++;
			t = str.substr( init, i - 1);
			if ( !t.empty() ) tokens.push_back( t );
			i = 0;
		}
	}
	return tokens;
}

bool	HttpParser::isAsciiPrintable( std::string const & str ) {
	std::string::const_iterator	it, ite = str.end();

	for ( it = str.begin(); *it != *ite; ++it ) { 
		if ( *it == '\t' ) continue;
		if ( *it > 126 || *it < 32 )
			return false;
	}
	return true;
}

bool	HttpParser::isUnreservedForUri( char c ) {
	if ( c == '-' || c == '_' || c == '.' || c == '~' || isalnum( c ))
		return true;
	return false;
}

bool	HttpParser::isReservedForUri( char c ) {
	if ( c == ':' || c == '/' || c == '?' || c == '#' || c == '[' || c == ']' || c == '@' )
        return true; //gen-delims

    if ( c == '!' || c == '$' || c == '&' || c == '\\' || c == '(' || c == ')' || c == '*' || \
		c == '+' || c == ',' || c == ';' || c == '=')
        return true; //sub-delims

    return false;
}

bool	HttpParser::isTokenChar( char c ) {
	const std::string specials = "!#$%&'*+-.^_`|~";

    return std::isalnum(static_cast<unsigned char>(c)) || specials.find(c) != std::string::npos;
}

bool HttpParser::isHexChar( char c ) {
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

std::string	HttpParser::toLower( std::string const & str ) {
	
	std::string					result = str;
	std::string::iterator	it, ite = result.end();
	
	for ( it = result.begin(); it != ite; ++it )
		*it = std::tolower( static_cast<unsigned char>( *it ));

	return result;
}

std::string	trimSpaceAndTab( std::string & str ) {
	std::string::reverse_iterator	rit, rite = str.rend();

	for ( rit = str.rbegin(); rit != rite; ++rit ) {
		if ( *rit == ' ' || *rit == '\t' )
			str.erase( str.size() - 1 );
		else
			break;
	}

	while (!str.empty() && (str[0] == ' ' || str[0] == '\t'))
		str.erase(0, 1);
	
//	std::cout << "\"" << str << "\"" << std::endl;

	return str;
}

std::vector<std::string>	HttpParser::parseHttpMessage( std::string const & message, std::string & host_str ) {

	std::vector<std::string> lines = crlfSplit( message );

	if ( lines.empty() || !lines.back().empty()) throw std::invalid_argument( E_400 );

	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator	ite = lines.end();
	std::string::const_iterator			s_it, s_ite;

	int header = 0, host = 0;

	for ( it = lines.begin(); it != ite; ++it ) {
		if ( header == 0 && (*it).empty()) continue;
		if ( header > 0 && !(*it).empty()) 
			if ( std::isspace( (*it)[0] )) throw std::invalid_argument( E_400 );
		s_ite = (*it).end();
		for ( s_it = (*it).begin(); s_it != s_ite; ++s_it )
			if ( *s_it == '\r' ) throw std::invalid_argument( E_400 );
		if ( strncmp( toLower( *it ).c_str(), "host:", 5 ) == 0 ) {
			host_str = (*it).substr( 5, (*it).length() - 5 );
			host++;
		}
		header++;
		if ( !isAsciiPrintable( *it )) throw std::invalid_argument( E_400 ); 
																			 
	}
	if ( host != 1 ) throw std::invalid_argument( E_400 );

	return lines;
}

RequestLine	HttpParser::parseRequestLine( std::string const & line ) {
	
	std::string::const_iterator	s_it, s_ite = line.end();
	int							spaces = 0;

	for ( s_it = line.begin(); s_it != s_ite; ++s_it )
		if ( *s_it == ' ' ) spaces++;
	if ( spaces != 2 ) throw std::invalid_argument( E_400 );

	std::vector<std::string>	tokens = split( line, ' ' );
	
	if ( tokens.size() != 3 ) throw std::invalid_argument( E_400 );

	if ( tokens[1].length() > 8000 ) throw std::invalid_argument( E_414 );

	if ( tokens[2] != "HTTP/1.1" ) throw std::invalid_argument( E_400 );

	return	 RequestLine( tokens );
}

void	HttpParser::parseReqTarget( std::string & uri ) { //request target = uri
	std::string::iterator	it, ite = uri.end();

	if ( uri.at( 0 ) && uri.at( 0 ) != '/' ) throw std::invalid_argument( E_400 );

	for ( it = uri.begin(); it != ite; ++it ) {
		if ( !isUnreservedForUri( *it ) && !isReservedForUri( *it ) && *it  != '%' )
			throw std::invalid_argument( E_400 );
		if ( *it == '%' ) {
			if ( it + 1 == ite || it + 2 == ite ) throw std::invalid_argument( E_400 );
			if ( !isHexChar( *( it + 1 )) || !isHexChar( *( it + 2 ))) throw std::invalid_argument( E_400 );
			it ++;
		}
	}
}

std::string	HttpParser::parsePath( std::string const & uri ) {
	std::size_t	foundQ = uri.find( "?" );
	std::size_t	foundF = uri.find( "#" );

	if ( foundQ != std::string::npos ) return uri.substr( 0, foundQ );
	if ( foundF != std::string::npos ) return uri.substr( 0, foundF );
	return uri;
}

std::string	HttpParser::parseQuery( std::string const & uri ) {
	std::size_t	foundQ = uri.find( "?" );
	std::string	tmp;

	if ( foundQ != std::string::npos )
		tmp =  uri.substr( foundQ + 1, uri.length());
	else
		return "";

	std::size_t	foundF = tmp.find( "#" );

	if ( foundF != std::string::npos )
		return tmp.substr( 0, foundF );
	else
		return tmp;
}

std::string	HttpParser::parseFragment( std::string const & uri ) {

	std::size_t	found = uri.find( "#" );

	if ( found != std::string::npos )
		return uri.substr( found + 1, uri.length());
	else
		return "";

}

bool	HttpParser::notImplementedMethod( std::string const & method ) {
	const char *	valid_method[] = { "GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "TRACE", "CONNECT" };

	for ( int i = 0; i < 8; i++ )
		if ( method == valid_method[i] ) 
			return true;
	return false;
}

ServerConf const &	HttpParser::checkIfServerExist( std::vector<ServerConf> const & servers, std::string const & host ) {
	std::vector<ServerConf>::const_iterator	it, ite = servers.end();
	std::vector<std::string>::const_iterator	it_name, ite_name; 

	for (it = servers.begin(); it != ite; ++it) {
		const std::vector<std::string>& names = it->getServerName();
		for (it_name = names.begin(); it_name != names.end(); ++it_name) {
			if (*it_name == host)
				return *it;
		}
	}	
	throw std::invalid_argument( E_421 );

}

void	HttpParser::checkIfPathExist( std::map<std::string, LocationConf> & location, std::string const & path ) {
	std::map<std::string, LocationConf>::iterator	it = location.find( path );

	if ( it == location.end() ) throw std::invalid_argument( E_404 );

	std::cout << "PATH EXIST IN SERVER: " << (*it).first << std::endl;
}


void	HttpParser::notAllowedMethod( std::map<std::string, LocationConf>::iterator location, 
	std::vector<std::string> const & allowed_serv, std::string const & method ) {

	std::vector<std::string> const &	allowed_loc = location->second.getAllowedMethods();

	if ( !allowed_loc.empty()) {
		for ( std::size_t i = 0; i < allowed_loc.size(); i++ )
			if ( allowed_loc[i] == method ) return;
		throw std::invalid_argument( E_405 );
	}

	if ( !allowed_serv.empty()) {
		for ( std::size_t i = 0; i < allowed_serv.size(); i++ )
			if ( allowed_serv[i] == method ) return;
		throw std::invalid_argument( E_405 );
	}
}

std::pair<std::string, std::string>	HttpParser::parseHost( std::string const & str ) {
	std::string	tmp( str );
	std::string	second = "";
	std::string	first = trimSpaceAndTab( tmp );
	std::size_t	found = tmp.find( ':' );

	if ( found != std::string::npos ) {
		first = tmp.substr( 0, found ); //NAME
		second = tmp.substr( found + 1, tmp.size() - found ); //PORT
		if ( second.empty()) throw std::invalid_argument( E_400 );
		if ( !isInt( second )) throw std::invalid_argument( E_400 );
	}


	
	return  std::make_pair( first, second );
}
