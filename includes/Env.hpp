/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 17:15:20 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/25 13:06:47 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef ENV_HPP
# define ENV_HPP
# include "Web_serv.hpp"
# include "Server.hpp"

# include <iostream>
# include <vector>
typedef std::string string;

class	Server;
struct t_conf;
class	 Env
{
	public:
	 	Env(string conf_file);
	 	~Env(void);
		bool createServer(string conf_file);
		bool	getContent(std::string &buffer, std::string prec, Server &serv, t_conf &tmp_conf, int *i);
		bool pushServersConf(Server tmp);
		void init_server_map(std::string buffer);
	 	int		getMaxFd(void);
	 	int		getOpenFd(void);
	 	void	initFd(void);
	 	void	handleRequests(std::vector<Server>::iterator server);
	 	void	launchWebserv(void);

		class InvalidFile: public std::exception
		{
			public:
				virtual const char *what(void) const throw()
				{
					return (" Invalid configuration file.");
				}
		};

	 private:

	 	fd_set					_ReadSet;
	 	fd_set					_WriteSet;
	 	fd_set					_RSet;
	 	fd_set					_WSet;
	 	struct timeval			_Timeout;
	 	std::vector<Server>		_Servers;
	 	// Client					*_Client;
};

# endif
