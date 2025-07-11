/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:38:09 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/22 16:13:24 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	REQUESTLINE_HPP
#define	REQUESTLINE_HPP

#include <string>
#include <vector>

class	RequestLine	{
private:
	std::string	method;
	std::string	uri;
	std::string	path;
	std::string	query;
	std::string	version;
	
public:
	RequestLine( std::vector<std::string> const & line );
	RequestLine( RequestLine const & src );
	RequestLine();
	~RequestLine();
	
	RequestLine &	operator=( RequestLine const & rhs );
	
	std::string	getMethod() const;
	std::string	getUri() const;
	std::string	getPath() const;
	std::string getQuery() const;
	std::string	getVersion() const;
};

#endif
