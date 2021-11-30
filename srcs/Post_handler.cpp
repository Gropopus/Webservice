/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post_handler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:41:22 by gmaris            #+#    #+#             */
/*   Updated: 2021/11/30 16:40:05 by gmaris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Client.hpp"
# include "Server.hpp"

static std::string _getPart(const string &body, const string &boundary)
{
	std::string part;
	size_t start;
	size_t end;
	string start_str = "--";
	start_str += boundary;
	string end_str = "--";
	end_str += boundary;

	start = body.find(start_str) + start_str.length();
	end = body.find(end_str, start + 1);
	std::cout << "end = " << end_str << std::endl;
	std::cout << CYAN << "start = " << start << "  end = " << end << std::endl;
	std::cout << BLUE;
	part = body.substr(start, end - start);
	return part;
}

static void	_multiPost(Client &client)
{
	std::string	post;
	std::string	boundary;
	
	size_t	start;

	start = client.request.headers["Content-Type"].find("boundary=");
	boundary = client.request.headers["Content-Type"].substr(start + 9);
	post = _getPart(client.request.body, boundary);
	std::cout << post << std::endl;
}

void	post_handler(Client &client)
{
	size_t t;
	std::cout << std::endl << std::endl;
	std::cout << BLUE << "\t======POST_HANLDER DEBUG START HERE======" << std::endl;
	t = client.request.headers["Content-Type"].find("multipart/form-data");
	if ((t = client.request.headers["Content-Type"].find("multipart/form-data")) !=
		client.request.headers["Content-Type"].npos)
	{
		std::cout << "t = " << t << std::endl;
		std::cout << "multi part = " << client.request.headers["Content-Type"] << std::endl;
		std::cout << "got multiple post to handle" << std::endl;
		_multiPost(client);
	}
	//if (client->request.headers["Content-Type"] == "application/x-www-form-urlencoded")

	std::cout << "\t======POST_HANLDER DEBUG  END  HERE======" << NC << std::endl;
	std::cout << std::endl << std::endl;
	//if resources has been create respond shoudl be 201 (created)response
	//containing a Location header field that provides an identifier for
	//the primary resource created
}