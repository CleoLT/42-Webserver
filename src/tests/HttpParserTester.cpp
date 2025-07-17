/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParserTester.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:22:15 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/17 15:42:46 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParserTester.hpp"
#include "HttpParser.hpp"
#include "RequestLine.hpp"
#include "Uri.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>

/*--------------------------Request Message------------------------------------*/

void	HttpParserTester::onlyASCII() {
	std::string mess( "GET /こんにちは HTTP/1.1" );
	std::string host;

	try {
		std::string	http_mess( mess + "\r\nHost: www.example.com \r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << RED << "valid message:	  " << mess << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << mess << " host don't accepted/ Test OK" << std::endl;
	}

	host = "españa";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << RED << "valid message:	  " << host << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << std::endl;
	}

}

void	HttpParserTester::crWithoutLf() {
	std::string host;
	
	try {
		std::string	http_mess( "POST /for\rm HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << RED << "Valid message:    CR without LF accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  CR without LF in body don't accepted/ Test OK" << std::endl;
	}

	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apel\rido=perez" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << GRE << "Valid message:    CR without LF in body accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  CR without LF in body don't accepted / Test FAIL" << std::endl;
	}
}

void	HttpParserTester::emptyLinesBeforeReqLine() {
	std::string host;

	try {
		std::string	http_mess( "\r\n\r\n\r\nPOST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << GRE << "Valid message:    three empty lines (CRLF) before Request-Line accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  three CRLF before req-line not accepted / Test FAIL" << std::endl;
	}

	try {
		std::string	http_mess( "\r\nPOST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << GRE << "Valid message:    one empty line (CRLF) before Request-Line accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  one CRLF before req-line not accepted / Test FAIL" << std::endl;
	}
}

void	HttpParserTester::isspaceBeforeHeader() {
	std::string host;

	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\n\tContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << RED << "Valid message:    \\tab before header accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  \\tab (isspace) before header don't accepted / Test OK" << std::endl;
	}
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\n   Host: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << RED << "Valid message:    spaces before header accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  spaces (isspace) before header don't accepted / Test OK" << std::endl;
	}
}

void	HttpParserTester::sfWithoutCrlf() {
	std::string host;

	try {
		std::string	http_mess( "POST /form HTTP/1.1" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << RED << "Valid message:    string without CRLF / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  string without CRLF / Test OK" << std::endl;
	}
}

void	HttpParserTester::shouldHaveOneHost() {

	std::string host;
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nHost: 27\r\n\r\nnombre=juan&apellido=perez" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << RED  <<"Valid message:    two hosts accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  two hosts don't accepted/ Test OK" << std::endl;
	}

	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apel\rido=perez" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << RED <<"Valid message:     message without host accepted / Test FAIL" << RESET << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  message without host don't accepted / Test OK" << RESET << std::endl;
	}
}

void	HttpParserTester::parseHttpMessageTest() {
	
	std::cout << "_____________Http message tests_____________" << std::endl;

	onlyASCII();
	crWithoutLf();
	emptyLinesBeforeReqLine();
	isspaceBeforeHeader();
	sfWithoutCrlf();
	shouldHaveOneHost();

	std::cout << std::endl;

}



/*-----------------------------------Host Header-----------------------------------------*/


void	HttpParserTester::validHostSyntaxis() {
	
	std::string host( "www.example.com" );
	
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHoSt: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		std::cout << GRE << "valid host:	  " << host << " accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  " << host << " host don't accepted/ Test FAIL" << std::endl;
	}
	
	host = "example.com";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << GRE << "valid host:	  " << host << " accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  " << host << " host don't accepted/ Test FAIL" << std::endl;
	}

	host = "http://example.com";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << RED << "valid host:	  " << host << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << std::endl;
	}

	host = "example..com";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << RED << "valid host:	  " << host << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << std::endl;
	}

	host = "exam ple.com";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << RED << "valid host:	  " << host << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << std::endl;
	}

	host = "127.0.0.1";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << GRE << "valid host:	  " << host << " accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  " << host << " host don't accepted/ Test FAIL" << std::endl;
	}

	host = "127.0.0.256";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << RED << "valid host:	  " << host << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << std::endl;
	}

	host = "127.0.0";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << RED << "valid host:	  " << host << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << std::endl;
	}

	host = "domain.com:443";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << GRE << "valid host:	  " << host << " accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  " << host << " host don't accepted/ Test FAIL" << std::endl;
	}

	host = "domain.com:65536";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << RED << "valid host:	  " << host << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << std::endl;
	}

	host = "domain.com:";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << RED << "valid host:	  " << host << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << std::endl;
	}

	host = "domain.com:08";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << RED << "valid host:	  " << host << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << std::endl;
	}

	host = "domain.com:80.0";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << RED << "valid host:	  " << host << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << std::endl;
	}
	
	host = "domain.com:abc";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << RED << "valid host:	  " << host << " accepted / Test FAIL" << RESET << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << host << " host don't accepted/ Test OK" << RESET << std::endl;
	}

}

void	HttpParserTester::trimSpacesAndTab() {

	std::string host( "   www.example.com   " );
	
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHoSt: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << GRE << "valid host:	  \"" << host << "\" spaces accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  \"" << host << "\" host don't accepted/ Test FAIL" << std::endl;
	}

		host = "   \twww.example.com   \t";
	try {
		std::string	http_mess( "POST /form HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/urlencoded\r\n\r\n" );
		HttpParser::parseHttpMessage( http_mess, host );
		HttpParser::parseHost( host );
		std::cout << GRE << "valid host:	  \"" << host << "\" spaces and tabs accepted / Test OK" << RESET << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  \"" << host << "\" host don't accepted, tabs ans spaces/ Test FAIL";
		std::cout << RESET << std::endl;
	}

}


void	HttpParserTester::parseHostTest() {

	std::cout << "_______________Host syntaxis tester_____________" << std::endl;
	validHostSyntaxis();
	trimSpacesAndTab();

	std::cout << std::endl;
}


/*-----------------------------------Request Line----------------------------------------*/

void	HttpParserTester::shouldHaveTwoSpaces() {
	try {
		std::string	req_line( "POST /form HTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << GRE << "Valid reqline:    two spaces accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  two spaces don't accepted / Test FAIL" << std::endl;
	}
	try {
		std::string	req_line( "POST /form  HTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << RED << "Valid reqline:    three spaces accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  three spaces don't accepted / Test OK" << std::endl;
	}
	try {
		std::string	req_line( "POST /formHTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << RED << "Valid reqline:    one space accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  one space don't accepted / Test OK" << std::endl;
	}
}

void	HttpParserTester::shouldHaveThreeTokens() {
	try {
		std::string	req_line( "POST /form HTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << GRE << "Valid req-line:   three tokens accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  three tokens don't accepted / Test FAIL" << std::endl;
	}
	try {
		std::string	req_line( "POST  /formHTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << RED <<  "Valid req-line:   two tokens accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  two tokens don't accepted / Test OK" << std::endl;
	}
}

void	HttpParserTester::uriTooLong() {
	try {
		std::string	uri( 8001, '/' );
		std::string req_line( "GET " + uri + " HTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << RED << "Valid req-line:   8001 octects accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ": 8001 octets don't accepted / Test OK" << std::endl;
	}

	try {
		std::string	uri( 8000, '/' );
		HttpParser::parseRequestLine( "GET " + uri + " HTTP/1.1" );
		std::cout << GRE << "Valid req-line:   8000 octects accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ": 8000 octets don't accepted / Test FAIL" << std::endl;
	}
}

void	HttpParserTester::httpVersion() {
	try {
		std::string	req_line( "POST /form HTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << GRE << "Valid http-vers:  HTTP/1.1 version accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  HTTP/1.1 version don't accepted / Test FAIL" << std::endl;
	}
	try {
		std::string	req_line( "POST  /form HTTP//1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << RED <<  "Valid http-vers:  HTTP//1.1 accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  HTTP//1.1 don't accepted / Test OK" << std::endl;
	}
	try {
		std::string	req_line( "POST  /form HTTP/2.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << RED <<  "Valid http-vers:   HTTP/2.1 accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  HTTP/2.1 don't accepted / Test OK" << std::endl;
	}
}

void	HttpParserTester::implementedMethod() {
	try {
		std::string	req_line( "GET /form HTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << GRE << "Implemented method:  GET method accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  GET method don't accepted / Test FAIL" << std::endl;
	}
	try {
		std::string	req_line( "FOOMETHOD /form HTTP/1.1" );
		HttpParser::parseRequestLine( req_line );
		std::cout << RED << "Implemented method:  FOOMETHOD accepted / Test FAIL" << RESET << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  FOOMETHOD don't accepted / Test OK" << RESET << std::endl;
	}
}

void	HttpParserTester::parseRequestLineTest() {

	std::cout << "_____________Request-line tests_____________" << std::endl;

	shouldHaveTwoSpaces();
	shouldHaveThreeTokens();
	uriTooLong();
	httpVersion();
	implementedMethod();
 
	std::cout << RESET << std::endl;
}


void	HttpParserTester::invalidCharUri() {

	std::string uri( "/holi(hola)" );
	
	try {
		std::string	http_mess( "GET " + uri + " HTTP/1.1");
		HttpParser::parseRequestLine( http_mess );
		std::cout << GRE << "valid uri:    	  " << uri << " accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  " << uri << " '(' and ')' char don't accepted/ Test FAIL" << std::endl;
	}
	
	uri = "/holi{hola}";
	try {
		std::string	http_mess( "POST " + uri + " HTTP/1.1");
		HttpParser::parseRequestLine( http_mess );
		std::cout << RED << "valid uri:	          " << uri << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << uri << " '{' and '}' char don't accepted/ Test OK" << std::endl;
	}
} 

void	HttpParserTester::invalidForm() {

	std::string uri( "http://example.com/index.html" );
		
	try {
		std::string	http_mess( "POST " + uri + " HTTP/1.1");
		HttpParser::parseRequestLine( http_mess );
		std::cout << RED << "valid uri:	          " << uri << " absolut-form accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << uri << " absolut-form don't accepted/ Test OK" << std::endl;
	}

	uri = "example.com:80";
	try {
		std::string	http_mess( "POST " + uri + " HTTP/1.1");
		HttpParser::parseRequestLine( http_mess );
		std::cout << RED << "valid uri:	          " << uri << " authority-form accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << uri << " authority-form don't accepted/ Test OK" << std::endl;
	}

	uri = "*";
	try {
		std::string	http_mess( "POST " + uri + " HTTP/1.1");
		HttpParser::parseRequestLine( http_mess );
		std::cout << RED << "valid uri:	          " << uri << " asterisk-form accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << uri << " asterisk-form don't accepted/ Test OK" << std::endl;
	}

} 


void	HttpParserTester::validPercentEncoded() {

	std::string uri( "/post%20kost" );
	
	try {
		std::string	http_mess( "GET " + uri + " HTTP/1.1");
		HttpParser::parseRequestLine( http_mess );
		std::cout << GRE << "valid percent:	  " << uri << " accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  " << uri << " percent encoded don't accepted/ Test FAIL" << std::endl;
	}

	uri = "/post%A";
	try {
		std::string	http_mess( "POST " + uri + " HTTP/1.1");
		HttpParser::parseRequestLine( http_mess );
		std::cout << RED << "valid percent:	  " << uri << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << uri << " percent encoded don't accepted/ Test OK" << std::endl;
	}
	
	uri = "/post%RTlala";
	try {
		std::string	http_mess( "POST " + uri + " HTTP/1.1");
		HttpParser::parseRequestLine( http_mess );
		std::cout << RED << "valid percent:	  " << uri << " accepted / Test FAIL" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << GRE << e.what() << ":  " << uri << " percent encoded don't accepted/ Test OK" << std::endl;
	}

	uri = "/post%4E%33%54lala%F5";
	try {
		std::string	http_mess( "POST " + uri + " HTTP/1.1");
		HttpParser::parseRequestLine( http_mess );
		std::cout << GRE << "valid percent:	  " << uri << " accepted / Test OK" << std::endl;
	} catch( std::invalid_argument e ) {
		std::cout << RED << e.what() << ":  " << uri << " percent encoded don't accepted/ Test FAIL" << std::endl;
	}
}

void	HttpParserTester::uriReconstruction() {
	std::string	uri( "/post%20kost?query#holi" ), host( "example.com" );
	Uri			test = Uri( uri, host );
		
	std::cout << std::endl;
	std::cout << RESET << "Uri reconstruction: " << test.getUri() << std::endl;
	std::cout << "path: " << test.getPath() << ", query: " << test.getQuery();
	std::cout << ", fragment: " << test.getFragment() << std::endl;

	std::string	uri2( "/post%20kost#frrragment" ), host2( "example.com" );
	Uri			test2 = Uri( uri2, host2 );
		
	std::cout << std::endl;
	std::cout << RESET << "Uri reconstruction: " << test2.getUri() << std::endl;
	std::cout << "path: " << test2.getPath() << ", query: " << test2.getQuery();
	std::cout << ", fragment: " << test2.getFragment() << std::endl;

}



void	HttpParserTester::parseUriTest() {

	std::cout << "_________________Uri tests__________________" << std::endl;

//	uriTooLong();
	invalidCharUri();
	invalidForm();
	validPercentEncoded();
	uriReconstruction(); 

 
	std::cout << RESET << std::endl << "_____________End tests_____________" << std::endl << std::endl;

}



