/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse_Request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 17:53:23 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/26 15:21:17 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Client.hpp"
# include "Server.hpp"

/*	Request				request;
	std::string			buffer;

	buffer = std::string(client.rBuf);
	if (buffer[0] == '\r')
		buffer.erase(buffer.begin());
	if (buffer[0] == '\n')
		buffer.erase(buffer.begin());
	ft_getline(buffer, request.method, ' ');
	ft_getline(buffer, request.uri, ' ');
	ft_gnl(buffer, request.version);
	request.valid = true;
	if (request.method.size() == 0 || request.uri.size() == 0
		|| request.version.size() == 0)
		request.valid = false;
	else if (request.method != "GET" && request.method != "POST"
		&& request.method != "HEAD" && request.method != "PUT"
		&& request.method != "CONNECT" && request.method != "TRACE"
		&& request.method != "OPTIONS" && request.method != "DELETE")
		request.valid =  false;
	else if (request.version != "HTTP/1.1\r" && request.version != "HTTP/1.1")
		request.valid = false;
	parseHeaders(buffer, request);
	if (request.uri != "*")
		getConf(client, request, conf);
	if (request.valid)
	{
		if (client.conf["root"][0] != '\0')
			chdir(client.conf["root"].c_str());
		//si ca marche pas.
		if (request.method == "POST")
		{
			client.status = BODYPARSING;
			parseBody(client);
		}
		else
			client.status = CODE;
	}
	else if (request.valid == false)
	{
		request.method = "BAD";
		client.status = CODE;
	}
	client.req = request;
*/
bool		parseHeaders(std::string &buf, Request &req)
{
	size_t		i;
	std::string	line;
	std::string	key;
	std::string	value;

	while (!buf.empty())
	{
		ft_gnl(buf, line, '\n');
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
				req.valid = false;
				return (false);
			}
 			req.headers[key] = value;
 			req.headers[key].pop_back(); //remove '\r'
		}
		else
		{
			req.valid = false;
			return (false);
		}
	}
	return (true);
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
	if (request.method.size() == 0 || request.uri.size() == 0
		|| request.version.size() == 0)
		request.valid = false;
	if (request.method != "GET" && request.method != "POST"
		&& request.method != "HEAD" && request.method != "PUT"
		&& request.method != "CONNECT" && request.method != "TRACE"
		&& request.method != "OPTIONS" && request.method != "DELETE")
		request.valid =  false;
	if (request.version != "HTTP/1.1" && request.version != "HTTP1/1\r")
		request.valid = false;
	client.request = request;
}
