/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:51:24 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/28 19:45:45 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "HttpRequest.hpp"
#include "Utils.hpp"

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

Response::Response(HttpRequest const &request) : _request(&request)
{

	_clientFd	   	= -1;
	_response	   	= "";
	_location	   	= "";
	_method			= "";
	_contentType	= "";
	_contentLength	= "";
	_autoindex		= _request->getAutoindex();
}

Response::~Response() {}

Response::Response(Response const &copy)
	: _clientFd(copy._clientFd), _response(copy._response), _location(copy._location),
	  _method(copy._method), _contentLength(copy._contentLength)
{
	_output << copy._output.str();
}

void Response::setResponse(std::string response)
{
	_response = response;
}

void Response::setContent(std::pair<std::string, std::string> fullPath, std::string method)
{
	std::cout << PINK << "FullPath.first: " << fullPath.first << "\n" << "FullPath.second: " << fullPath.second << RESET << std::endl; // TO BORROW
	if (fullPath.second == "/redirect" || fullPath.second == "/redirect/") //TODO check with another code (303) and send good errorpages
		_location = fullPath.first + "/redirect/index.html";
	else if (fullPath.second == "/" || fullPath.second.empty())
		_location = fullPath.first + "/index.html";
	else
		_location = fullPath.first + fullPath.second;
	std::cout << PINK << "_LOcation: " << _location << RESET << std::endl; // TO BORROW
	_method = method;
}

void Response::setClientFd(int clientFd)
{
	_clientFd = clientFd;
}

std::string Response::prepFile()
{
	// Check if it's a binary file (image)
	//check if _location is empty first? TODO
	if (isBinary(_location))
	{
		std::ifstream file(_location.c_str(), std::ios::binary);
		if (!file.is_open())
			return ""; // TODO ERROR MESSAGE?
		std::ostringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
    else if (isFolder(_location))
    {
		// if (not exist) //find location bloc, done by cleo in http request
		// 	//error
		DIR *dir = opendir(_location.c_str());
		if (!dir)
			return "";//return error to open directory
        return doAutoindex(_location, dir);
	}
	else
	{
		std::ifstream page(_location.c_str());
		if (!page.is_open())
			return ""; // TODO ERROR MESSAGE?
		std::ostringstream pageContent;
		pageContent << page.rdbuf();
		page.close();
		return pageContent.str();
	}
}

std::string Response::doAutoindex(std::string location, DIR *dir)
{
    std::ostringstream html;
    struct dirent *entry;

    std::string uri = location.substr(5);
	doHtmlAutoindex(uri, html);
	
    while ((entry = readdir(dir)) != NULL)
    {
		if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..") // Ignore "." & ".."
			continue;
        html << "      <li><a href=\"" << uri;
        if (uri[uri.size() - 1] != '/')
            html << "/";
        html << entry->d_name << "\">" << entry->d_name << "</a></li>\n";
		std::cout << BLUE << "URI: " << uri << "D_name: " << entry->d_name << RESET << std::endl; // TO BORROW
    }
    html << "    </ul>\n</div>\n</body>\n</html>\n";
    closedir(dir);
    return html.str();
}


void Response::doHtmlAutoindex(std::string &uri, std::ostringstream &html)
{
	html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
    html << "    <meta charset=\"UTF-8\">\n";
    html << "    <title>Index of " << uri << "</title>\n";
    html << "    <style>\n";
    html << "        body {\n";
    html << "            background-color: white;\n";
    html << "            font-family: Arial, sans-serif;\n";
    html << "            display: flex;\n";
    html << "            flex-direction: column;\n";
    html << "            justify-content: center;\n";
    html << "            align-items: center;\n";
    html << "            min-height: 100vh;\n";
    html << "            margin: 0;\n";
    html << "            padding: 20px;\n";
    html << "            box-sizing: border-box;\n";
    html << "        }\n";
    html << "        .purple-text {\n";
    html << "            color: #b388eb;\n";
    html << "            font-size: 48px;\n";
    html << "            text-align: center;\n";
    html << "            margin-bottom: 20px;\n";
    html << "        }\n";
    html << "        .image-container {\n";
    html << "            text-align: center;\n";
    html << "            margin-bottom: 20px;\n";
    html << "        }\n";
    html << "        .image-container img {\n";
    html << "            max-width: 100%;\n";
    html << "            height: auto;\n";
    html << "            max-height: 300px;\n";
    html << "        }\n";
    html << "        .list-container {\n";
    html << "            text-align: center;\n";
    html << "        }\n";
    html << "        ul {\n";
    html << "            list-style: none;\n";
    html << "            padding: 0;\n";
    html << "        }\n";
    html << "        li {\n";
    html << "            margin: 8px 0;\n";
    html << "        }\n";
    html << "        a {\n";
    html << "            text-decoration: none;\n";
    html << "            color: #7b2cbf;\n";
    html << "            font-size: 20px;\n";
    html << "        }\n";
    html << "        a:hover {\n";
    html << "            text-decoration: underline;\n";
    html << "        }\n";
    html << "    </style>\n";
    html << "</head>\n<body>\n";
    html << "    <div class=\"purple-text\">Index of " << uri << "</div>\n";
    html << "    <div class=\"image-container\">\n";
    html << "        <img src=\"/static/cat.png\" alt=\"Autoindex banner\">\n";
    html << "    </div>\n";
    html << "    <div class=\"list-container\">\n";
    html << "    <ul>\n";
}

std::string Response::runScript(std::string const &cgiPath)
{
	size_t		lastDot	   = cgiPath.find_last_of('.');
	std::string scriptType = cgiPath.substr(lastDot);
	std::cout << RED << "script type = " + scriptType << RESET << std::endl; // DBG
	std::string query = _request->getQuery();
	std::cout << RED << std::string(__func__) + " " + query << RESET << std::endl; // DBG

	std::string content;

	int pipeIn[2];
	int pipeOut[2];

	if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1)
	{
		//TODO what error number here?
		printBoxError("Pipe Error");
		return "";
	}

	pid_t child = fork();

	if (child < 0)
	{
		//TODO also here, what error?
		printBoxError("Fork error");
		close(pipeOut[PIPE_WRITE]);
		close(pipeOut[PIPE_READ]);
		close(pipeIn[PIPE_WRITE]);
		close(pipeIn[PIPE_READ]);
		return "";
	}

	/* TODO to implement more types of script,
	create option for selecting the kind of 
	script detected (here for example it's
	python)*/

	else if (child == 0)
	{
		std::string runPath;
		close(pipeIn[PIPE_WRITE]);
		dup2(pipeIn[PIPE_READ], STDIN_FILENO);
		close(pipeIn[PIPE_READ]);

		// Redirect script's stdout to write to pipeOut
		close(pipeOut[PIPE_READ]);
		dup2(pipeOut[PIPE_WRITE], STDOUT_FILENO);
		close(pipeOut[PIPE_WRITE]);

		/* TODO wrap env creation
		into its owh function*/

		std::string varQuery  = "QUERY_STRING=" + query;
		std::string varMethod = "REQUEST_METHOD=" + _request->getHttpMethod();
		char	   *envQuery  = new char[varQuery.length() + 1];
		strcpy(envQuery, varQuery.c_str());

		char *envMethod = new char[varMethod.length() + 1];
		strcpy(envMethod, varMethod.c_str());

		char *envServ[] = {envQuery, envMethod, NULL};

		if (scriptType == ".py")
			runPath = "/usr/bin/python3";
		else if (scriptType == ".php")
			runPath = "/usr/bin/php";
		char *argv[] = {const_cast<char *>(runPath.c_str()), const_cast<char *>(cgiPath.c_str()),
						NULL};
		execve(runPath.c_str(), argv, envServ);

		std::cerr << "Execve failed for " << cgiPath << ": " << strerror(errno) << std::endl; // DBG
		delete[] envQuery;
		exit(1);
	}
	else
	{
		close(pipeIn[PIPE_READ]);
		close(pipeOut[PIPE_WRITE]);

		if (!query.empty())
		{
			if ((write(pipeIn[PIPE_WRITE], query.c_str(), query.length())) == -1)
				printBoxError("Error parent writing");
		}
		close(pipeIn[PIPE_WRITE]);

		// Read the script output from pipeOut
		std::string scriptOutput;
		char		buffer[4096];
		ssize_t		bytesRead;

		while ((bytesRead = read(pipeOut[PIPE_READ], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytesRead] = '\0';
			scriptOutput += buffer;
		}
		close(pipeOut[PIPE_READ]);

		int status;
		waitpid(child, &status, 0);

		// Check if child process exited with error
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			std::cerr << "CGI script " << cgiPath << " exited with code " << WEXITSTATUS(status)
					  << std::endl;
		}
		else if (WIFSIGNALED(status))
		{
			std::cerr << "CGI script " << cgiPath << " killed by signal " << WTERMSIG(status)
					  << std::endl;
		}
		else
		{
			content		 = scriptOutput;
			_contentType = "text/html";
			std::cout << GREEN << "CGI script output captured: " << scriptOutput.length()
					  << " bytes" << RESET << std::endl;
		}
	}
	return content;
}

std::string Response::checkType()
{
	if (isFolder(_location) && _autoindex)
		return "text/html";
		
	std::string extension;
	size_t		dotPos = _location.find_last_of('.');
	if (dotPos != std::string::npos)
		extension = _location.substr(dotPos);

	if (extension == ".html" || extension == ".htm")
		return "text/html";
	else if (extension == ".jpg" || extension == ".jpeg")
		return "image/jpeg";
	else if (extension == ".png")
		return "image/png";
	else if (extension == ".gif")
		return "image/gif";
	else if (extension == ".css")
		return "text/css";
	else if (extension == ".py" || extension == ".php") // TODO check if valid
		return "cgi-script";
	else
		return "application/octet-stream";
}

void Response::prepResponse()
{
	std::string content;
	_contentType = checkType();

	if (_contentType == "cgi-script")
		content = runScript(_location);
	
	else if (_request->getStatusCode() == 200)
		content = prepFile();
	// else
	// 	//errorpages

	std::ostringstream output;
	output << content.length();
	_contentLength = output.str();

	Header header(*this);
	_response = header.getHeader() + content;
}

void Response::printRawResponse()
{
	std::cout << PINK << "[RAW RESPONSE]" << RESET << std::endl;
	for (size_t i = 0; i < _response.size(); i++)
	{
		if (_response[i] == '\r')
			std::cout << "\\r";
		else if (_response[i] == '\n')
			std::cout << "\\n\n";
		else
			std::cout << _response[i];
	}
	std::cout << PINK << "\n[END OF RESPONSE]" << RESET << std::endl;
}

void Response::sendResponse()
{
	prepResponse();

	size_t		totalSent	= 0;
	size_t		totalSize	= _response.size();
	const char *data		= _response.c_str();
	int			attempts	= 0;
	const int	maxAttempts = 100;

	while (totalSent < totalSize && attempts < maxAttempts)
	{

		ssize_t sent = send(_clientFd, data + totalSent, totalSize - totalSent, 0);
		if (sent > 0)
			totalSent += sent;
		else if (sent == 0)
		{
			std::cerr << "Connection closed by client during send" << std::endl;
			break;
		}
		else
		{
			usleep(1000);
			attempts++;
		}
	}

	if (totalSent != totalSize)
		printBoxError("Warning, send incomplete");
}

std::string Response::getType()
{
	return _contentType;
}

std::string Response::getLength()
{
	return _contentLength;
}

std::string Response::getResponse()
{
	return _response;
}

bool	Response::getAutoindex() const
{
	return _autoindex;
}
