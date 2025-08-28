/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/08/28 20:02:52 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ServerData.hpp"

ServerData::ServerData()
{
	_listens.push_back(std::make_pair(8080, "127.0.0.1"));
}
  
ServerData::~ServerData() {} 

ServerData::ServerData(ServerData const& other) : ABlockBase(other)
{
	*this = other;
}

ServerData&	ServerData::operator=(ServerData const& other)
{
	if (this != &other)
	{
  		ABlockBase::operator=(other);
		this->_listens = other._listens;
		this->_serverName = other._serverName;
		this->_locations = other._locations;     
	}
	return *this;
}

bool    ServerData::getAutoindex() const
{
	return _autoindex;
}

std::map<std::string, LocationConf>&	ServerData::getLocations() const
{
	return const_cast<std::map<std::string, LocationConf>&>(_locations);
}

std::map<std::string, LocationConf>::iterator	ServerData::getItLocations(std::string const& key)
{
	return _locations.find(key);
}

std::vector<std::pair<int, std::string> >	ServerData::getListens() const
{
	return _listens;
}

std::vector<std::string>	ServerData::getServerName() const
{
	return _serverName;
}

size_t ServerData::fillListens(std::vector<std::string>& buffer, size_t i)
{
	int port = 0;
	std::string ip = "";
	
	if (i >= buffer.size() || buffer[i].empty() || i + 1 >= buffer.size()
		|| buffer[i + 1].empty() || buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error with 'listen' argument,"
		" need one argument followed by a semicolon\n"); 
	if (isInt(buffer[i]))
	{
		port = strToInt(buffer[i]);
		ip = "127.0.0.1";
	}
	else if (isLocal(buffer[i]))
	{
		port = strToInt(buffer[i].substr(10));;
		ip = "127.0.0.1";
	}
	else if (isSocket(buffer[i]))
	{
		if (!checkSocketAddress(buffer[i]))
			throw std::invalid_argument(" Parsing error, 'listen' socket address is not correct\n"); 
		ip = socketToIp(buffer[i]);
		port = socketToPort(buffer[i]);
	}
	else
		throw std::invalid_argument(" Parsing error, 'listen' argument is not correct\n"); 
	if (checkFlag("listen"))
	{
		for (size_t i = 0; i < _listens.size(); i++)
		{
			if (_listens[i].first == port && _listens[i].second == ip)
				throw std::invalid_argument(" Parsing error, duplicate 'listen' arguments\n");
		}	
		_listens.push_back(std::make_pair(port, ip));
	}
	else
		_listens[0] = std::make_pair(port, ip);
	return (i + 2);   
}

size_t	ServerData::fillServerName(std::vector<std::string>& buffer, size_t i)
{
	if (checkFlag("server_name"))
		throw std::invalid_argument(" Parsing error, only one 'server_name'"
			" directive allowed by server block\n"); 
	if (i >= buffer.size() || buffer[i].empty() || buffer[i] == ";")
		throw std::invalid_argument(" Parsing error, miss 'server_name' arguments\n");
	if (!_serverName.empty())
		_serverName.clear(); //supprimer l'arg par defaut (vide tout le vector)
	while (i < buffer.size())
	{
		if (buffer[i] == ";")
			break;
		if (buffer[i] == "{" || buffer[i] == "}")
			throw std::invalid_argument(" Parsing error, miss semicolon after"
				" 'server_name' directive\n");
		for (size_t j = 0; j < _serverName.size(); j++)
		{
			if (buffer[i] == _serverName[j])
				throw std::invalid_argument(" Parsing error, 'server_name' argument duplicated\n");
		}
		_serverName.push_back(buffer[i]);
		i++;
	}
	if (!checkDns(_serverName))
		throw std::invalid_argument(" Parsing error, 'server_name' arguments"
			" need to follow DNS's rules\n");
    return (i + 1);   
}
