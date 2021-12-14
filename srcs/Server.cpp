/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:17:55 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/14 17:37:14 by gmaris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
# define MAX_FD 256

Server::Server(void) : _Port(-1), _Fd(-1), _MaxFd(-1) { return ;}

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
		config.clear();
 		Clients.clear();
 		close(_Fd);
 		FD_CLR(_Fd, _RSet);
 		std::cout << ft_getDate() << "\t";
		std::cout << "Port:\t[" << GREEN << _Port << NC << + "]\t[";
		std::cout << RED << "closed" << NC << "]\n";
 	}
}

int		Server::getMaxFd()
{
	Client	*client = NULL;

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

	_ReadSet = readSet;
	_WriteSet = writeSet;
	_WSet = wSet;
	_RSet = rSet;
	std::memset(&_Info, 0, sizeof(_Info));
	if ((_Fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw(ServerFailure("Server unable to create the reauested socket."));
	if (setsockopt(_Fd, SOL_SOCKET, SO_REUSEADDR, &ptr, sizeof(int)) == -1)
		throw(ServerFailure("setsockopt function returned an error."));
	_Info.sin_addr.s_addr = INADDR_ANY;
	if(_Port < 0)
		throw(ServerFailure("Requested Port unavailable."));
	_Info.sin_port = htons(_Port);
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

void	Server::acceptConnection(void)
{
	int 				fd = -1;
	struct sockaddr_in	info;
	socklen_t			len;
	Client				*newOne = NULL;

	std::memset(&info, 0, sizeof(struct sockaddr));
	len = sizeof(struct sockaddr);
	if ((fd = accept(_Fd, (struct sockaddr *)&info, &len)) == -1)
		throw(ServerFailure("Server is unable to accept the incoming connection\n"));
	if (fd > _MaxFd)
		_MaxFd = fd;
	std::vector<Client*>::iterator it = Clients.begin();
	while (it != Clients.end())
	{
		if ((*it)->ip == inet_ntoa(info.sin_addr))
		{
			(*it)->Buf.clear();
			return ;
		}
		it++;
	}
	newOne = new Client(fd, _RSet, _WSet, info);
	newOne->chunk.is_chunk = false;
	newOne->chunk.finish = false;
	Clients.push_back(newOne);
}

int		Server::readRequest(std::vector<Client*>::iterator it)
{
	int			ret;
	Client		*client = NULL;
	char			Buffer[BUFFER_SIZE];

	client = *it;
	std::memset(Buffer, 0, BUFFER_SIZE);
	ret = recv(client->fd, Buffer, BUFFER_SIZE - 1, 0);
	if (ret == 0 || ret == -1)
	{
		*it = NULL;
		close(client->fd);
		Clients.erase(it);
		if (client)
			delete client;
		if (!ret)
		{
			std::cout << ft_getDate() << "\t";
			std::cout << "Client " << RED << "closed" << NC << " the connection on port:\t[";
			std::cout << GREEN << _Port << NC << "]\n" << std::endl;
		}
		else
		{
			std::cout << ft_getDate() << "\t";
			std::cout << "The connection was \t" << RED << "closed" << NC << " on port:\t[";
			std::cout << GREEN << _Port << NC << "] due to a recv error." << std::endl;
		}
		return (0);
	}
	else
	{
		std::string tmp;
		int i;
		i = 0;
		while (i < ret)
		{
			tmp.push_back(Buffer[i]);
			++i;
		}
		client->Buf = tmp;
		//std::cout << YELLOW << std::endl << tmp << std::endl << NC;
		std::cout << "\n" << ft_getDate() << "\t";
		std::cout << "Port:\t[" << GREEN << std::to_string(_Port) << NC << "]\tClient connected";
		std::cout << "\tIP: " << GREEN << client->ip << ":" << client->port << NC << std::endl;
		std::cout << ft_getDate() << "\t";
		ParseRequest(*client);
		client->setFdSets(true, 1);
		client->dispatcher(*client);
		return (1);
	}
	return (-1);
}


int		Server::writeResponse(std::vector<Client*>::iterator it)
{
	unsigned long	size = 0;
	Client			*client = NULL;

	client = *it;
	if (client->response.res != "")
		std::cout << ft_getDate() << "\tReponse:\n" << CYAN\
		<< client->response.res << NC << std::endl;
	size = write(client->fd, client->response.res.c_str(), client->response.res.size());
	client->response.clear();
//	client->etFdSets(false, 1);
	/*	if (size < client->response.size())
			client->response = client->response.substr(size);
		else
		{
			client->response.clear();
			client->setToStandBy();
		}
		client->last_date = ft_getDate();
	}
	if (client->status == STANDBY)
	{
		if (getTimeDiff(client->last_date) >= TIMEOUT)
			client->status = DONE;
	}
	if (client->status == DONE)
	{
			delete client;
			Clients.erase(it);
			return (0);
	}
	else
		_Manage.dispatcher(*client);*/
	return (1);
}


Server::ServerFailure::ServerFailure(std::string Error)
{
	this->error = Error;
}

Server::ServerFailure::~ServerFailure(void) throw() { return ;}

const char			*Server::ServerFailure::what(void) const throw()
{
	return (this->error.c_str());
}
