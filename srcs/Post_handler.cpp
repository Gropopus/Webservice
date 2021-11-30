/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post_handler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:41:22 by gmaris            #+#    #+#             */
/*   Updated: 2021/11/30 22:34:17 by gmaris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Client.hpp"
# include "Server.hpp"

static std::string _get_path(Request &request)
{
	std::string path;

	if (request.uri.find(request.config.location) != std::string::npos && request.config.location != "/")
		path = request.config.root + request.uri.substr(request.uri.find_last_of('/'));
	else
		path = request.config.root + request.uri;
	
	return (path);
}

static string	_getFileName(string &elem, Client &client)
{
	size_t start;
	size_t end;
	string	name = _get_path(client.request);

	start = elem.find("filename=\"") + 10;
	end   = elem.find("\"", start);

	if (name.back() != '/')
		name += "/";
	name += elem.substr(start, end - start);
	return (name);
}

static bool	_isDirectory(string &path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			std::cout << "URI IS DIR" << std::endl;
 			return true;
		}
	}
	return false;
}

static bool	_fileExist(string &path)
{
	std::ifstream f(path.c_str());
	return f.good();
}

static bool	_isTxt(string &elem)
{
	size_t first_line = elem.find("\r\n");
	size_t type_pos = elem.find("Content-Type: ", first_line) + 14;
	if (elem.find("text/", type_pos) == type_pos)
		return true;
	return false;
}

bool	_putFile(string &path, string &elem, Client &client)
{
	std::ofstream	outfile;
	
	if (_isTxt(elem))
		outfile.open(path, std::ofstream::out);
	else
		outfile.open(path, std::ofstream::binary);
	std::cout << "open file " << path << std::endl;
	if (outfile.fail())
	{
		client.response.status_code = FORBIDDEN;
		client.response.status_code = INTERNALERROR;
	}
	outfile << elem.substr(elem.find("\r\n\r\n") + 4);
	outfile.close();
	client.response.status_code = CREATED;
	client.response.status_code = NOCONTENT;
	return true;
}

static int	_resolvePart(string &elem, Client &client)
{
	size_t first_line = elem.find("\r\n");
	if (elem.find("Content-Type: ", first_line) < elem.find("\r\n\r\n", first_line))
	{
		std::cout << "THIS IS A FUCKING FILE" << std::endl;
		string path = _getFileName(elem, client);
		std::cout << "path file to create = [" << path << "]" << std::endl;
	//check permission
		if ((_isDirectory(client.request.uri) == true && _fileExist(path) == false) ||
			_fileExist(client.request.uri) == false)
		{
			std::cout << "file doesn't exist :D" << std::endl;
			_putFile(path, elem, client);
		}
		else
			std::cout << "file exist :'(" << std::endl;
	}
	else
	{
		std::cout << "is not a file or uri isn't a directory" << std::endl;
		//cgi;
	}

	return 0;
}

static std::string _getPart(string &body, const string &boundary)
{
	std::string part;
	size_t start;
	size_t end;
	string str = "--";
	str += boundary;

	start = body.find(str) + str.length();
	end = body.find(str, start + 1);
	if (start == body.npos || end == body.npos)
	{
		body = "";
		return ("");
	}
	part = body.substr(start, end - start);
	body = body.substr(end);
	
	return part;
}

static void	_multiPost(Client &client)
{
	std::string	post = "tmp";
	std::string	boundary;
	
	size_t	start;

	start = client.request.headers["Content-Type"].find("boundary=");
	boundary = client.request.headers["Content-Type"].substr(start + 9);

	while (post != "")
	{
		post = _getPart(client.request.body, boundary);
		if (post != "")
		{
			_resolvePart(post, client);
		//	std::cout << YELLOW << "next "<<BLUE << std::endl;
		}
	}
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
		std::cout << "got multiple post to handle" << std::endl << std::endl;
		_multiPost(client);
	}
	//if (client->request.headers["Content-Type"] == "application/x-www-form-urlencoded")

	std::cout << "\t======POST_HANLDER DEBUG  END  HERE======" << NC << std::endl;
	std::cout << std::endl << std::endl;
	//if resources has been create respond shoudl be 201 (created)response
	//containing a Location header field that provides an identifier for
	//the primary resource created
}