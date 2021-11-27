/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 16:49:19 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/27 13:10:24 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Client.hpp"
#include "Web_serv.hpp"

void	buildHeader(Response &response)
{
	response.headers = response.version + ' ' +  response.status_code + "\r\n";
	response.headers += "Content-Length: " + std::to_string(response.body_len) + "\r\n"; 
	response.headers += "Content-Type: " + response.content_type + "\r\n";
	response.headers += "Date: " + ft_getDate() + "\r\n";
	response.headers += "Last-Modified: \r\n";
	response.headers += "Location: " + response.location + "\r\n";
	if (response.name != "")
		response.headers += "Server: " + response.name + "\r\n";
	else
		response.headers += "Server: Webserv\r\n";
}



void	HandleGET(Client &client)
{
	if (client.request.config.methods.find("GET") == std::string::npos)
		client.response.status_code = NOTALLOWED;
	else
		client.response.status_code = OK;
	

	
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
