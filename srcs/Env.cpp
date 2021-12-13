/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 17:55:15 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/13 12:33:18 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Env.hpp"

bool g_exit = true;

bool			isdouble_serv(std::vector<Server> _Servers, Server tmp)
{
	std::vector<Server>::iterator it = _Servers.begin();
	while (it != _Servers.end())
	{
		if ((*it)._Port == tmp._Port)
			return (false);
		it++;
	}
	return (true);
}

void			Env::init_server_map(std::string buffer)
{
	int						i = 0;
	std::string				line;
	Server					tmp;
	t_conf					tmp_conf;

	tmp_conf.clear();
	while (!buffer.empty())
	{
		ft_gnl(buffer, line, '\n');
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
				if (getContent(buffer, line, tmp, tmp_conf, &i) == false)
					throw (Env::InvalidFile());
				if (isdouble_serv(_Servers, tmp) == true)
				{
					tmp.config.push_back(tmp_conf);
					_Servers.push_back(tmp);
				}
				i = 0;
				tmp_conf.clear();
				tmp.config.clear();
			}
			else
				throw(Env::InvalidFile());
		}
		else if (line[0])
			throw(Env::InvalidFile());
	}
	
}

Env::Env(string conf_file)
{
	init_server_map(conf_file);
	// debug()
	/*std::vector<Server>::iterator it = _Servers.begin();
	std::cout << _Servers.size() << std::endl;
	while (it != _Servers.end())
	{
		std::vector<t_conf>::iterator i = (*it).config.begin();
		std::cout << "Server port: " << (*it)._Port << std::endl;
		std::cout << "Server Name: " << (*it)._Name << std::endl;
		std::cout << "Server error pages :" << (*it)._Error << std::endl;
		std::cout << "==================================\n";
		while (i != (*it).config.end())
		{
			std::cout << "location: " << (*i).location << std::endl;
			std::cout << "index: " <<(*i).index << std::endl;
			std::cout << "root: " << (*i).root << std::endl;
			std::cout << "methods: " << (*i).methods << std::endl;
			std::cout << "max_body: " << (*i).max_body << std::endl;
			std::cout << "cgi: " << (*i).cgi << std::endl;
			std::cout << "exec: " << (*i).exec << std::endl;
			std::cout << "auth: " << (*i).auth << std::endl;
			std::cout << "php: " << (*i).php << std::endl;
			std::cout << "====== end of location struct\n";
			i++;
		}
		std::cout << "\n******* Fin du Server *********\n";
		it++;
	}*/
	return ;
}

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
 		(*it).init(&_ReadSet, &_WriteSet, &_RSet, &_WSet);
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
		it_client++;
	}
}

void	handleSignals(int sig)
{
	std::cout << "\b  ";
	std::cout << std::endl << RED << "Exiting..." << NC << std::endl;
	if (SIGINT == sig)
		g_exit = false;
}

void	Env::launchWebserv(void)
{
	int ret = 0;

	try
	{
		initFd();
	}
	catch (std::exception &e)
	{
		std::cerr << RED << "Error: " << NC << e.what() << std::endl;
		return ;
	}
	std::cout << GREEN << "Webserv ready !\n" << NC;
	while (g_exit == true)
	{
		_ReadSet = _RSet;
		_WriteSet = _WSet;
		if (ret == 0)
			std::cout << "\rWaiting for connection\t⏳" << std::flush;
		ret = select(getMaxFd() + 1, &_ReadSet, &_WriteSet, NULL, &_Timeout);
	for (std::vector<Server>::iterator server(_Servers.begin()); server != _Servers.end(); ++server)
		{
			handleRequests(server);
			if (FD_ISSET(server->getFd(), &_ReadSet) && g_exit == true)
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
			signal(SIGINT, handleSignals);
		//	if (!server->_tmp_clients.empty())
		//		if (FD_ISSET(server->_tmp_clients.front(), &_WriteSet))
				//erreur 503
		}
	}
	_Servers.clear();
	return ;
}
