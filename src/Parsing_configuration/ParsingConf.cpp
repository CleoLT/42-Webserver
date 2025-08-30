/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConf.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:21:07 by esellier          #+#    #+#             */
/*   Updated: 2025/08/30 16:27:45 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ParsingConf.hpp"
#include "ServerData.hpp"
#include "LocationConf.hpp"

ParsingConf::ParsingConf() {}

ParsingConf::~ParsingConf() {}

void	ParsingConf::print_tokens(std::vector<std::string>& buffer)
{
	for (size_t i = 0; i < buffer.size(); i++)
		std::cout << BLUE << buffer[i] << std::endl;
	return;
}

std::vector<ServerData>&	ParsingConf::getServers()
{
	return servers;
}

void	ParsingConf::checkLocation(std::string& line)
{
	size_t		i = 0;
	size_t			j;
	std::string first;
	
	while(i < line.length())
	{
		if (line[i] != 9 && line[i] != 32)
		{
			j = i;
			while (i < line.length() && line[i] != 9 && line[i] != 32)
				i++;
			first = line.substr(j, i - j);
			break;
		}
		i++;
	}
	if(first == "location")
	{
		while (i < line.length())
		{
			if (line[i] != 9 && line[i] != 32)
			{
				j = i;
				while (i < line.length() && line[i] != 9 && line[i] != 32)
					i++;
				first = line.substr(j, i - j);
				if  (first == "{" || first == "}" || first == ";" )
					throw std::invalid_argument("Parsing error, miss an argument"
						" after 'location'\n");
				if (first[0] != '/')
					throw std::invalid_argument("Parsing error, wrong location's"
						" direction\n");
				return;
			}
			i++;
		}
		throw std::invalid_argument("Parsing error, miss an argument after 'location'\n");
	}
	return;
}

void	ParsingConf::checkSemicolon(std::string& line)
{
	size_t j = line.find(';');
	if(j == std::string::npos)
		return;
	for (size_t i = j + 1; i < line.length(); i++)
	{
		if (line[i] != 9 && line[i] != 32 && line[i] != '}')
			throw std::invalid_argument("Parsing error, no argument accepted"
				" after the semicolon\n");
		if (line[i] == '}')
		{
			i++;
			while (line[i])
			{
				if (line[i] != 9 && line[i] != 32)
					throw std::invalid_argument("Parsing error, no argument"
						" accepted after the brace\n");
				i++;
			}
			break;
		}
	}
	size_t i = 0;
	while (line[i] && line[i] < 33)
		i++;
	if (line[i] == ';')
		throw std::invalid_argument("Parsing error, need argument before the semicolon\n");
	line.insert(j, " ");
	return;
}

void	ParsingConf::checkParenthesis(std::string& line)
{
	size_t i = line.find('{');
	size_t j = line.find('}');
	
	if(i == std::string::npos && j == std::string::npos)
		return ;
	for (size_t i = 0; i < line.length(); i++)
	{
		if ((line[i] == '{' || line[i] == '}') && (i + 1 < line.length()
			&& (line[i + 1] == '{' || line[i + 1] == '}')))
			throw std::invalid_argument("Parsing error, parenthesis cannot be"
			 	" followed by another parenthesis\n");
	}
}

void	ParsingConf::doParsing(std::string line, std::vector<std::string>& buffer)
{
	for (size_t i = 0; i < line.length(); i++)
	{
		if ((line[i] < 32 && line[i] != 9 && line[i] != 10) || line[i] == 127)
			throw std::invalid_argument("Parsing error, invalid no printable character\n");
		if (line[i] == '#')
		{
			line = line.substr(0, i);
			break;
		}
	}
	checkParenthesis(line);
	checkLocation(line);
	checkSemicolon(line);
	for (size_t i = 0; i < line.length(); i++)
	{
		if (line[i] != 9 && line[i] != 32)
		{
			size_t j = i;
			while (i < line.length() && line[i] != 9 && line[i] != 32)
				i++;
			buffer.push_back (line.substr(j, i - j));
		}
	}
	for (size_t i = 0; i + 1 < buffer.size(); i++)
	{
		if (buffer[i] == "{" && !buffer[i + 1].empty() && buffer[i + 1] == "}")
			throw std::invalid_argument("Parsing error, server & locations's"
				" blocks cannot be empty\n");
	}
}

void	ParsingConf::checkStructure(std::vector<std::string>& buffer)
{
	std::vector<std::string> stack;

	if (buffer.empty() || buffer[0] != "server")
		throw std::invalid_argument(" Parsing error, configuration files need"
			" to begin by a server\n");
	for (size_t i = 0; i < buffer.size(); i++)
	{
		if (buffer[i] == "server")
		{
			if (i + 1 >= buffer.size() || buffer[i + 1] != "{")
				throw std::invalid_argument(" Parsing error, miss '{'"
					" to opening server's block\n");
			stack.push_back(buffer[i]);
		}
		else if (buffer[i] == "location")
		{
			if (stack.empty() || stack.back() != "server")
				throw std::invalid_argument(" Parsing error, 'location' blocks "
					"allowed only in a server block\n");
			if (i + 2 >= buffer.size() || buffer[i + 2] != "{")
				throw std::invalid_argument(" Parsing error, miss '{' to"
					" opening location's block\n");
			stack.push_back(buffer[i]);
		}
		else if (buffer[i] == "{")
		{
			if ((i > 0 && buffer[i - 1] != "server")
				&& (i > 1 && buffer[i - 2] != "location"))
				throw std::invalid_argument(" Parsing error, wrong block's"
					" name before parenthesis\n");
		}
		else if (buffer[i] == "}")
		{
			if (stack.empty())
			{
				throw std::invalid_argument(" Parsing error, trying to close"
					" a block not open\n");
			}
			stack.pop_back();
		}
	}
	if (!stack.empty())
		throw std::invalid_argument(" Parsing error, a block is not closed\n");
	return;
}

size_t	ParsingConf::fillServers(std::vector<std::string>& buffer, size_t& i,
	std::vector<ABlockBase*>& blocks, std::vector<ServerData>::iterator& itServer)
{
	int	num = i;

	servers.push_back(ServerData());
	itServer = servers.end() - 1;
	blocks.push_back(&(*itServer));
	i = i + 2;
	while (i < buffer.size())
	{
		if (buffer[i] == "location")
		{
			if (!blocks.back()->checkFlag("location"))
				num = i;
			//checker si i + 1 == "{" ??
			for(; i < buffer.size(); i++)
			{
				if(buffer[i] == "}")
				{
					i++;
					break;
				}
			}
		}
		else if (buffer[i] == "listen")
			i = itServer->fillListens(buffer, i + 1);
		else if (buffer[i] == "server_name")
			i = itServer->fillServerName(buffer, i + 1);
		else if (buffer[i] == "root")
			i = blocks.back()->fillRoot(buffer, i + 1);
		else if (buffer[i] == "index")
			i = blocks.back()->fillIndex(buffer, i + 1);
		else if (buffer[i] == "client_max_body_size")
			i = blocks.back()->fillBodySize(buffer, i + 1);
		else if (buffer[i] == "return")
				i = blocks.back()->fillReturnDirectives(buffer, i + 1);
		else if (buffer[i] == "error_page")
			i = blocks.back()->fillErrorPage(buffer, i + 1);
		else if (buffer[i] == "allow_methods")
			i = blocks.back()->fillAllowedMethods(buffer, i + 1);
		else if (buffer[i] == "cgi_pass")
			i = blocks.back()->fillCgiPass(buffer, i + 1);
		else if (buffer[i] == "}")
			return num;
		else
			throw std::invalid_argument(" Parsing error, invalid directives: " + buffer[i]);
	}
	return num;
}

size_t	ParsingConf::fillLocations(std::vector<std::string>& buffer, size_t& i,
	std::vector<ABlockBase*>& blocks, std::vector<ServerData>::iterator& itServer,
	std::map<std::string, LocationConf>::iterator& itLocation)
{
	if (itServer->getLocations().find(buffer[i + 1]) != itServer->getLocations().end())
		throw std::invalid_argument(" Parsing error, this 'location'"
			" directive already exist\n");
	itServer->getLocations()[buffer[i + 1]] = LocationConf(*itServer);
	itLocation = itServer->getItLocations(buffer[i + 1]);
	itLocation->second.setKey(buffer[i + 1]);
	blocks.push_back(&itLocation->second);
	i = i + 3;
	while (i < buffer.size())
	{
		if (buffer[i] == "listen" || buffer[i] == "server_name")
			throw std::invalid_argument("Parsing error, '" + buffer[i] +
			"' directive allowed only in server block\n");
		else if (buffer[i] == "autoindex")
			i = itLocation->second.fillAutoIndex(buffer, i + 1);
		else if (buffer[i] == "root")
			i = blocks.back()->fillRoot(buffer, i + 1);
		else if (buffer[i] == "index")
			i = blocks.back()->fillIndex(buffer, i + 1);
		else if (buffer[i] == "client_max_body_size")
			i = blocks.back()->fillBodySize(buffer, i + 1);
		else if (buffer[i] == "return")
			i = blocks.back()->fillReturnDirectives(buffer, i + 1);
		else if (buffer[i] == "error_page")
			i = blocks.back()->fillErrorPage(buffer, i + 1);
		else if (buffer[i] == "allow_methods")
			i = blocks.back()->fillAllowedMethods(buffer, i + 1);
		else if (buffer[i] == "cgi_pass")
			i = blocks.back()->fillCgiPass(buffer, i + 1);
		else if (buffer[i] == "}")
			return i;
		else
		 	throw std::invalid_argument(" Parsing error, invalid directives: " + buffer[i]);
		// std::cout << BLUE << itLocation->second.getAutoindex() << RESET << std::endl;

	}
	return i;
}

void	ParsingConf::fillStructs(std::vector<std::string>& buffer)
{
	size_t											i = 0;
	std::vector<ABlockBase*>						blocks;
	std::vector<ServerData>::iterator 				itServer;
	std::map<std::string, LocationConf>::iterator	itLocation;
	
	checkStructure(buffer);
	while(i < buffer.size())
	{
		if (buffer[i] == "server")
			i = fillServers(buffer, i, blocks, itServer);
		else if (buffer[i] == "location")
			i = fillLocations(buffer, i, blocks, itServer, itLocation);
		else if (blocks.empty())
			throw std::invalid_argument(" Parsing error, directives allowed only in a block\n");	
		else if (buffer[i] == "}")
		{
			blocks.pop_back();
			i++;
		}
		else
			i++;
	}
	return;
}

// void	ParsingConf::print_structure()
// {
// 	for (size_t i  = 0; i < servers.size(); i++)
// 	{
// 		std::cout << GREEN << "\n[SERVER #" << i << "]" << std::endl;
// 		std::cout << PINK << "Listen:\n";
// 		for (size_t j = 0; j < servers[i].getListens().size(); j++)
// 			std::cout << "port: " << servers[i].getListens()[j].first
// 					  << "ip: " << servers[i].getListens()[j].second << "\n";
		 
// 		std::cout << PURPLE << "ServerName:\n";
// 		for (size_t j = 0; j < servers[i].getServerName().size(); j++)
// 			std::cout << servers[i].getServerName()[j] << "\n";
			 
// 		std::cout << PURPLE << "Root: " << servers[i].getRoot() << "\n"
		
// 		<< "Bodysize: " << servers[i].getBodySize() << "\n"
		
// 		<< "Index: ";
// 		for (size_t j = 0; j < servers[i].getIndex().size(); j++)
// 			std::cout << servers[i].getIndex()[j] << "\n";
		
// 		std::cout << "AllowedMethods:\n";
// 		for (size_t j = 0; j < servers[i].getAllowedMethods().size(); j++)
// 			std::cout << servers[i].getAllowedMethods()[j] << "\n";
		
// 		std::cout << "ReturnDirective:\n";
// 		for (size_t j = 0; j < servers[i].getReturnDirective().size(); j++)
// 			std::cout << servers[i].getReturnDirective()[j] << "\n";
		
// 		std::cout << "ErrorPage:\n";
// 		for (std::map<int, std::string>::const_iterator it = servers[i].getErrorPage().begin(); it != servers[i].getErrorPage().end(); it++)
// 			std::cout << it->first << " " << it->second << "\n";

// 		std::cout << "CgiPass:\n";
// 		for (std::map<std::string, std::string>::const_iterator it = servers[i].getCgiPass().begin(); it != servers[i].getCgiPass().end(); it++)
// 			std::cout << it->first << " " << it->second << "\n";

// 		std::cout << PINK "Locations: \n";
// 		std::map<std::string, LocationConf> loc = servers[i].getLocations();
// 		for (std::map<std::string, LocationConf>::iterator it = loc.begin(); it != loc.end(); it++)
// 		{
//     		std::cout << PINK << it->first << "\n" << PURPLE
// 			<< "Autoindex: " << it->second.getAutoindex() << "\n"
// 			<< "Root: " << it->second.getRoot() << "\n"
// 			<< "Bodysize: " << it->second.getBodySize() << "\n"
// 			<< "Index: ";
// 			for (size_t j = 0; j < it->second.getIndex().size(); j++)
// 				std::cout << it->second.getIndex()[j] << "\n";
// 			std::cout << "AllowedMethods:\n";
// 			for (size_t j = 0; j < it->second.getAllowedMethods().size(); j++)
// 				std::cout << it->second.getAllowedMethods()[j] << "\n";
// 			std::cout << "ReturnDirective:\n";
// 			for (size_t j = 0; j <it->second.getReturnDirective().size(); j++)
// 				std::cout <<it->second.getReturnDirective()[j] << "\n";
// 			std::cout << "ErrorPage:\n";
// 			for (std::map<int, std::string>::const_iterator it2 = it->second.getErrorPage().begin(); it2 != it->second.getErrorPage().end(); it2++)
// 				std::cout << it2->first << " " << it2->second << "\n";
// 			std::cout << "CgiPass:\n";
// 			for (std::map<std::string, std::string>::const_iterator it3 = it->second.getCgiPass().begin(); it3 != it->second.getCgiPass().end(); it3++)
// 				std::cout << it3->first << " " << it3->second << "\n";
// 		}
// 	}
// 	std::cout << RESET;
// }
