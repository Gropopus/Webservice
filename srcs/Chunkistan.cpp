/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunkistan.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <thsembel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 23:05:45 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/10 14:43:13 by thsembel         ###   ########.fr       */
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
/*
 *  length := 0
     read chunk-size, chunk-ext (if any), and CRLF
     while (chunk-size > 0) {
        read chunk-data and CRLF
        append chunk-data to decoded-body
        length := length + chunk-size
        read chunk-size, chunk-ext (if any), and CRLF
     }
     read trailer field
     while (trailer field is not empty) {
        if (trailer field is allowed to be sent in a trailer) {
            append trailer field to existing header fields
        }
        read trailer-field
     }
     Content-Length := length
     Remove "chunked" from Transfer-Encoding
     Remove Trailer from existing header fields
}*/
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

void	printcode(std::string str)
{
	int i = 0;
	int nb = 0;
	while (str[i])
	{
		nb = str[i];
		std::cout << nb << "\t";
		i++;
	}
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
	std::string line;
	if (client.Buf.find("HTTP/1.1") != std::string::npos)
	{
		client.chunk.header = client.Buf;
		return ;
	}
	if (client.request.config.max_body >= 0)
	{
		if (client.Buf.size() > (size_t)client.request.config.max_body)
		{
			client.response.status_code = REQTOOLARGE;
			_construct_error(client.response, client.request);
			return ;
		}
	}
	get_len(client);
	if (buf != "0\r\n\r\n\0")
		while (!buf.empty())
		{
			ft_gnl(buf, line, '\n');
			if (ft_ishexa(line) == false)
			{
				client.chunk.body += line;
				if (buf == "0\r\n\r\n\0")
				{
					client.chunk.body.pop_back();
					client.chunk.body.pop_back();
					break;
				}
			}
		}
	if (client.chunk.body.size() == client.chunk.size)
	{
		client.chunk.is_chunk = false;
		client.response.body = client.chunk.body;
		std::cout << client.response.body << std::endl;
	}
}
/*void	dechunk(Client &client)
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
}*/
