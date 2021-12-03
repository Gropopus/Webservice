/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Web_serv.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:21:24 by gmaris            #+#    #+#             */
/*   Updated: 2021/12/03 12:15:14 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_SERV_HPP
#define WEB_SERV_HPP

//Class include
//# include "Server.hpp"
//# include "Env.hpp"
//# include "Client.hpp"

/* C++ */
# include <iostream>
# include <vector>
# include <map>
# include <iterator>
# include <string>
# include <fstream>
# include <sstream>

/* C */
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/stat.h>
#include <sys/time.h>
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define NC		"\033[0m"
#define BUFFER_SIZE		10000

#define OK 				"200 OK"
#define CREATED			"201 Created"
#define NOCONTENT		"204 No Content"
#define NOTMODIFIED		"304 Not Modified"
#define BADREQUEST		"400 Bad Request"
#define UNAUTHORIZED	"401 Unauthorized"
#define FORBIDDEN		"403 Forbidden"
#define NOTFOUND 		"404 Not Found"
#define NOTALLOWED		"405 Method Not Allowed"
#define REQTOOLARGE		"413 Request Entity Too Large"
#define UNAVAILABLE		"503 Service Unavailable"
#define NOTIMPLEMENTED	"501 Not Implemented"
#define INTERNALERROR	"500 Internal Server Error"

struct	t_conf
{
	std::string location;
	std::string index;
	std::string root;
	std::string methods;
	int			port;
	int			max_body;
	std::string cgi;
	std::string exec;
	std::string auth;
	std::string php;
	bool		autoIndex;

	void	clear(void)
	{
		index.clear();
		root.clear();
		methods.clear();
		max_body = -1;
		cgi.clear();
		exec.clear();
		auth.clear();
		php.clear();
		port = 0;
		autoIndex = false;
	}
};

struct Request
{
	struct t_conf						config;
	bool								valid;
	std::string							method;
	std::string							uri;
	std::string							version;
	std::map<std::string, std::string> 	headers;
	std::string							body;
	std::string							status_code;
	std::string							server_name;
	std::string							errors;

	void	clear()
	{
		method.clear();
		uri.clear();
		version.clear();
		headers.clear();
		body.clear();
		status_code.clear();
		server_name.clear();
		errors.clear();
	}
};

struct Response
{
	std::string							version;
	std::string							status_code;
	std::string							headers;
	std::string							content_type;
	std::string							name;
	std::string							location;
	size_t								body_len;
	std::string							path;
	//std::map<std::string, std::string> 	headers;
	std::string							body;
	std::string							res;

	void	clear()
	{
		body_len = 0;
		path.clear();
		location.clear();
		name.clear();
		content_type.clear();
		version.clear();
		status_code.clear();
		headers.clear();
		body.clear();
		res.clear();
	}
};

class Server;
class Client;
//typedef std::string string;

std::string		eraseWhiteSpace(int i, int to, std::string str);
std::string		ft_getDate(void);
std::string		get_path(Request &request);
int				ft_stoi(std::string str);
int				isFileDir(std::string path);
bool			is_only(std::string str);
void			ft_gnl(std::string &buffer, std::string &line, char to);
void			create_autoIndex(Request &request, Response &response);
void			post_handler(Client &client);
void			buildHeader(Response &response);
void			getErrors(Response &response, Request &request, std::string error);
#endif
