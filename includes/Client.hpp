/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:22:42 by thsembel          #+#    #+#             */
/*   Updated: 2021/11/25 17:07:28 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CLIENT_HPP
#define CLIENT_HPP
# include "Web_serv.hpp"

class Client
{
//	friend class Server;
//	friend class Manager;
//	friend class Helper;

/*	typedef std::map<std::string, std::string> t_conf;
	struct t_chunk
	{
		unsigned int	len;
		bool			done;
		bool			found;
	};*/

	public:	
		int			fd;
		int			read_fd;
		int			write_fd;
		std::string	Buf;
		Client(int filed, fd_set *r, fd_set *w, struct sockaddr_in info);
		~Client(void);
//		void	readFile(void);
//		void	writeFile(void);
		void	setFdSets(bool set, int n);
//		void	setToStandBy(void);

	private:
		int			port;
//		int			status;
//		int			cgi_pid;
//		int			tmp_fd;
		fd_set			*rSet;
		fd_set			*wSet;
//		struct Request	*req;
//		struct Response	*res;
		std::string		ip;
//		std::string	last_date;
//		std::string	response;
//		t_conf 		conf;
//		t_chunk		chunk;

};

#endif
