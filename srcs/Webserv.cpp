/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:19:05 by gmaris            #+#    #+#             */
/*   Updated: 2021/12/17 12:31:27 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Env.hpp"
#include <stdlib.h>
#include <stdio.h>

//void destructor() attribute((destructor));

char **g_env;

void			printfHeader(void)
{
std::cout << CYAN << "\e[1m" << " __      __          __										\n\
/\\ \\  __/\\ \\        /\\ \\                                      \n\
\\ \\ \\/\\ \\ \\ \\     __\\ \\ \\____    ____     __   _ __   __  __  \n\
 \\ \\ \\ \\ \\ \\ \\  /'__`\\ \\ '__`\\  /',__\\  /'__`\\/\\`'__\\/\\ \\/\\ \\ \n\
  \\ \\ \\_/ \\_\\ \\/\\  __/\\ \\ \\L\\ \\/\\__, `\\/\\  __/\\ \\ \\/ \\ \\ \\_/ |\n\
   \\ `\\___x___/\\ \\____\\\\ \\_,__/\\/\\____/\\ \\____\\\\ \\_\\  \\ \\___/ \n\
    '\\/__//__/  \\/____/ \\/___/  \\/___/  \\/____/ \\/_/   \\/__/  \n\
                                                              \n";
std::cout << "By " << YELLOW << "\e[1m" << "thsembel & gmaris\n" << NC << std::endl;
}

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


void destructor() __attribute__((destructor));

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
		printfHeader();
		Env env(_getFile(av[1]));
		env.launchWebserv();
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << "Error: " << NC;
		std::cerr << e.what() << '\n';
		return 0;
	}
//	exit(EXIT_SUCCESS);
	return (0);
}

void destructor()
{
   // system("leaks webserv");
}
