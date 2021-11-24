/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 17:55:15 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/24 17:32:21 by gmaris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Env.hpp"

static void	ft_gnl(string &buffer, string &line)
{
	size_t					pos;

	pos = buffer.find("\n");
	if (pos != string::npos)
	{
		line = string (buffer, 0, pos++);
		buffer = buffer.substr(pos);
	}
	else
	{
		if (buffer[buffer.size() - 1] == '\n')
			buffer = buffer.substr(buffer.size());
		else
		{
			line = buffer;
			buffer = buffer.substr(buffer.size());
		}
	}
}

static string	eraseWhiteSpace(int i, int to, string str)
{
	while (str[i] == '\f' || str[i] == '\t' || str[i] == '\v'
	|| str[i] == '\n' || str[i] == '\r' || str[i] == ' ')
	{
		if (to == -1)
			str.erase(str.begin());
		else
			str.erase(i, to);
	}
	return (str);
}

Env::Env(string conf_file)
{
	std::cout << "file = " << conf_file << std::endl;

	
	return ;
}

Env::~Env(void) { return ;}

// void init_server_map(string conf_file)
// {
// 	Server			tmp;
// 	std::string		line;
// 	while (!conf_file.empty())
// 	{
// 		ft_gnl(conf_file, line);
// 		line = eraseWhiteSpace(0, -1, line);
// 		if (!line.compare(0, 6, "server"))
// 		{
// 			line = eraseWhiteSpace(6, 1, line);
// 			if (line[6] != '{')
// 				throw(Env::InvalidFile());
// 			if (!line.compare(0, 7, "server{"))
// 			{
// 				line = eraseWhiteSpace(7, 1, line);
// 				if (line[7])
// 					throw(Env::InvalidFile());
// 				if (getContent(conf_file, context, line, tmp) == false)
// 					throw (Env::InvalidFile());
// 				// if (pushServersConf(servers, tmp) == false)
// 				// 	throw(Env::InvalidFile());
// 				// tmp.clear();
// 				// context.clear();
// 			}
// 			else
// 				throw(Env::InvalidFile());
// 		}
// 		else if (line[0])
// 			throw(Env::InvalidFile());
// 	}
// }

// Servers->server->map[server][map containing]->map[location/index...][adress(ex index.html)]
void			init_server_map(std::string buffer)
{
	std::string				context;
	std::string				line;
	Server					tmp;

	while (!buffer.empty())
	{
		ft_gnl(buffer, line);
		line = eraseWhiteSpace(0, -1, line);
		if (!line.compare(0, 6, "server"))
		{
			line = eraseWhiteSpace(6, 1, line);
			if (line[6] != '{')
				throw(Env::InvalidFile());
			if (!line.compare(0, 7, "server{"))
			{
				line = eraseWhiteSpace(7, 1, line);
				if (line[7])
					throw(Env::InvalidFile());
				if (getContent(buffer, context, line, tmp) == false)
					throw (Env::InvalidFile());
			//	if (pushServersConf(servers, tmp) == false)
			//		throw(Env::InvalidFile());
			//	tmp.clear();
				context.clear();
			}
			else
				throw(Env::InvalidFile());
		}
		else if (line[0])
			throw(Env::InvalidFile());
	}
}

// void	Env::initFd(void)
// {
// 	std::vector<Server>::iterator it = _Servers.begin();
// 	FD_ZERO(&_RSet);
// 	FD_ZERO(&_WSet);
// 	FD_ZERO(&_ReadSet);
// 	FD_ZERO(&_WriteSet);
// 	_Timeout.tv_sec = 1;
// 	_Timeout.tv_usec = 0;
// 	while (it != _Servers.end())
// 	{
// 		it->init(&_ReadSet, &_WriteSet, &_RSet, &_WSet);
// 		it++;
// 	}
// }
// int				Env::getMaxFd(void)
// {
// 	std::vector<Server>::iterator	it = _Servers.begin();
// 	int								max = 0;
// 	int								fd;
// 	while (it != _Servers.end())
// 	{
// 		fd = it->getMaxFd();
// 		if (fd > max)
// 			max = fd;
// 		it++;
// 	}
// 	return (max);
// }
// int				Env::getOpenFd(void)
// {
// 	std::vector<Server>::iterator	it = _Servers.begin();
// 	int								nb = 0;
// 	while (it != _Servers.end())
// 	{
// 		nb++;
// 		nb += it->getOpenFd();
// 		it++;
// 	}
// 	return (nb);
// }
// void	Env::handleRequests(std::vector<Server>::iterator server)
// {
// 	std::vector<Client*>::iterator it_client = server->Clients.begin();
// 	while (it_client != server->Clients.end())
// 	{
// 		_Client = *it_client;
// 		if (FD_ISSET(_Client->fd, &_ReadSet))
// 			if (!server->readRequest(it_client))
// 				break ;
// 		if (FD_ISSET(_Client->fd, &_WriteSet))
// 			if (!server->writeResponse(it_client))
// 				break ;
// 		if (_Client->write_fd != -1)
// 			if (FD_ISSET(_Client->write_fd, &_WriteSet))
// 				_Client->writeFile();
// 		if (_Client->read_fd != -1)
// 			if (FD_ISSET(_Client->read_fd, &_ReadSet))
// 				_Client->readFile();
// 		it_client++;
// 	}
// }
// void	Env::launchWebserv(char *file)
// {
// 	int ret = 0;
// 	bool run = true;
// 	try
// 	{
// 		_Parser.getFile(file, _Servers);
// 		initFd();
// 	}
// 	catch (std::exception &e)
// 	{
// 		std::cerr << RED << "Error: " << NC << e.what() << std::endl;
// 		return ;
// 	}
// 	std::cout << GREEN << "Webserv ready !\n" << NC;
// 	while (run == true)
// 	{
// 		_ReadSet = _RSet;
// 		_WriteSet = _WSet;
// 		if (ret == 0)
// 			std::cout << "\rWaiting for connection\t⏳" << std::flush;
// 		ret = select(getMaxFd() + 1, &_ReadSet, &_WriteSet, NULL, &_Timeout);
// 		for (std::vector<Server>::iterator server(_Servers.begin()); server != _Servers.end(); ++server)
// 		{
// 			handleRequests(server);
// 			if (FD_ISSET(server->getFd(), &_ReadSet))
// 			{
// 				try
// 				{
// 					server->acceptConnection();
// 				}
// 				catch (std::exception &e)
// 				{
// 					std::cerr << RED <<  "Error: " << NC << e.what() << std::endl;
// 				}
// 			}
// 		//	if (!server->_tmp_clients.empty())
// 		//		if (FD_ISSET(server->_tmp_clients.front(), &_WriteSet))
// 				//erreur 503
// 		}
// 	}
// 	_Servers.clear();
// 	return ;
// }