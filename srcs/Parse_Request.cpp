/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse_Request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 17:53:23 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/01 19:32:07 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Client.hpp"
# include "Server.hpp"

bool	getHeader(std::string &buffer, Request &request)
{
	size_t		i;
	std::string	line;
	std::string	key;
	std::string	value;

	while (!buffer.empty())
	{
		ft_gnl(buffer, line, '\n');
		if (line.size() < 1 || line[0] == '\n' || line[0] == '\r')
			break ;
		if (line.find(':') != std::string::npos)
		{
			i = line.find(':');
			key = line.substr(0, i);
			if (line[i + 1] == ' ')
				value = line.substr(i + 2);
			else
				value = line.substr(i + 1);
			if (std::isspace(value[0]) || std::isspace(key[0]) || value.empty() || key.empty())
				return (false);
	//		std::cout << "key: " << key << std::endl;
	//		std::cout << "value: " << value << std::endl;
 			request.headers[key] = value;
 			request.headers[key].pop_back(); //remove \r
		}
		else
			return (false);
	}
	request.body = buffer;
	return (true);
}

void	getConf(Server &server, Request &request)
{
	std::vector<t_conf>::iterator it = server.config.begin();
	size_t pos = -1;
	size_t i = -1;
	int ret = 0;

	while (it != server.config.end())
	{
		if (request.uri.find((*it).location) != std::string::npos)
		{
			pos = request.uri.find((*it).location);
			i = request.uri.find("/");
			if ((request.uri[(*it).location.size()] == '/' || request.uri[(*it).location.size()] == '\0') && i == pos)
			{
				request.config = (*it);
				ret++;
			}
		}
		it++;
	}
	if (ret == 0)
		request.config = server.config[0];
}

void	Server::ParseRequest(Client &client)
{
	std::string buffer = client.Buf;
	Request		request;
	request.valid = true;

	if (buffer[0] == '\r')
		buffer.erase(buffer.begin());
	if(buffer[0] == '\n')
		buffer.erase(buffer.begin());
	ft_gnl(buffer, request.method, ' ');
	ft_gnl(buffer, request.uri, ' ');
//	request.uri.erase(0, 1); erase /
	ft_gnl(buffer, request.version, '\n');
	request.version.pop_back();
	if (request.method.size() == 0 || request.uri.size() == 0
		|| request.version.size() == 0)
	{
		request.valid = false;
		request.status_code = BADREQUEST;
	}
	if (request.method != "GET" && request.method != "POST"
		&& request.method != "HEAD" && request.method != "PUT"
		&& request.method != "CONNECT" && request.method != "TRACE"
		&& request.method != "OPTIONS" && request.method != "DELETE")
	{
		request.status_code = BADREQUEST;
		request.valid =  false;
	}
	if (request.version != "HTTP/1.1")
		request.valid = false;
	if (getHeader(buffer, request) == false)
	{	
		request.valid = false;
		request.status_code = BADREQUEST;
	}
	if (request.valid == true)
		getConf(*this, request);
	request.server_name = this->_Name;
	request.errors = this->_Error;
	client.request = request;
}
