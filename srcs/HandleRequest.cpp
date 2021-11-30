/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 16:49:19 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/30 22:34:05 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Web_serv.hpp"
#include "Client.hpp"

void	buildHeader(Response &response)
{
	response.headers = response.version + ' ' +  response.status_code + "\r\n";
	response.headers += "Content-Type: " + response.content_type + "\r\n";
	response.headers += "Content-Length: " + std::to_string(response.body_len) + "\r\n"; 
	response.headers += "Date: " + ft_getDate() + "\r\n";
//	response.headers += "Last-Modified: \r\n";
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

int		isFileDir(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFREG)
			return (1);
		if (s.st_mode & S_IFDIR)
			return (2);
		else
			return 0;
	}
	return (-1);
}

bool	is_only(std::string str)
{
	int i = 0;

	while (str[i])
	{
		if (str[i] != '/')
			return (false);
		i++;
	}
	return (true);
}

std::string get_path(Request &request)
{
	std::string path;

	if ((request.uri == request.config.location
		&& request.uri == "/") || is_only(request.uri) == true)
	{
		request.uri = "/" + request.config.index;
		path = request.config.root + request.uri;
	}
	else if (request.uri == request.config.location && request.uri != "/")
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
	ret[4] = "text/html;";
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
	std::string	path;
	std::ifstream		file;
	std::stringstream	buffer;

	if (response.status_code == NOTALLOWED)
	{
		getErrors(response, request, "/405.html");
		return ;
	}
	path = get_path(request);
	std::cout << RED << path << " " << isFileDir(path) << NC << "\n";
	if (isFileDir(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false && response.status_code == OK)
		{
			response.status_code = NOTFOUND;
			path = request.errors + "/404.html";
			file.close();
			file.open(path.c_str(), std::ifstream::in);
			response.content_type = "text/html";
		}
		// gerer la langue ici
		buffer << file.rdbuf();
		response.body = buffer.str();
		response.body_len = response.body.size();
		file.close();
	}
	//else if () auto index a gerer;
	//{
	//	
	//		response.content_type = "text/html";
	//}
	else
	{
		getErrors(response, request, "/403.html");
		response.status_code = FORBIDDEN;
	}
	std::cout << response.res;
	response.content_type = getContent_type(path.substr(path.find_last_of('.')));
}

void	HandleGET(Client &client)
{
	if (client.request.config.methods.find("GET") == std::string::npos)
		client.response.status_code = NOTALLOWED;
	else
		client.response.status_code = OK;
	openFile(client.response, client.request);
	buildHeader(client.response);
	std::cout<< client.response.headers << std::endl;
	client.response.res = client.response.headers + client.response.body;
}

void	HandlePOST(Client &client)
{
	if (client.request.config.methods.find("POST") == std::string::npos)
		client.response.status_code = NOTALLOWED;
	else
		client.response.status_code = OK;
	std::cout << BLUE << "\t====try" << NC << std::endl;
	post_handler(client); //build post respond
}

void	HandleDELETE(Client &client)
{
	std::string			path;
	std::ifstream		file;
	std::stringstream	buffer;

	client.response.status_code = OK;
	std::cout << YELLOW << client.request.config.methods << NC << std::endl;
	if (client.request.config.methods.find("DELETE") == std::string::npos)
	{
		client.response.status_code = NOTALLOWED;
		getErrors(client.response, client.request, "/405.html");
		buildHeader(client.response);
		client.response.res = client.response.headers + client.response.body;
		return ;
	}
	path = get_path(client.request);
	//std::cout << CYAN << path << NC << std::endl;
	if (isFileDir(path) > 0)
	{
		if (remove(path.c_str()) == 0)
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
//	std::cout << client.response.headers << std::endl;
	client.response.res = client.response.headers + client.response.body;
}

void	HandleBAD(Client &client)
{
	std::string			path;
	std::ifstream		file;
	std::stringstream	buffer;

	if (client.request.method != "HEAD" && client.request.method != "PUT"
		&& client.request.method != "CONNECT" && client.request.method != "TRACE")
	{
		client.response.status_code = NOTIMPLEMENTED;
		path = client.request.errors + "/501.html";
		file.open(path.c_str(), std::ifstream::in);
		buffer << file.rdbuf();
		client.response.body = buffer.str();
		client.response.body_len = client.response.body.size();
		file.close();
	}
	else
	{
		client.response.status_code = BADREQUEST;
		path = client.request.errors + "/400.html";
		file.open(path.c_str(), std::ifstream::in);
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
