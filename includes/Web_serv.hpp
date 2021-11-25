/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Web_serv.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:21:24 by gmaris            #+#    #+#             */
/*   Updated: 2021/11/25 16:46:39 by thsembel         ###   ########.fr       */
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

struct Request
{
	bool								valid;
	std::string							method;
	std::string							uri;
	std::string							version;
	std::map<std::string, std::string> 	headers;
	std::string							body;

	void	clear()
	{
		method.clear();
		uri.clear();
		version.clear();
		headers.clear();
		body.clear();
	}
};

struct Response
{
	std::string							version;
	std::string							status_code;
	std::map<std::string, std::string> 	headers;
	std::string							body;

	void	clear()
	{
		version.clear();
		status_code.clear();
		headers.clear();
		body.clear();
	}
};

class Server;
typedef std::string string;

void	ft_gnl(string &buffer, string &line);
string	eraseWhiteSpace(int i, int to, string str);
int		ft_stoi(std::string str);
#endif
