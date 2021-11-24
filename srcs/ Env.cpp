/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*    Env.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 17:55:15 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/24 15:36:27 by gmaris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Env.hpp"

Env::Env(void) { return ;}

Env::~Env(void) { return ;}

void	Env::initFd(void)
{
	std::vector<Server>::iterator it = _Servers.begin();
	FD_ZERO(&_RSet);
	FD_ZERO(&_WSet);
	FD_ZERO(&_ReadSet);
	FD_ZERO(&_WriteSet);
	_Timeout.tv_sec = 1;
	_Timeout.tv_usec = 0;
	while (it != _Servers.end())
	{
		it->init(&_ReadSet, &_WriteSet, &_RSet, &_WSet);
		it++;
	}
}

int				Env::getMaxFd(void)
{
	std::vector<Server>::iterator	it = _Servers.begin();
	int								max = 0;
	int								fd;

	while (it != _Servers.end())
	{
		fd = it->getMaxFd();
		if (fd > max)
			max = fd;
		it++;
	}
	return (max);
}

int				Env::getOpenFd(void)
{
	std::vector<Server>::iterator	it = _Servers.begin();
	int								nb = 0;

	while (it != _Servers.end())
	{
		nb++;
		nb += it->getOpenFd();
		it++;
	}
	return (nb);
}


void	Env::handleRequests(std::vector<Server>::iterator server)
{

	std::vector<Client*>::iterator it_client = server->Clients.begin();
	while (it_client != server->Clients.end())
	{
		_Client = *it_client;
		if (FD_ISSET(_Client->fd, &_ReadSet))
			if (!server->readRequest(it_client))
				break ;
		if (FD_ISSET(_Client->fd, &_WriteSet))
			if (!server->writeResponse(it_client))
				break ;
		if (_Client->write_fd != -1)
			if (FD_ISSET(_Client->write_fd, &_WriteSet))
				_Client->writeFile();
		if (_Client->read_fd != -1)
			if (FD_ISSET(_Client->read_fd, &_ReadSet))
				_Client->readFile();
		it_client++;
	}
}

void	Env::launchWebserv(char *file)
{
	int ret = 0;
	bool run = true;
	try
	{
		_Parser.getFile(file, _Servers);
		initFd();
	}
	catch (std::exception &e)
	{
		std::cerr << RED << "Error: " << NC << e.what() << std::endl;
		return ;
	}
	std::cout << GREEN << "Webserv ready !\n" << NC;
	while (run == true)
	{
		_ReadSet = _RSet;
		_WriteSet = _WSet;
		if (ret == 0)
			std::cout << "\rWaiting for connection\t⏳" << std::flush;
		ret = select(getMaxFd() + 1, &_ReadSet, &_WriteSet, NULL, &_Timeout);
		for (std::vector<Server>::iterator server(_Servers.begin()); server != _Servers.end(); ++server)
		{
			handleRequests(server);
			if (FD_ISSET(server->getFd(), &_ReadSet))
			{
				try
				{
					server->acceptConnection();
				}
				catch (std::exception &e)
				{
					std::cerr << RED <<  "Error: " << NC << e.what() << std::endl;
				}
			}
		//	if (!server->_tmp_clients.empty())
		//		if (FD_ISSET(server->_tmp_clients.front(), &_WriteSet))
				//erreur 503
		}
	}
	_Servers.clear();
	return ;
}
