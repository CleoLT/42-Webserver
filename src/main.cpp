/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:56:03 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/16 11:58:23 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ParsingConf.hpp"
#include "HttpRequest.hpp"
#include "HttpParserTester.hpp"
#include "Server.hpp"

int main(int ac, char** av)
{
	if (ac != 2)
		return (std::cerr << ERROR << PINK << "Error, one configuration file is required\n", 1);
		
	std::ifstream file(av[1]);
	if (!file)
		return (std::cerr << ERROR << PINK << "Error, file not open\n", 1);
	
	ParsingConf P;
	
	try
	{	
		static std::vector<std::string>	buffer;
		std::string line;
		while (std::getline(file, line))
			P.doParsing(line, buffer);
		// P.print_tokens(buffer);
		P.fillStructs(buffer);
	}
	catch(const std::exception& e)
	{
		std::cerr << ERROR << PINK << e.what() << '\n' << RESET;
		file.close();
		return (1);
	}
	file.close();

	Server testserv(P);

	// ----> print defaultErrorPages
	// for (std::map<int, std::pair<std::string, std::string> >::const_iterator it = testserv.getDefaultErrorPages().begin(); it != testserv.getDefaultErrorPages().end(); it++)
	// {
	// 	std::cout << GREEN << it->first << " " << it->second.first << " " << it->second.second << "\n"; 
	// }
	
	HttpRequest test = HttpRequest();
	
	testserv.servSetup();
	testserv.servRun();

	return 0;
}
	//manage ctrl + c
