/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:38:09 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/16 17:30:43 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	REQUESTLINE_HPP
#define	REQUESTLINE_HPP

#include <string>
#include <vector>

class	RequestLine	{
private:
	std::string	method;
	std::string	req_target;
	std::string	version;

	RequestLine();
	
public:
	RequestLine( std::vector<std::string> const & line );
	RequestLine( RequestLine const & src );
	~RequestLine();
	
	RequestLine &	operator=( RequestLine const & rhs );
	
	std::string	getMethod() const;
	std::string	getReqTarget() const;
	std::string	getVersion() const;
};

#endif
