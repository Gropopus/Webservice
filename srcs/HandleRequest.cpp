/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 16:49:19 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/02 13:46:50 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Web_serv.hpp"
#include "Client.hpp"
#include <dirent.h>

std::string		getLastModified(std::string path)
{
	char		buf[BUFFER_SIZE + 1];
	int			ret;
	struct tm	*tm;
	struct stat	file_info;

	std::memset(buf, 0, BUFFER_SIZE);
	if (lstat(path.c_str(), &file_info) == -1)
		return ("");
	tm = localtime(&file_info.st_mtime);
	ret = strftime(buf, BUFFER_SIZE, "%a, %d %b %Y %T %Z", tm);
	return (buf);
}

void	buildHeader(Response &response)
{
	response.headers = response.version + ' ' +  response.status_code + "\r\n";
	response.headers += "Content-Type: " + response.content_type + "\r\n";
	response.headers += "Content-Length: " + std::to_string(response.body_len) + "\r\n"; 
	response.headers += "Date: " + ft_getDate() + "\r\n";
	std::cout << YELLOW << response.path << NC << std::endl;
	response.headers += "Last-Modified: " + getLastModified(response.path) + "\r\n";;
	response.headers += "Location: " + response.location + "\r\n";
	if (response.name != "")
		response.headers += "Server: " + response.name + "\n\n";
	else
		response.headers += "Server: Webserv\n\n";
}

void	get_basics(Request &request, Response &response)
{
	response.version = request.version;
	response.location = request.config.location;
	response.name = request.server_name;
}

void	getErrors(Response &response, Request &request, std::string error)
{
	std::string	path;
	std::ifstream		file;
	std::stringstream	buffer;

	path = request.errors + error;
	file.open(path.c_str(), std::ifstream::in);
	buffer << file.rdbuf();
	response.body = buffer.str();
	response.body_len = response.body.size();
	file.close();
	response.content_type = "text/html";
}

std::string get_path(Request &request, Response &response)
{
	std::string path;

	while (request.uri.back() == '/')
		request.uri.pop_back();
	std::cout << request.uri;
	if ((request.uri == request.config.location
		&& request.uri == "/") || is_only(request.uri) == true)
	{
		if (request.config.autoIndex == true)
		{
			create_autoIndex(request, response);
			path = "autoIndex";
		}
		else
		{
			request.uri = "/" + request.config.index;
			path = request.config.root + request.uri;
		}
	}
	else if ((request.uri == request.config.location
	|| request.uri == request.config.location + "/") && request.uri != "/")
		path = request.config.root + "/" + request.config.index;
	else
	{
		if (request.uri.find(request.config.location) != std::string::npos && request.config.location != "/")
			path = request.config.root + request.uri.substr(request.uri.find_last_of('/'));
		else
			path = request.config.root + request.uri;
	}
	return (path);
}

std::string		getContent_type(std::string to_find)
{
	int i = 0;
	std::string		types[15];
	std::string		ret[15];
	types[0] = ".txt";
	types[1] = ".bin";
	types[2] = ".jpeg";
	types[3] = ".jpg";
	types[4] = ".html";
	types[5] = ".htm";
	types[6] = ".png";
	types[7] = ".bmp";
	types[8] = ".pdf";
	types[9] = ".tar";
	types[10] = ".json";
	types[11] = ".css";
	types[12] = ".js";
	types[13] = ".mp3";
	types[14] = ".avi";
	ret[0] = "text/plain";
	ret[1] = "application/octet-stream";
	ret[2] = "image/jpeg";
	ret[3] = "image/jpeg";
	ret[4] = "text/html";
	ret[5] = "text/html";
	ret[6] = "image/png";
	ret[7] = "image/bmp";
	ret[8] = "application/pdf";
	ret[9] = "application/x-tar";
	ret[10] = "application/json";
	ret[11] = "text/css";
	ret[12] = "application/javascript";
	ret[13] = "audio/mpeg";
	ret[14] = "video/x-msvideo";
	while (i < 15)
	{
		if (to_find == types[i])
			return (ret[i]);
		i++;
	}
	return ("not found");
}

void	openFile(Response &response, Request &request)
{
	std::ifstream		file;
	std::stringstream	buffer;

	if (response.status_code == NOTALLOWED)
	{
		getErrors(response, request, "/405.html");
		return ;
	}
	response.path = get_path(request, response);
	if (response.path == "autoIndex")
		return ;
	std::cout << RED << response.path << NC << "\n";
	if (isFileDir(response.path))
	{
		file.open(response.path.c_str(), std::ifstream::in);
		if (file.is_open() == false && response.status_code == OK)
		{
			response.status_code = NOTFOUND;
			response.path = request.errors + "/404.html";
			file.close();
			file.open(response.path.c_str(), std::ifstream::in);
			response.content_type = "text/html";
		}
		buffer << file.rdbuf();
		response.body = buffer.str();
		response.body_len = response.body.size();
		file.close();
	}
	else
	{
		getErrors(response, request, "/403.html");
		response.status_code = FORBIDDEN;
	}
	std::cout << response.res;
	if (response.path.find_last_of('.') != response.path.npos)
		response.content_type = getContent_type(response.path.substr(response.path.find_last_of('.')));
	else
		response.content_type = "not found";
}

void	HandleGET(Client &client)
{
	if (client.request.config.methods.find("GET") == std::string::npos)
		client.response.status_code = NOTALLOWED;
	else
		client.response.status_code = OK;
	openFile(client.response, client.request);
	buildHeader(client.response);
	client.response.res = client.response.headers + client.response.body;
}

void	HandlePOST(Client &client)
{
	if (client.request.config.methods.find("POST") == std::string::npos)
		client.response.status_code = NOTALLOWED;
	else
		client.response.status_code = OK;
	post_handler(client); //build post respond
}

void	HandleDELETE(Client &client)
{
	std::ifstream		file;
	std::stringstream	buffer;

	client.response.status_code = OK;
	if (client.request.config.methods.find("DELETE") == std::string::npos)
	{
		client.response.status_code = NOTALLOWED;
		getErrors(client.response, client.request, "/405.html");
		buildHeader(client.response);
		client.response.res = client.response.headers + client.response.body;
		return ;
	}
	client.response.path = get_path(client.request, client.response);
	//std::cout << CYAN << path << NC << std::endl;
	if (isFileDir(client.response.path) > 0)
	{
		if (remove(client.response.path.c_str()) == 0)
		{
			client.response.status_code = NOCONTENT;
			getErrors(client.response, client.request, "/204.html");
		}
		else
		{
			client.response.status_code = FORBIDDEN;
			getErrors(client.response, client.request, "/403.html");
		}
	}
	else
	{
		client.response.status_code = NOTFOUND;	
		getErrors(client.response, client.request, "/404.html");
	}
	buildHeader(client.response);
	client.response.res = client.response.headers + client.response.body;
}

void	HandleBAD(Client &client)
{
	std::ifstream		file;
	std::stringstream	buffer;

	if (client.request.method != "HEAD" && client.request.method != "PUT"
		&& client.request.method != "CONNECT" && client.request.method != "TRACE")
	{
		client.response.status_code = NOTIMPLEMENTED;
		client.response.path = client.request.errors + "/501.html";
		file.open(client.response.path.c_str(), std::ifstream::in);
		buffer << file.rdbuf();
		client.response.body = buffer.str();
		client.response.body_len = client.response.body.size();
		file.close();
	}
	else
	{
		client.response.status_code = BADREQUEST;
		client.response.path = client.request.errors + "/400.html";
		file.open(client.response.path.c_str(), std::ifstream::in);
		client.response.content_type = "text/html";
		buffer << file.rdbuf();
		client.response.body = buffer.str();
		client.response.body_len = client.response.body.size();
		file.close();
	}
	buildHeader(client.response);
	client.response.res = client.response.headers + client.response.body;
}

void	Client::dispatcher(Client &client)
{
	get_basics(client.request, client.response);
	if (client.request.method == "GET")
		HandleGET(client);
	else if (client.request.method == "POST")
		HandlePOST(client);
	else if (client.request.method == "DELETE")
		HandleDELETE(client);
	else
		HandleBAD(client);
}
