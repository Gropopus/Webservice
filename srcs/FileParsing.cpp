/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetContent.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:54:56 by gmaris            #+#    #+#             */
/*   Updated: 2021/11/25 16:08:34 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Web_serv.hpp"
#include "Env.hpp"
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

void	fill_conf(Server &serv, t_conf &conf, std::string &key, std::string &value)
{
	if (key == "index")
		conf.index = value;
	else if (key == "root")
		conf.root = value;
	else if (key == "methods")
		conf.methods = value;
	else if (key == "max_body")
		conf.max_body = ft_stoi(value);
	else if (key == "CGI")
		conf.cgi = value;
	else if (key == "exec")
		conf.exec = value;
	else if (key == "auth")
		conf.auth = value;
	else if (key == "php")
		conf.php = value;
	else if (key == "listen")
		serv._Port = ft_stoi(value);
	else if (key == "error")
		serv._Error = value;
	else if (key == "server_name")
		serv._Name = value;
}

bool	Env::getContent(std::string &buffer, std::string prec, Server &serv, t_conf &tmp_conf, int *i)
{
	std::string			line;
	std::string			key;
	std::string			value;
	int					pos = 0;

	prec.pop_back();
	while (prec.back() == ' ' || prec.back() == '\t')
		prec.pop_back();
	if (prec.find("location") != std::string::npos)
	{
		if (*i > 0)
		{
			serv.config.push_back(tmp_conf);
			tmp_conf.clear();
		}
		value = prec.substr(9);
		tmp_conf.location = value;
		value.clear();
		*i+=1;
	}
	line = eraseWhiteSpace(0, -1, line);
	while (line != "}" && !buffer.empty())
	{
		ft_gnl(buffer, line);
		line = eraseWhiteSpace(0, -1, line);
		if (line[0] != '}')
		{
			if ((pos = getPair(line, &key, &value)) < 0)
				return (false);
			else if (line[pos] == '{')
				getContent(buffer, line, serv, tmp_conf, i);
			else
			{
				fill_conf(serv, tmp_conf, key, value);
				key.clear();
				value.clear();
			}
		}
		else if (line[0] == '}' && !buffer.empty())
		{
			pos = 0;
			line = eraseWhiteSpace(pos++, -1, line);
			if (line[pos])
				return (false);
		}
	}
	return (true);
}
