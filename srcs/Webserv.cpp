/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:19:05 by gmaris            #+#    #+#             */
/*   Updated: 2021/12/13 12:18:42 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Env.hpp"

char **g_env;

static bool		_isConf(char *file)
{
	int len = std::strlen(file);
	int i;
	if (len < 5)
		return (false);
	i = len - 5;
	if (file[i] == '.' && file[i + 1] == 'c' && file[i + 2] == 'o'
		&& file[i + 3] == 'n' && file[i + 4] == 'f' && file[i + 5] == '\0')
		return (true);
	return (false);
}

static string	_getFile(char *file)
{
	string				content;
	std::stringstream	buffer;

	if (_isConf(file) == false)
		throw (Env::InvalidFile());
	std::ifstream	infile(file);
	if (infile.fail())
		throw (Env::InvalidFile());
	buffer << infile.rdbuf();
	if (buffer.fail())
		throw (Env::InvalidFile());
	infile.close();
	content = buffer.str();
	buffer.clear();
	if(content.empty())
		throw(Env::InvalidFile());
	return content;
}

int		main(int ac, char **av, char **env)
{
	g_env = env;
	if (ac != 2)
	{
		std::cout << RED << "Error: " << NC << "Bad arguments.\n";
		std::cout << YELLOW << "Usage: " << NC << "./Webserv [Config File]\n";
		return (1);
	}
	try
	{
		Env env(_getFile(av[1]));
		env.launchWebserv();
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << "Error: " << NC;
		std::cerr << e.what() << '\n';
		return 0;
	}
	exit(EXIT_SUCCESS);
	return (0);
}
