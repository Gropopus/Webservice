/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 13:33:46 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/02 13:43:45 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include <dirent.h>

std::string		createPage(Request &request)
{
	DIR			*dir = opendir(request.config.root.c_str());
	struct dirent *dirAccess = readdir(dir);
	std::stringstream strstream;
	std::string	index = "<html>\n\
	<head>\n\
	<title>" + request.config.location + "</title>\n\
	</head>\n\
	<body>\n\
	<h1>AUTO INDEX</h1>\n\
	<p>\n";
	if (dir == NULL)
	{
		std::cerr << RED << "Error: " << NC << "could not open " << request.config.root << std::endl;
		return "";
	}
	while (dirAccess)
	{
		strstream << "\t\t<p><a href=\"http://localhost:"\
			<< request.config.port << request.config.location\
			+ std::string(dirAccess->d_name)\
			+ "\">" + std::string(dirAccess->d_name) + "</a></p>\n";
		index += strstream.str();
		strstream.str("");
		dirAccess = readdir(dir);
	}
	index+= "</p>\n</body>\n</html>\n";
	closedir(dir);
	return (index);
}

void	create_autoIndex(Request &request, Response &response)
{
	response.body = createPage(request);
	std::cout << RED << response.body << NC;
	response.body_len = response.body.size();
	response.content_type = "text/html";
	response.status_code = OK;
	buildHeader(response);
	response.res = response.headers + response.body;
}
