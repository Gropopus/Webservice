/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunkistan.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <thsembel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 23:05:45 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/09 17:47:00 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Client.hpp"

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

int				findLen(Client &client)
{
	std::string		to_convert;
	int				len;
	std::string		tmp;

	to_convert = client.Buf;
	if (to_convert.find("\r\n") != std::string::npos)
			to_convert = to_convert.substr(0, to_convert.find("\r\n"));
	while (to_convert[0] == '\n')
		to_convert.erase(to_convert.begin());
	if (to_convert.size() == 0)
		len = 0;
	else
		len = fromHexa(to_convert.c_str());
	len = fromHexa(to_convert.c_str());
	tmp = client.Buf;
	tmp = tmp.substr(tmp.find("\r\n") + 2);
	client.Buf = tmp;
	return (len);
}

void			fillBody(Client &client)
{
	std::string		tmp;

	tmp = client.Buf;
	if (tmp.size() > client.chunk.size)
	{
		client.chunk.body += tmp.substr(0, client.chunk.size);
		tmp = tmp.substr(client.chunk.size + 1);
		client.Buf.clear();
		client.Buf = tmp;
		client.chunk.size = 0;
	}
	else
	{
		client.chunk.body += tmp;
		client.chunk.size -= tmp.size();
		std::cout << RED << client.chunk.body << NC << std::endl;
		client.Buf.clear();
	}
}


/*void			getBody(Client &client, t_chunk &chunk, size_t *len)
{
	size_t	size;

	if (client.chunk.size == 0)
		client.chunk.size = *len;
	if (client.chunk.size < 0)
	{
		client.request.method = "BAD";
		return ;
	}
	size = client.Buf.size();
	std::cout << RED << client.chunk.size << std::endl;
	std::cout << BLUE << size << NC << std::endl;
	if (size >= client.chunk.size)
	{
		chunk.body += client.Buf;
		client.chunk.size = 0;
	}
	else
	{
		client.chunk.size -= size;
		chunk.body += client.Buf;
		client.Buf.clear();
	}
}*/
void	dechunk(Client &client)
{
	std::string	head = client.Buf.substr(0, client.Buf.find("\r\n\r\n"));
	std::string	chunks = client.Buf.substr(client.Buf.find("\r\n\r\n") + 4, client.Buf.size() - 1);
	std::string	subchunk = chunks.substr(0, 100);
	std::string	body = "";
	int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	size_t		i = 0;

	while (chunksize)
	{
		i = chunks.find("\r\n", i) + 2;
		body += chunks.substr(i, chunksize);
		i += chunksize + 2;
		subchunk = chunks.substr(i, 100);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}
	client.chunk.body += head + "\r\n\r\n" + body + "\r\n\r\n";
	client.request.body = client.chunk.body;
	std::cout << client.chunk.body << std::endl;
	client.chunk.is_chunk = false;
}
