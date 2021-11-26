/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse_Request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 17:53:23 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/26 16:06:44 by thsembel         ###   ########.fr       */
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
	if (request.valid == true)
		getConf(*this, request);
	client.request = request;
}
