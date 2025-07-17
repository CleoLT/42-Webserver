/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:59:53 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/17 13:09:51 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTTPARSER_HPP
#define	HTTPARSER_HPP

#define E_400 "400 Bad Request"
#define E_404 "404 Not Found"
#define E_405 "405 Method Not Allowed"
#define E_414 "414 URI Too Long"
#define E_421 "421 Misdirected Request"
#define E_501 "501 Not Implemented"

#include "LocationConf.hpp"
#include <string>
#include <vector>

class	RequestLine;
class	ServerConf;

class	HttpParser	{
private:
	HttpParser();
	HttpParser( HttpParser const & src );
	~HttpParser();

	HttpParser &	operator=( HttpParser const & rhs );

public:
	static std::vector<std::string>	split( std::string const & str, char const delimiter );
	static std::vector<std::string>	crlfSplit( std::string const & str );
	static std::vector<std::string>	isspaceSplit( std::string const & str );
	static bool						isAsciiPrintable( std::string const & str );
	static bool						isUnreservedForUri( char c );
	static bool						isReservedForUri( char c );
	static bool						isHexChar( char c );
	static std::string				toLower( std::string const & str );
	static bool						isTokenChar( char c );

	static RequestLine				parseRequestLine( std::string const & line );
	static void						parseReqTarget( std::string & uri ); 
	static std::string				parsePath( std::string const & uri );
	static std::string				parseQuery( std::string const & uri );
	static std::string				parseFragment( std::string const & uri );
	static bool						notImplementedMethod( std::string const & method );
	static ServerConf const &		checkIfServerExist( std::vector<ServerConf> const & servers, std::string const & host );
	static void						checkIfPathExist( std::map<std::string, LocationConf> & loc, std::string const & path );
	static void						notAllowedMethod( std::map<std::string, LocationConf>::iterator loc, 
									std::vector<std::string> const & serv_meth, std::string const & meth);
	static std::pair<std::string, std::string>	parseHost( std::string const & str );

	static std::vector<std::string>	parseHttpMessage( std::string const & str, std::string & host );
};

#endif
