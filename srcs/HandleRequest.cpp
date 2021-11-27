/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 16:49:19 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/27 12:43:08 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Client.hpp"
#include "Web_serv.hpp"

void	buildHeader(Response &response)
{
	response.headers = response.version + ' ' +  response.status_code + "\n";
	response.headers += "Content-Length: " + std::to_string(response.body_len) + "\n"; 
	response.headers += "Content-Type: " + response.content_type + "\n";
	response.headers += "Date: " + ft_getDate() + "\n";
	response.headers += "Last-Modified: ";
	response.headers += "Location: " + response.location + "\n";
	if (response.name != "")
		response.headers += "Server: " + response.name + "\n";
	else
		response.headers += "Server: Webserv\n";
}



void	HandleGET(Client &client)
{
	if (client.request.config.methods.find("GET") == std::string::npos)
		client.response.status_code = NOTALLOWED;
}

void	HandlePOST(Client &client)
{
	if (client.request.config.methods.find("POST") == std::string::npos)
		client.response.status_code = NOTALLOWED;
}

void	HandleDELETE(Client &client)
{
	if (client.request.config.methods.find("DELETE") == std::string::npos)
		client.response.status_code = NOTALLOWED;
}

void	HandleBAD(Client &client)
{
	if (client.request.method != "HEAD" && client.request.method != "PUT"
		&& client.request.method != "CONNECT" && client.request.method != "TRACE")
		client.response.status_code = NOTIMPLEMENTED;
	else
		client.response.status_code = BADREQUEST;
}

void	dispacher(Client &client)
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
