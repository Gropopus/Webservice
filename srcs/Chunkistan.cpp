/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunkistan.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 23:05:45 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/17 17:27:52 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Client.hpp"
# include <fstream>

int		ft_getpower(int nb, int power)
{
	if (power < 0)
		return (0);
	if (power == 0)
		return (1);
	return (nb * ft_getpower(nb, power - 1));
}

int			fromHexa(const char *nb)
{
	char	base[17] = "0123456789abcdef";
	char	base2[17] = "0123456789ABCDEF";
	int		result = 0;
	int		i;
	int		index;

	i = 0;
	while (nb[i])
	{
		int j = 0;
		while (base[j])
		{
			if (nb[i] == base[j])
			{
				index = j;
				break ;
			}
			j++;
		}
		if (j == 16)
		{
			j = 0;
			while (base2[j])
			{
				if (nb[i] == base2[j])
				{
					index = j;
					break ;
				}
				j++;
			}
		}
		result += index * ft_getpower(16, (strlen(nb) - 1) - i);
		i++;
	}
	return (result);
}

bool ft_ishexa(std::string str)
{
	int i = 0;
	int j = 0;
	bool ok = false;
	char base[17] = "0123456789abcdef";
	char base2[17] = "0123456789ABCDEF";
	while (str[i])
	{
			j = 0;
			while (base[j] && base2[j])
			{
				if ((str[i] == base[j] || str[i] == base2[j]) || str[i] == '\n'
					|| str[i] == '\r')
					ok = true;
				j++;
			}
			if (ok == false)
				return (false);
			ok = false;
		i++;
	}
	return (true);
}

void	get_len(Client &client)
{
	int			len = 0;
	std::string	buf = client.Buf;
	std::string	line;

	while (!buf.empty())
	{
		ft_gnl(buf, line, '\n');
		if (ft_ishexa(line) == true)
		{
			line = line.substr(0, line.find("\r"));
			len = fromHexa(line.c_str());
			client.chunk.size += len;
		}
	}
}

void	dechunk(Client &client)
{
	std::string buf = client.Buf;
	std::string len_s;
	std::string	tmp;

	if (client.chunk.size == 0)
	{
		len_s = buf.substr(0, buf.find("\r\n"));
		if (buf.find("\r\n") != buf.npos)
			buf = buf.substr(buf.find("\r\n") + 2); 
		else
			buf = "";
		client.chunk.size = fromHexa(len_s.c_str());
	}
	tmp = buf.substr(0, client.chunk.size);
	client.chunk.size -= tmp.size();
	client.chunk.body += tmp;
	
	std::string asap;
	asap = buf.substr(tmp.size());
	while (asap != "")
	{
		asap.erase(0, 2);
		len_s = asap.substr(0, asap.find("\r\n"));
		if (asap.find("\r\n") != asap.npos)
			buf = asap.substr(asap.find("\r\n") + 2);
		else
			buf = "";
		client.chunk.size = fromHexa(len_s.c_str());
		tmp = buf.substr(0, client.chunk.size);
		client.chunk.size -= tmp.size();
		client.chunk.body += tmp;
		if (tmp != "")
			asap = buf.substr(tmp.size());
		else
			asap.clear();
	}
	if (client.chunk.size == 0)
	{
		client.chunk.is_chunk = false;
		client.request.body += client.chunk.body;
		client.chunk.body.clear();

		std::ostringstream os;
		os << client.request.body.size();
		std::string str(os.str());
		client.request.headers["Content-Length"] = str;
	}
}
