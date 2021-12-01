/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post_handler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:41:22 by gmaris            #+#    #+#             */
/*   Updated: 2021/12/01 19:41:35 by gmaris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Client.hpp"
# include "Server.hpp"



static std::string _get_path(Request &request)
{
	std::string path;

	if (request.uri == request.config.location || request.uri == request.config.location + "/")
		path = request.config.root + "/";
	else
	{
		if (request.uri.find(request.config.location)  == 0)
			path = request.config.root + "/" + request.uri.substr(request.config.location.length());
		else
			std::cout << RED << "THIS SHOULD NEVER HAPPEND WTF FIX THIS SHIT CODE" << NC << std::endl;
	}
	return (path);
}
static string	_getFileName(string &elem)
{
	size_t start;
	size_t end;
	string name = "";
	start = elem.find("filename=\"") + 10;
	end   = elem.find("\"", start);

	name += elem.substr(start, end - start);
	return (name);
}

static string	_getUriName(Request &request)
{
	std::string name;
	name = request.uri.substr(request.uri.find_last_of('/') + 1);
	return name;
}

/*

	static bool	_fileExist(string &path)
	{
		std::ifstream f(path.c_str());
		return f.good();
	}

	static bool	_Permission(string path, Client &client)
	{
		std::ifstream f;
		f.open(path);
		if (f.good() == false)
		{
			client.response.status_code = FORBIDDEN;
			return false;
		}
		f.close();
		return true;
	}
*/

static bool	_isDirectory(string &path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			return true;
	}
	return false;
}

static bool	_isTxt(string &elem)
{
	size_t first_line = elem.find("\r\n");
	size_t type_pos = elem.find("Content-Type: ", first_line) + 14;
	if (elem.find("text/", type_pos) == type_pos)
		return true;
	return false;
}

static bool	_DirExist(string &path)
{
	string	path_dir;
	
	path_dir = path.substr(0, path.find_last_of('/'));
	if (_isDirectory(path_dir) == true)
		return true;
	return true;
}

static bool	_putFile(string &path, string &elem, Client &client)
{
	std::ofstream	outfile;
	
	if (_DirExist(path) == false)
	{
		client.response.status_code = FORBIDDEN;
		std::cout << "No directory to create file" << std::endl;
		return false;
	}
	if (_isTxt(elem))
		outfile.open(path, std::ofstream::out | std::ofstream::trunc);
	else
		outfile.open(path, std::ofstream::binary | std::ofstream::trunc);
	std::cout << "open file " << path << std::endl;
	if (outfile.fail())
	{
		client.response.status_code = INTERNALERROR;
		return false;
	}
	if (elem.substr(elem.find("\r\n\r\n") + 4) == "")
	{
		client.response.status_code = NOCONTENT;
		std::cout << "file is empty" << std::endl;
	}
	else
	{
		outfile << elem.substr(elem.find("\r\n\r\n") + 4);
		client.response.status_code = CREATED;
	}
	outfile.close();
	std::cout << "\t file created" << std::endl;
	return true;
}

static bool	_resolvePart(string &elem, Client &client)
{
	string	path;
	string	name;
	string	path_name;
	//
	//cgi
	//
	size_t first_line = elem.find("\r\n");
	if (elem.find("Content-Type: ", first_line) < elem.find("\r\n\r\n", first_line))
	{
		path = _get_path(client.request);
		name = _getFileName(elem);
		path_name = path;
		path_name += name;
		if (path.back() != '/')
		{
			std::string tmp = _getUriName(client.request);
			if (name == tmp)
			{
				std::cout << CYAN << "Ok to replace file"<<path_name << NC;;//ok to replace file
				return _putFile(path_name, elem, client);
			}
		}
		std::cout << CYAN << "Ok to create/replace file\n"<<path_name << NC;
		return _putFile(path_name, elem, client);
	}
	else
	{
		std::cout << "is not a file or uri isn't a directory" << std::endl;
	}

	return true;
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
		}
	}
}

void	post_handler(Client &client)
{
	size_t t;
	std::cout << std::endl << std::endl;
	std::cout << BLUE << "\t======POST_HANLDER DEBUG START HERE======" << NC << std::endl;
	t = client.request.headers["Content-Type"].find("multipart/form-data");
	if ((t = client.request.headers["Content-Type"].find("multipart/form-data")) !=
		client.request.headers["Content-Type"].npos)
	{
		std::cout << "got multiple post to handle" << std::endl << std::endl;
		_multiPost(client);
	}
	else
	{
		std::cout << RED << "only one part" << std::endl;
	}
	//if (client->request.headers["Content-Type"] == "application/x-www-form-urlencoded")

	std::cout << BLUE << "\t======POST_HANLDER DEBUG  END  HERE======" << NC << std::endl;
	std::cout << std::endl << std::endl;
	//if resources has been create respond shoudl be 201 (created)response
	//containing a Location header field that provides an identifier for
	//the primary resource created
}