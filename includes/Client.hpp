/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:22:42 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/07 23:04:08 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
# include "Web_serv.hpp"

class Client
{
	public:
		int				fd;
		int				read_fd;
		int				write_fd;
		std::string		Buf;
		struct Request	request;
		struct Response	response;
		struct t_chunk 	chunk;;

		Client(int filed, fd_set *r, fd_set *w, struct sockaddr_in info);
		~Client(void);
		//		void	readFile(void);
//		void	writeFile(void);
		void	dispatcher(Client &client);
		void	setFdSets(bool set, int n);
//		void	setToStandBy(void);

		std::string		ip;
		int			port;
	private:
//		int			status;
//		int			cgi_pid;
//		int			tmp_fd;
		fd_set			*rSet;
		fd_set			*wSet;
//		std::string	last_date;
//		std::string	response;
//		t_conf 		conf;
//		t_chunk		chunk;

};

#endif
