/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetContent.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:54:56 by gmaris            #+#    #+#             */
/*   Updated: 2021/11/24 17:32:42 by gmaris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Web_serv.hpp"

static void	ft_gnl(string &buffer, string &line)
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

static string	_eraseWhiteSpace(int i, int to, string str)
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


int		getPair(std::string &line, std::string *key, std::string *value)
{
	int i = 0;
	int j = 0;

	while (line[i] && line[i] != ';' && line[i] != '{')
	{
		while (line[i] && !std::isspace(line[i]))
		{
			*key += line[i];
			i++;
		}
		while (std::isspace(line[i]))
			i++;
		while (line[i] && line[i] != ';' && line[i] != '{')
		{
			*value += line[i];
			i++;
		}
	}
	if (line[i] != ';' && line[i] != '{') // not the end of line or end of bracket content 
		return (-1);
	else
		j++;
	while (std::isspace(line[i + j]))
		j++;
	if (line[i + j])
		return (-1);
	return (i);
}


bool	getContent(std::string &buffer, std::string &context, std::string prec, Server &serv)
{

	std::string			line;
	std::string			key;
	std::string			value;
	int					pos = 0;

	prec.pop_back();
	while (prec.back() == ' ' || prec.back() == '\t')
		prec.pop_back();
	context += prec + "|";
	line = _eraseWhiteSpace(0, -1, line);
	while (line != "}" && !buffer.empty())
	{
		ft_gnl(buffer, line);
		line = _eraseWhiteSpace(0, -1, line);
		if (line[0] != '}')
		{
			if ((pos = getPair(line, &key, &value)) < 0)
				return (false);
			else if (line[pos] == '{')
				getContent(buffer, context, line, serv);
			else
			{
			//	std::cout << "key: " << key << "\nvalue: " << value << std::endl;
				std::pair<std::string, std::string>	tmp(key, value);
			//	std::cout << "context: " << context << std::endl;
		//		serv.config[context].insert(tmp);
				key.clear();
				value.clear();
			}
		}
		else if (line[0] == '}' && !buffer.empty())
		{
			pos = 0;
			line = _eraseWhiteSpace(pos++, -1, line);
			if (line[pos])
				return (false);
			context.pop_back();
			context = context.substr(0, context.find_last_of(':') + 1);
		}
	}
	return (true);
}