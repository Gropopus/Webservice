/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 12:55:32 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/25 13:13:09 by thsembel         ###   ########.fr       */
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

void	ft_gnl(string &buffer, string &line)
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
