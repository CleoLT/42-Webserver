/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABlockBase.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:05 by esellier          #+#    #+#             */
/*   Updated: 2025/08/29 10:53:09 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Utils.hpp"
#include "../../inc/ABlockBase.hpp"

ABlockBase::ABlockBase()
{
	_root = "/www";
	_index.push_back("index.html");
	_bodySize = 1048576;
	_returnDirective.push_back("301");
	_returnDirective.push_back("/redirect/index.html");
	_allowedMethods.push_back("GET");
	_allowedMethods.push_back("POST");
	_cgiPass[".py"] = "/usr/bin/python3";
	_cgiPass[".php"] = "/usr/bin/php-cgi";
}
  
ABlockBase::~ABlockBase() {} 

ABlockBase::ABlockBase(ABlockBase const& other)
{
	*this = other;
}
ABlockBase&	ABlockBase::operator=(ABlockBase const& other)
{
	if (this != &other)
	{
		this->_root = other._root;
		this->_index = other._index;
		this->_bodySize = other._bodySize;
		this->_returnDirective = other._returnDirective;
		this->_errorPage = other._errorPage;
		this->_allowedMethods = other._allowedMethods;
		this->_cgiPass = other._cgiPass;
	}
	return *this;
}

std::vector<std::string> const& ABlockBase::getIndex() const
{
	return _index;
}
		
unsigned int const& ABlockBase::getBodySize() const
{
	return _bodySize;
}
		
std::vector<std::string> const& ABlockBase::getReturnDirective() const
{
	return _returnDirective;
}

std::map<int, std::string> const& ABlockBase::getErrorPage() const
{
	return _errorPage;
}

std::vector<std::string> const&	ABlockBase::getAllowedMethods() const
{
	return _allowedMethods;
}

std::map<std::string, std::string> const&	ABlockBase::getCgiPass() const
{
	return _cgiPass;
}

bool	ABlockBase::checkFlag(std::string const& value)
{
	for(size_t i = 0; i < _flag.size(); i++)
	{
		if (_flag[i] == value)
			return true;
	}
	_flag.push_back(value);
	return false;
}

size_t	ABlockBase::fillRoot(std::vector<std::string>& buffer, size_t i)
{
	if ( i >= buffer.size() || buffer[i].empty())
		throw std::invalid_argument(" Parsing error, miss 'root' argument\n");
	// if (buffer[i][0] != '/') // FABIO paused it to use relative paths for testing
	// 	throw std::invalid_argument(" Parsing error, 'root' expects an absolute path\n");
	if (i + 1 >= buffer.size())
		throw std::invalid_argument(" Parsing error, miss semicolon after"
			" 'root' argument\n");
	if (i + 1 < buffer.size() && buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error, 'root' allows only"
			" one argument\n");
	if (checkFlag("root"))
		throw std::invalid_argument(" Parsing error, only one 'root'"
			" directive allowed by server block\n"); 
	_root = buffer[i];
	return (i + 2);
}

size_t	ABlockBase::fillIndex(std::vector<std::string>& buffer, size_t i)
{
	if (checkFlag("index"))
		throw std::invalid_argument(" Parsing error, only one 'index'"
			" directive allowed by server blocks\n"); 
	if (i >= buffer.size() || buffer[i].empty() || buffer[i] == ";")
		throw std::invalid_argument(" Parsing error, miss 'index' arguments\n");
	if (!_index.empty())
		_index.clear();
	while (i < buffer.size())
	{
		if (buffer[i] == ";")
			break;
		if (buffer[i] == "{" || buffer[i] == "}")
			throw std::invalid_argument(" Parsing error, miss semicolon after"
				" 'index' directive\n");
		for (size_t j = 0; j < _index.size(); j++)
		{
			if (buffer[i] == _index[j])
				throw std::invalid_argument(" Parsing error, 'index' argument duplicated\n");
		}
		_index.push_back(buffer[i]);
		i++;
	}
    return (i + 1);
}

size_t	ABlockBase::fillBodySize(std::vector<std::string>& buffer, size_t i)
{
	if (checkFlag("bodySize"))
		throw std::invalid_argument(" Parsing error, only one 'client_max_body_size'"
			" directive allowed by server blocks\n"); 
	if (i >= buffer.size() || buffer[i].empty() || buffer[i] == ";"
		|| buffer[i] == "{" || buffer[i] == "}")
		throw std::invalid_argument(" Parsing error, miss 'client_max_body_size' arguments\n");
	if (i + 1 >= buffer.size() || buffer[i + 1] != ";")
		throw std::invalid_argument(" Parsing error, miss semicolon after"
				" 'client_max_body_size' directive\n");
	_bodySize = strToSize(buffer[i]);
	if (_bodySize > 1048576)
		throw std::invalid_argument(" Parsing error, 'client_max_body_size'"
			" max allowed is 1 048 576 bits\n");
	return i + 2;
}

size_t	ABlockBase::fillAllowedMethods(std::vector<std::string>& buffer, size_t i)
{
	if (checkFlag("allowMethods"))
		throw std::invalid_argument(" Parsing error, only one 'allow_methods'"
			" directive allowed by server blocks\n"); 
	if (i >= buffer.size() || buffer[i].empty() || buffer[i] == ";"
		|| buffer[i] == "{" || buffer[i] == "}")
		throw std::invalid_argument(" Parsing error, miss 'allow_methods' arguments\n");
	_allowedMethods.clear();
	for (; i < buffer.size(); i++)
	{
		if (buffer[i] == "GET" || buffer[i] == "POST" || buffer[i] == "DELETE")
		{
			for(size_t j = 0; j < _allowedMethods.size(); j++)
			{
				if (_allowedMethods[j] == buffer[i])
					throw std::invalid_argument(" Parsing error, 'allow_methods' argument duplicated\n");
			}
			_allowedMethods.push_back(buffer[i]);
		}
		else if (buffer[i] == ";")
			break;
		else
			throw std::invalid_argument(" Parsing error 501, not implemented\n"
				"'allow_methods' argument is not correct\n");
	}
	return i + 1;
}

size_t	ABlockBase::fillReturnDirectives(std::vector<std::string>& buffer, size_t i)
{
	int	num;

	if (checkFlag("return"))
		throw std::invalid_argument(" Parsing error, only one 'return'"
			" directive allowed by server blocks\n"); 
	if (i + 1 >= buffer.size() || buffer[i].empty() || buffer[i] == ";"
		|| buffer[i] == "{" || buffer[i] == "}" || buffer[i + 1].empty()
		|| buffer[i + 1] == "{" || buffer[i + 1] == "}")
		throw std::invalid_argument(" Parsing error, miss 'return' directive arguments\n");
	_returnDirective.clear();
	if (isInt(buffer[i]))
		num = strToInt(buffer[i]);
	if (!isInt(buffer[i]) || num < 100 || num > 599)
			throw std::invalid_argument(" Parsing error, 'return' directive"
				" argument is not correct\n");
	_returnDirective.push_back(buffer[i]);
	if (buffer[i + 1] != ";")
	{
		_returnDirective.push_back(buffer[i + 1]);
		if (i + 2 >= buffer.size() || buffer[i + 2].empty() || buffer[i + 2] != ";")
			throw std::invalid_argument(" Parsing error, miss semicolon after"
				" 'return' directive arguments\n");
		return i + 3;
	}
	return i + 2;
}

size_t	ABlockBase::fillErrorPage(std::vector<std::string>& buffer, size_t i)
{
	std::vector<int>	num;
	std::string			path;
	
	if (!checkFlag("errorPage"))
		_errorPage.clear();
	if (i + 1 >= buffer.size() || buffer[i].empty() || buffer[i] == ";"
		|| buffer[i] == "{" || buffer[i] == "}" || buffer[i + 1].empty()
		|| buffer[i + 1] == "{" || buffer[i + 1] == "}" || buffer[i] == ";")
		throw std::invalid_argument(" Parsing error, miss 'error_page' arguments\n");
	for(; i < buffer.size(); i++)
	{
		if (isErrorPage(buffer[i]))
		{
			for(size_t j = 0; j < num.size(); j++)
			{
				if (num[j] == strToInt(buffer[i]))
					throw std::invalid_argument(" Parsing error, 'error_page' number duplicated\n");
			}
			num.push_back(strToInt(buffer[i]));
		}
		else if (buffer[i] == ";")
			break;
		else if (isHtmlAddress(buffer[i]))
		{
			path = buffer[i];
			if (i + 1 >= buffer.size() || buffer[i + 1] != ";")
				throw std::invalid_argument(" Parsing error, 'error_page' html adress"
					" must be the last argument\n");
		}	
		else
			throw std::invalid_argument(" Parsing error, 'error_page' argument invalid\n");
	}
	if (num.empty())	
		throw std::invalid_argument(" Parsing error, 'error_page' number is missed\n");
	if (path.empty())	
		throw std::invalid_argument(" Parsing error, 'error_page' html adress is missed\n");
	for (size_t j = 0; j < num.size(); j++)
		_errorPage[num[j]] = path;
	checkErrorPage(_errorPage);
	return i + 1;
}

size_t	ABlockBase::fillCgiPass(std::vector<std::string>& buffer, size_t i)
{
	if (!checkFlag("cgiPass"))
		_cgiPass.clear();
	if (i + 1 >= buffer.size() || buffer[i].empty() || buffer[i] == ";"
		|| buffer[i] == "{" || buffer[i] == "}" || buffer[i + 1].empty()
		|| buffer[i + 1] == "{" || buffer[i + 1] == "}" || buffer[i] == ";")
		throw std::invalid_argument(" Parsing error, miss 'cgi_pass' arguments\n");
	if ((buffer[i] != ".py" && buffer[i] != ".php") || (buffer[i + 1] != "/usr/bin/python3"
		&& buffer[i + 1] != "/usr/bin/php-cgi"))
		throw std::invalid_argument(" Parsing error, 'cgi_pass' argument invalid\n");
	if (_cgiPass.find(buffer[i]) != _cgiPass.end())
		throw std::invalid_argument(" Parsing error, 'cgi_pass' argument duplicated\n");
	_cgiPass[buffer[i]] = buffer[i + 1];
	if ((buffer[i] == ".py" && buffer[i + 1] != "/usr/bin/python3")
		|| (buffer[i] == ".php" && buffer[i + 1] != "/usr/bin/php-cgi"))
		throw std::invalid_argument(" Parsing error, 'cgi_pass' arguments don't match\n");
	if (i + 2 >= buffer.size() || buffer[i + 2] != ";")
		throw std::invalid_argument(" Parsing error, miss semicolon after 'cgi_pass' argument\n");
	return i + 3;
}
