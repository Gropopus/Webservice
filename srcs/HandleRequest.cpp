/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 16:49:19 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/13 15:50:52 by gmaris           ###   ########.fr       */
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
	int ret;

	if (response.status_code == NOTALLOWED)
	{
		getErrors(response, request, "/405.html");
		return ;
	}
	response.path = get_path(request, response);
	if (response.path == "autoIndex")
		return ;
	std::cout << RED << response.path << NC << "\n";
	if ((ret = isFileDir(response.path)))
	{
		file.open(response.path.c_str(), std::ifstream::in);
		std::cout << "ret = " << ret << std::endl;
		if (ret == 1)
		{//file
			if (file.is_open() == false && response.status_code == OK)
			{
				std::cout << CYAN << "OUI?\n" << request.config.root << request.uri << "->" << request.config.index << "<-" << std::endl;
				if (_isExist(response.path) == false)
				{
					response.status_code = NOTFOUND;
					getErrors(response, request, "/404.html");
					buildHeader(response);
					return ;
				}
				else
				{
					response.status_code = UNAUTHORIZED;
					getErrors(response, request, "/401.html");
					buildHeader(response);
					return ;
				}
			}
		}
		if (ret == 2)// directory
		{
			response.status_code = FORBIDDEN;
			getErrors(response, request, "/403.html");
			buildHeader(response);
			return ;
		}
		buffer << file.rdbuf();
		response.body = buffer.str();
		response.body_len = response.body.size();
		if (response.body_len == 0)
		{
			response.body = getDefaultIndex();
			response.body_len = response.body.size();
			response.content_type = ".html";
		}
		file.close();
	}
	else
	{
		std::cout << "????\n";
		getErrors(response, request, "/403.html");
		response.status_code = FORBIDDEN;
		buildHeader(response);
		return ;
	}
	std::cout << response.res;
	if (response.path.find_last_of('.') != response.path.npos)
		response.content_type = getContent_type(response.path.substr(response.path.find_last_of('.')));
	else
		response.content_type = "not found";
}

std::string	_getQueryString(std::string uri)
{
	size_t		pos_intero;
	std::string	query;

	pos_intero = uri.find_last_of("?");

	if (pos_intero == uri.npos)
		query = "";
	else
		query = uri.substr(pos_intero + 1);
	return query;
}

std::string	_getNewUri(std::string uri)
{
	size_t		pos_intero;
	std::string	query;

	pos_intero = uri.find_last_of("?");

	if (pos_intero == uri.npos)
		query = uri;
	else
		query = uri.substr(0, pos_intero);
	return query;
}

std::string	_getFileExtension(std::string uri)
{
	size_t		pos_intero;
	size_t		pos_dot;
	std::string	extension;

	pos_intero = uri.find_last_of("?");
	pos_dot = uri.find_last_of(".", pos_intero);

	if (pos_dot == uri.npos)
		return "";
	if (pos_intero == uri.npos)
		extension = uri.substr(pos_dot);
	else
		extension = uri.substr(pos_dot, pos_intero - pos_dot);
	return extension;
}

bool	_checkCgi(Client &client)
{
	std::string cgi[4];
	cgi[0] = ".py";
	cgi[2] = ".pl";
	cgi[3] = client.request.config.cgi;
	std::string	extension = _getFileExtension(client.request.uri);
	if (extension == "")
		return false;
	int i = 0;
	while (i < 4)
	{
		if (extension == cgi[i])
			return true;
		++i;
	}
	return false;
}

void	HandleGET(Client &client)
{
	if (client.request.config.methods.find("GET") == std::string::npos)
		client.response.status_code = NOTALLOWED;
	else
		client.response.status_code = OK;
	if (_checkCgi(client) == true)
	{
		if (client.response.status_code != OK)
			_construct_error(client.response, client.request);
		else
		{
			client.request.query_string = _getQueryString(client.request.uri);
			client.request.uri = _getNewUri(client.request.uri);
			_cgi(client);
		}
	}
	else
		openFile(client.response, client.request);
	buildHeader(client.response);
	client.response.res = client.response.headers + client.response.body;
}

void	HandlePOST(Client &client)
{
	if (client.request.config.methods.find("POST") == std::string::npos
			&& client.chunk.is_chunk == false)
		client.response.status_code = NOTALLOWED;
	else
		client.response.status_code = OK;
	if (client.chunk.is_chunk == true)
	{
		dechunk(client);
		return ;
	}
	post_handler(client); //build post respond
	buildHeader(client.response);
	client.response.res = client.response.headers + client.response.body;
}

void	HandleDELETE(Client &client)
{
	std::ifstream		file;
	std::stringstream	buffer;
	int					ret;

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
	if ((ret = isFileDir(client.response.path)) > 0)
	{
		file.open(client.response.path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			client.response.status_code = UNAUTHORIZED;
			getErrors(client.response, client.request, "/401.html");
			buildHeader(client.response);
			client.response.res = client.response.headers + client.response.body;
			return ;
		}
		file.close();
		if (remove(client.response.path.c_str()) == 0)
		{
			if (ret == 1)
			{
				client.response.status_code = OK;
				getErrors(client.response, client.request, "/200.html");
				client.response.body = "\tFile Deleted\n";
			}
			else if (ret == 2)
			{
				client.response.status_code = OK;
				getErrors(client.response, client.request, "/200.html");
				client.response.body = "\tDirectory Deleted\n";
			}
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

	if (client.request.method != "HEAD" && client.request.method != "PUT"
		&& client.request.method != "CONNECT" && client.request.method != "TRACE")
	{
		client.response.status_code = BADREQUEST;
		getErrors(client.response, client.request, "/400.html");
	}
	else
	{
		client.response.status_code = NOTIMPLEMENTED;
		getErrors(client.response, client.request, "/501.html");
	}
	buildHeader(client.response);
	client.response.res = client.response.headers + client.response.body;
}

void	Client::dispatcher(Client &client)
{
	get_basics(client.request, client.response);
	if (client.request.method == "GET")
		HandleGET(client);
	else if (client.request.method == "POST"|| client.chunk.is_chunk == true)
		HandlePOST(client);
	else if (client.request.method == "DELETE")
		HandleDELETE(client);
	else
		HandleBAD(client);
}
