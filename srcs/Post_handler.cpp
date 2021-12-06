/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post_handler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:41:22 by gmaris            #+#    #+#             */
/*   Updated: 2021/12/06 19:37:33 by gmaris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Client.hpp"
# include "Server.hpp"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

/*
** Construct response if case of error
*/
void	_construct_error(Response &response, Request &request)
{
	std::ifstream	file;
	std::stringstream	buf;

	response.path = request.errors + "/" + response.status_code.substr(0, response.status_code.find(" "));
	response.path += ".html";
	std::cout << "path response error = " << response.path << std::endl;
	response.content_type = "text/html";
	file.open(response.path.c_str(), std::ifstream::in);
	if (file.fail())
	{
		response.body = response.status_code;
		response.body_len = response.body.size();
	}
	else
	{
		buf << file.rdbuf();
		response.body = buf.str();
		response.body_len = response.body.size();
		file.close();
	}

}

/*
** Tell if file exist
*/
bool	_isExist(string path)
{
	struct stat	sb;

	if (stat(path.c_str(), &sb) == 0) 
		return true;
	else
		return false;
}

/*
** Tell if file is executable
*/
bool	_isExec(string path)
{
	struct stat	sb;

	if (stat(path.c_str(), &sb) == 0 && sb.st_mode & S_IXUSR) 
		return true;
	else
		return false;
}

/*
** Get the output of cmd
*/
std::string		_exec(const char* cmd)
{
	char buffer[128];
	std::string result = "";
	FILE* pipe = popen(cmd, "r");
	if (!pipe)
		throw std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof buffer, pipe) != NULL) {
			result += buffer;
		}
	} catch (...) {
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return result;
}

/*
** Get path of requested uri
*/
std::string get_path(Request &request)
{
	std::string path;

	if (request.uri.find(request.config.location) != std::string::npos && request.config.location != "/")
		path = request.config.root + request.uri.substr(request.uri.find_last_of('/'));
	else
		path = request.config.root + request.uri;
	return (path);
}

/*
** Extract content-type from output
*/

std::string _newOutput(Client &client, std::string output)
{
	int		body_start = output.find("\n\n");
	string	tmp = output.substr(body_start + 2);

	client.response.content_type = output.substr(14, body_start - 14);
	return tmp;
}

/*
** Call the cgi executable if exist
*/
bool	_cgi(Client &client)
{
	string path_exec;
	string output;

	path_exec = get_path(client.request);
	if (_isExist(path_exec) == false)
	{
	std::cout << "path not found : " <<  path_exec << std::endl;
		client.response.status_code = NOTFOUND;
		_construct_error(client.response, client.request);
		return false;
	}
	if (_isExec(path_exec) == false)
	{
		std::cout << "path not exec" << path_exec << std::endl;
		client.response.status_code = UNAUTHORIZED;
		_construct_error(client.response, client.request);
		return false;
	}
	//From here the executable exist and can be use
	try
	{
		std::cout << "get output from " << path_exec << std::endl;
		output = _exec(path_exec.c_str());
		client.response.body = _newOutput(client, output);
		client.response.body_len = client.response.body.length();
	}
	catch (...)
	{
		client.response.status_code = INTERNALERROR;
		_construct_error(client.response, client.request);
		return false;
	}
	return true;
}

void	post_handler(Client &client)
{
	std::cout << std::endl << std::endl;
	std::cout << BLUE << "\t======POST_HANLDER DEBUG START HERE======" << NC << std::endl;
	
	//check if body is too large
	if (client.request.config.max_body >= 0 && std::stoi(client.request.headers["Content-Length"]) > client.request.config.max_body)
	{
		client.response.status_code = REQTOOLARGE;
		_construct_error(client.response, client.request);
		std::cout << "body too large" << std::endl;
		return ;
	}

	//check if cgi
	if (client.request.uri.find(client.request.config.cgi,
			client.request.uri.length() - client.request.config.cgi.length()) != client.request.uri.npos)
	{
		_cgi(client);
	}


	if (client.request.headers["Content-Type"].find("multipart/form-data") !=
		client.request.headers["Content-Type"].npos)
	{
	//	std::cout << "got multiple post to handle" << std::endl << std::endl;
	}
	else
	{
	//	std::cout << RED << "only one part" << std::endl;
	}

	std::cout << BLUE << "\t======POST_HANLDER DEBUG  END  HERE======" << NC << std::endl;
	std::cout << std::endl << std::endl;
}