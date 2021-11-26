/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 12:55:32 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/26 11:43:31 by thsembel         ###   ########.fr       */
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

string	eraseWhiteSpace(int i, int to, string str)
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
