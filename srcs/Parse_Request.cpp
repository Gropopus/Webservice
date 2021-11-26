/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse_Request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 17:53:23 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/26 16:48:38 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Client.hpp"
# include "Server.hpp"

void	getHeader(std::string &buffer, Request &request)
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
			{
				request.valid = false;
				return ;
			}
			std::cout << "key: " << key << std::endl;
			std::cout << "value: " << value << std::endl;
 			request.headers[key] = value;
 			request.headers[key].pop_back(); //remove \r
		}
		else
		{
			request.valid = false;
			return ;
		}
	}
}

void	getConf(Server &server, Request &request)
{
	std::vector<t_conf>::iterator it = server.config.begin();

	while (it != server.config.end())
	{
		if ((*it).location == request.uri)
		{
			request.config = (*it);
			return ;
		}
		it++;
	}
	request.valid = false;
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
	ft_gnl(buffer, request.version, '\n');
	request.version.pop_back();
	if (request.method.size() == 0 || request.uri.size() == 0
		|| request.version.size() == 0)
		request.valid = false;
	if (request.method != "GET" && request.method != "POST"
		&& request.method != "HEAD" && request.method != "PUT"
		&& request.method != "CONNECT" && request.method != "TRACE"
		&& request.method != "OPTIONS" && request.method != "DELETE")
		request.valid =  false;
	if (request.version != "HTTP/1.1")
		request.valid = false;
	getHeader(buffer, request);
	if (request.valid == true)
		getConf(*this, request);
	client.request = request;
}
