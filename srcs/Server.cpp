/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:17:55 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/24 16:20:06 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) : _Fd(-1), _MaxFd(-1), _Port(-1)
{
	std::memset(&_Info, 0, sizeof(_Info));
}

Server::~Server(void)
{
	Client		*client = NULL;

	if (_Fd != -1)
	{
		for (std::vector<Client*>::iterator it(Clients.begin()); it != Clients.end(); ++it)
		{
			client = *it;
			*it = NULL;
			if (client)
				delete client;
		}
		while (!_tmp_clients.empty())
		{
			close(_tmp_clients.front());
			_tmp_clients.pop();
		}
		Clients.clear();
		close(_Fd);
		FD_CLR(_Fd, _RSet);
		std::cout << "Port:\t[" << GREEN << _Port << NC << + "]\t[";
		std::cout << RED << "closed" << NC << "]\n";
	}
}

int		Server::getMaxFd()
{
	Client	*client;

	for (std::vector<Client*>::iterator it(Clients.begin()); it != Clients.end(); ++it)
	{
		client = *it;
		if (client->read_fd > _MaxFd)
			_MaxFd = client->read_fd;
		if (client->write_fd > _MaxFd)
			_MaxFd = client->write_fd;
	}
	return (_MaxFd);
}

int		Server::getFd() const
{
	return (_Fd);
}

int		Server::getOpenFd()
{
	int 	nb = 0;
	Client	*client;
	std::vector<Client*>::iterator it = Clients.begin();
	
	while (it != Clients.end())
	{
		client = *it;
		nb += 1;
		if (client->read_fd != -1)
			nb += 1;
		if (client->write_fd != -1)
			nb += 1;
		it++;
	}
	nb += _tmp_clients.size();
	return (nb);
}

void	Server::init(fd_set *readSet, fd_set *writeSet, fd_set *rSet, fd_set *wSet)
{
	int				ptr = 1;
	std::string		serv;
	std::string		host;

	_ReadSet = readSet;
	_WriteSet = writeSet;
	_WSet = wSet;
	_RSet = rSet;
	serv = _Conf[0]["server|"]["listen"];
	errno = 0;
	if ((_Fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw(ServerFailure("Server unable to create the reauested socket."));
	if (setsockopt(_Fd, SOL_SOCKET, SO_REUSEADDR, &ptr, sizeof(int)) == -1)
		throw(ServerFailure("setsockopt function returned an error."));
	if (serv.find(":") != std::string::npos)
	{
		host = serv.substr(0, serv.find(":"));
		if((_Port = ft_stoi(serv)) < 0)
			throw(ServerFailure("Requested Port unavailable."));
		_Info.sin_addr.s_addr = inet_addr(host.c_str());
		_Info.sin_port = htons(_Port);
	}
	else
	{
		_Info.sin_addr.s_addr = INADDR_ANY;
		if((_Port = ft_stoi(serv)) < 0)
			throw(ServerFailure("Requested Port unavailable."));
		_Info.sin_port = htons(_Port);
	}
	_Info.sin_family = AF_INET;
	if (bind(_Fd, (struct sockaddr *)&_Info, sizeof(_Info)) == -1)
		throw(ServerFailure("Server failed to bind sockets."));
	if (listen(_Fd, MAX_FD) == -1)
		throw(ServerFailure("Server unable to listen the requested file descriptor."));
	if (fcntl(_Fd, F_SETFL, O_NONBLOCK) == -1)
		throw(ServerFailure("fcntl function returned an error."));
	FD_SET(_Fd, _RSet);
	_MaxFd = _Fd;
	std::cout << "Port:\t[" << GREEN << _Port << NC << "]\tlistening...\n";
}
