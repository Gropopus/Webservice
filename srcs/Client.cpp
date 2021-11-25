/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:29:17 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/25 16:20:52 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
# include "Web_serv.hpp"
# include "Client.hpp"
# define BUFFER_SIZE 10000

Client::Client(int filed, fd_set *r, fd_set *w, struct sockaddr_in info)
: fd(filed), read_fd(-1), write_fd(-1), rSet(r), wSet(w)
{
	ip = inet_ntoa(info.sin_addr);
	port = htons(info.sin_port);
//	std::memset(rBuf, 0, BUFFER_SIZE);
	fcntl(fd, F_SETFL, O_NONBLOCK);
	FD_SET(fd, rSet);
	FD_SET(fd, wSet);
	std::cout << "\nnew connection from IP\t\t[" << GREEN << ip << ":" <<  port << NC;
	std::cout << "]" << std::endl;
}

Client::~Client(void)
{
	free(rBuf);
	rBuf = NULL;
	if (fd != -1)
	{
		close(fd);
		FD_CLR(fd, rSet);
		FD_CLR(fd, wSet);
	}
	if (read_fd != -1)
	{
		close(read_fd);
		FD_CLR(read_fd, rSet);
	}
	if (write_fd != -1)
	{
		close(write_fd);
		FD_CLR(write_fd, wSet);
	}
	std::cout << "\nconnection closed from IP\t[" << GREEN << ip << ":" <<  port << NC;
	std::cout << "]" << std::endl;
}

void	Client::setFdSets(bool set, int n)
{
	if (n == 0)
	{
		if (set)
			FD_SET(fd, rSet);
		else
			FD_CLR(fd, rSet);
	}
	else if (n == 1)
	{
		if (set)
			FD_SET(fd, wSet);
		else
			FD_CLR(fd, wSet);
	}
	else if (n == 2)
	{
		if (set)
			FD_SET(read_fd, rSet);
		else
			FD_CLR(read_fd, rSet);
	}
	else if (n == 3)
	{
		if (set)
			FD_SET(write_fd, wSet);
		else
			FD_CLR(write_fd, wSet);
	}
}
