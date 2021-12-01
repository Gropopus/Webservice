/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post_handler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:41:22 by gmaris            #+#    #+#             */
/*   Updated: 2021/12/01 17:42:20 by gmaris           ###   ########.fr       */
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
	
	if (path.back() != '/')
		path += "/";
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
		outfile.open(path, std::ofstream::out);
	else
		outfile.open(path, std::ofstream::binary);
	std::cout << "open file " << path << std::endl;
	if (outfile.fail())
	{
		client.response.status_code = INTERNALERROR;
		return false;
	}
	outfile << elem.substr(elem.find("\r\n\r\n") + 4);
	outfile.close();
	std::cout << "\t file created" << std::endl;
	client.response.status_code = CREATED;
	client.response.status_code = NOCONTENT;
	return true;
}

static bool _Permission(string path, Client &client)
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

static bool	_resolvePart(string &elem, Client &client)
{
	//
	//cgi
	//
	size_t first_line = elem.find("\r\n");
	if (elem.find("Content-Type: ", first_line) < elem.find("\r\n\r\n", first_line))
	{
		if (_getFileName(elem) == "")
			return true;
		string path = _get_path(client.request);
		if (_Permission(path, client) == false)
			return false;
		path += _getFileName(elem);
		std::cout << "path file to create = [" << path << "]" << std::endl;
		if (_isDirectory(client.request.uri) == true && _fileExist(path) == false)
		{
			std::cout << "create file from concat uri + file name" << std::endl;
			if (_putFile(path, elem, client) == false)
				return false;
		}
		else if (_fileExist(client.request.uri) == false)
		{
			std::cout << "create file from concat uri " << std::endl;
			if (_putFile(client.request.uri, elem, client) == false)
				return false;
		}
		else
		{
			std::cout << "file exist don't change it :'(  uri = " << client.request.uri << std::endl;
		}
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