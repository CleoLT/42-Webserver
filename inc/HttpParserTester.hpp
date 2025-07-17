/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParserTester.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:22:22 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/17 14:31:13 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTTPARSERTESTER_HPP
#define	HTTPARSERTESTER_HPP

#define RED		"\033[31m"
#define GRE		"\033[32m"
#define RESET	"\033[0m"

class	HttpParserTester	{
private:
	HttpParserTester();
	HttpParserTester( HttpParserTester const & src );
	~HttpParserTester();

	HttpParserTester &	operator=( HttpParserTester const & rhs );

	/*---------------Http Message-----------------*/
	static void	onlyASCII();
	static void	crWithoutLf();
	static void	emptyLinesBeforeReqLine();
	static void	isspaceBeforeHeader();
	static void sfWithoutCrlf();
	static void shouldHaveOneHost();
	



	static void validHostSyntaxis();
	static void trimSpacesAndTab();

	

	/*--------------Request Line------------------*/
	static void	shouldHaveTwoSpaces();
	static void shouldHaveThreeTokens();
	static void	uriTooLong();
	static void httpVersion();
	static void	implementedMethod();


	/*-------------------Uri----------------------*/
	static void	invalidCharUri();
	static void	invalidForm();
	static void	validPercentEncoded();
	static void	uriReconstruction();

	
public:
	static void	parseHttpMessageTest();
	static void	parseRequestLineTest();
	static void parseUriTest();
	static void	parseHostTest();
};

#endif
