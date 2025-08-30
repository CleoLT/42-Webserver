/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:56:03 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/30 16:28:09 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "ParsingConf.hpp"
#include "HttpRequest.hpp"
#include "HttpParserTester.hpp"
#include "ServerManager.hpp"

int main(int ac, char** av)
{
	if (ac > 2)
		return (std::cerr << ERROR << PINK << "Too many arguments" << std::endl, 1);
		
	std::ifstream file;
	if (ac == 1)
		file.open(DEFAULTCONF); // FABIO it can also run without a specified cfg file
	else
		file.open(av[1]);
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
	// P.print_structure();
	file.close();

	setupSignal(); // TODO this needs to be completed

	ServerManager	testserv(P);

	HttpRequest		req(testserv); //run tests

	printServerManager(testserv);
	testserv.servSetup();
	testserv.servRun();
	//throw exception for this part too because he will return 0 for error without

	return 0;
}
