/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:22:42 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/17 17:15:33 by thsembel         ###   ########.fr       */
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
		int			port;
		std::string		ip;
		std::string		Buf;
		struct Request	request;
		struct Response	response;
		struct t_chunk	chunk;;

		Client(int filed, fd_set *r, fd_set *w, struct sockaddr_in info);
		~Client(void);
		void	dispatcher(Client &client);
		void	setFdSets(bool set, int n);

	private:
		fd_set			*rSet;
		fd_set			*wSet;
};

#endif
