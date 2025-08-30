/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/08/30 16:24:49 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Utils.hpp"
#include "../../inc/LocationConf.hpp"
#include "../../inc/ServerData.hpp"
#include "../../inc/ABlockBase.hpp"

LocationConf::LocationConf()
{
	_autoindex = false;
}

LocationConf::LocationConf(ServerData const& S)
{
	_autoindex = false;
	_root = S.getRoot();
	_index = S.getIndex();
	_bodySize = S.getBodySize();
	_returnDirective = S.getReturnDirective();
	_errorPage = S.getErrorPage();
	_allowedMethods = S.getAllowedMethods();
	_cgiPass = S.getCgiPass();
}
  
LocationConf::~LocationConf() {} 

LocationConf::LocationConf(LocationConf const& other) : ABlockBase (other)
{
	*this = other;
}

LocationConf&	LocationConf::operator=(LocationConf const& other)
{
	if (this != &other)
	{
		ABlockBase::operator=(other);
		this->_key = other._key;
		_autoindex = other._autoindex;
	}
	return *this;
}

std::string const&	LocationConf::getRoot() const
{
	return _root;
}

bool    LocationConf::getAutoindex() const
{
	return _autoindex;
}

std::string const&	LocationConf::getKey() const
{
	return _key;	
}

void	LocationConf::setKey(std::string const value)
{
	_key = value;
	return;
}

size_t	LocationConf::fillAutoIndex(std::vector<std::string>& buffer, size_t i)
{
	// std::cout << BLUE << buffer[i] << RESET << std::endl;
	if ( i >= buffer.size() || buffer[i].empty())
		throw std::invalid_argument(" Parsing error, miss 'autoindex' argument\n");
	if (buffer[i] != "on" && buffer[i] != "off")
		throw std::invalid_argument(" Parsing error, 'autoindex' allow only"
			" 'on' or 'off' arguments\n");
	if (i + 1 >= buffer.size())
		throw std::invalid_argument(" Parsing error, miss semicolon after"
			" 'autoindex' argument\n");
	if (i + 1 < buffer.size() && buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error, 'autoindex' allow only"
			" one argument\n");
	if (checkFlag("autoindex"))
		throw std::invalid_argument(" Parsing error, only one 'autoindex'"
			" directive allowed by location block\n");
	if (buffer[i] == "on")
		_autoindex = true;
	else
		_autoindex = false;
	// std::cout << BLUE << _autoindex << RESET << std::endl;
	return (i + 2);
}
