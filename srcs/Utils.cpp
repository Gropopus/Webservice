/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 12:55:32 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/02 13:45:37 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"

int		ft_stoi(std::string str)
{
	int i = 0;
	int ret = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9' || str.length() > 10)
			return (-1);
		i++;
	}
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		ret = ret * 10 + str[i] - 48;
		i++;
	}
	return (ret);
}

void	ft_gnl(std::string &buffer, std::string &line, char to)
{
	size_t	i;

	i = buffer.find(to);
	if (i != std::string::npos)
	{
		line = std::string (buffer, 0, i++);
		buffer = buffer.substr(i);
	}
	else
	{
		if (buffer[buffer.size() - 1] == to)
			buffer = buffer.substr(buffer.size());
		else
		{
			line = buffer;
			buffer = buffer.substr(buffer.size());
		}
	}
}

std::string	eraseWhiteSpace(int i, int to, std::string str)
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

std::string		ft_getDate()
{
	struct timeval	time;
	struct tm		*tm;
	char			buffer[BUFFER_SIZE + 1];
	int				ret;

	std::memset(buffer, 0, BUFFER_SIZE + 1);
	gettimeofday(&time, NULL);
	tm = localtime(&time.tv_sec);
	ret = strftime(buffer, BUFFER_SIZE, "%a, %d %b %Y %T %Z", tm);
	return (buffer);
}

int		isFileDir(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFREG)
			return (1);
		if (s.st_mode & S_IFDIR)
			return (2);
		else
			return 0;
	}
	return (-1);
}

bool	is_only(std::string str)
{
	int i = 0;

	while (str[i])
	{
		if (str[i] != '/')
			return (false);
		i++;
	}
	return (true);
}
