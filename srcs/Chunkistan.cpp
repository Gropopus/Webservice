/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunkistan.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <thsembel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 23:05:45 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/08 16:02:56 by thsembel         ###   ########.fr       */
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
	to_convert = to_convert.substr(0, to_convert.find("\r\n"));
	while (to_convert[0] == '\n')
		to_convert.erase(to_convert.begin());
	if (to_convert.size() == 0)
		len = 0;
	else
		len = fromHexa(to_convert.c_str());

	len = fromHexa(to_convert.c_str());
	std::cout << "len=>" << len << "\tstr:" << to_convert << std::endl;
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
		client.chunk.is_chunk = false;
	}
	else
	{
		client.chunk.body += tmp;
		client.request.body += tmp;
		client.chunk.size -= tmp.size();
		client.Buf.clear();
	}
//	std::cout << RED << client.request.body << NC << std::endl;
}

void			getBody(Client &client)
{
	size_t	size;

	std::cout << "1->" << client.chunk.size << std::endl;
	if (client.chunk.size == 0)
		client.chunk.size = ft_stoi(client.request.headers["Content-Length"].c_str()); // atoi
	std::cout << "2->" << client.chunk.size << std::endl;
	if (client.chunk.size < 0)
	{
		client.request.method = "BAD";
		return ;
	}
	size = client.Buf.size();
	if (size >= client.chunk.size)
	{
		client.Buf.clear();
		client.chunk.body += client.Buf;
		client.request.body += client.Buf;
		client.chunk.size = 0;
	}
	else
	{
		client.chunk.size -= size;
		client.chunk.body += client.Buf;
		client.request.body += client.Buf;
		client.Buf.clear();
	}
}

void			dechunkBody(Client &client)
{
	if (std::strstr(client.Buf.c_str(), "\r\n"))
	{
		std::cout << "passe dans le false\n";
		client.chunk.size = findLen(client);
	//	std::cout << RED << client.chunk.size << std::endl;
		client.chunk.is_chunk = true;
		if (client.chunk.size == 0)
		{
			std::cout << CYAN << "iiiiiiiiiFINISH!!!!!!!!!!\n" << NC;
			client.chunk.finish = true;
		}
		else
			client.chunk.is_chunk = true;
	}
	//else if (client.chunk.is_chunk == true)
	//	fillBody(client);
	if (client.chunk.finish)
	{
		client.Buf.clear();
		client.chunk.is_chunk = false;
		client.chunk.finish = false;
		client.request.body = client.chunk.body;
		client.chunk.body.clear();
		return ;
	}
}

void			parseBody(Client &client)
{
	std::cout << "entre dans parsebody\n";
	if (client.request.headers.find("Content-Length") != client.request.headers.end())
	{
		std::cout << "entre dans getBody\n";
		getBody(client);
	}
	if (client.request.headers["Transfer-Encoding"] == "chunked")
	{
		std::cout << "passe au dechunkage\n";
		dechunkBody(client);
		//client.chunk.is_chunk = true;
	}
}

