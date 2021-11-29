/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 16:49:19 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/29 15:34:11 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Client.hpp"
#include "Web_serv.hpp"

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


int		isFileDir(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0 )
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

void	OpenFile(Response &response, Request &request)
{
	std::string	path;
	std::ifstream		file;
	std::stringstream	buffer;

	response.version = request.version;
	response.location = request.config.location;
	response.name = request.server_name;
	if (request.uri == request.config.location)
		request.uri += request.config.index;
	path = request.config.root + request.uri;
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
		else if (file.is_open() == false && response.status_code == NOTALLOWED)
		{
			response.status_code = NOTALLOWED;
			file.close();
			path = request.errors + "/405.html";
			file.open(path.c_str(), std::ifstream::in);
			response.content_type = "text/html";
		}
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
		response.status_code = FORBIDDEN;
		path = request.errors + "/403.html";
		file.open(path.c_str(), std::ifstream::in);
		buffer << file.rdbuf();
		response.body = buffer.str();
		response.body_len = response.body.size();
		file.close();
		response.content_type = "text/html";
	}
}

void	HandleGET(Client &client)
{
	if (client.request.config.methods.find("GET") == std::string::npos)
		client.response.status_code = NOTALLOWED;
	else
		client.response.status_code = OK;
	OpenFile(client.response, client.request);
	buildHeader(client.response);
	client.response.res = client.response.headers + client.response.body;
	std::cout << "Reponse:\n"<< CYAN << client.response.res << NC << std::endl;
//	write(client.fd, client.response.res.c_str(), client.response.res.size());
}

void	HandlePOST(Client &client)
{
	if (client.request.config.methods.find("POST") == std::string::npos)
		client.response.status_code = NOTALLOWED;
	else
		client.response.status_code = OK;
}

void	HandleDELETE(Client &client)
{
	if (client.request.config.methods.find("DELETE") == std::string::npos)
		client.response.status_code = NOTALLOWED;
	else
		client.response.status_code = OK;
}

void	HandleBAD(Client &client)
{
	if (client.request.method != "HEAD" && client.request.method != "PUT"
		&& client.request.method != "CONNECT" && client.request.method != "TRACE")
		client.response.status_code = NOTIMPLEMENTED;
	else
		client.response.status_code = BADREQUEST;
}

void	Client::dispatcher(Client &client)
{
	if (client.request.method == "GET")
		HandleGET(client);
	else if (client.request.method == "POST")
		HandlePOST(client);
	else if (client.request.method == "DELETE")
		HandleDELETE(client);
	else
		HandleBAD(client);
}
