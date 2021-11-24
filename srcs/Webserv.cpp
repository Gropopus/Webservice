/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:19:05 by gmaris            #+#    #+#             */
/*   Updated: 2021/11/24 15:35:26 by gmaris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Env.hpp"

int		main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << RED << "Error: " << NC << "Bad arguments.\n";
		std::cout << YELLOW << "Usage: " << NC << "./Webserv [Config File]\n";
		return (1);
	}
	(void)av;
	//Env env;
//	env.launchWebserv(av[1]);
	return (0);
}