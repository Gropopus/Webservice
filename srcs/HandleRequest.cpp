/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 16:49:19 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/26 17:17:42 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Client.hpp"
#include "Web_serv.hpp"

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
