#ifndef Server_HPP
#define Server_HPP

#include "Web_serv.hpp"
# include <iostream>
# include <vector>
# include <queue>
# include "Client.hpp"
typedef std::string string;

#define TIMEOUT 10
#define RETRY	"25"


struct	t_conf
{
	std::string location;
	std::string index;
	std::string root;
	std::string methods;
	int			port;
	int			max_body;
	std::string cgi;
	std::string exec;
	std::string auth;
	std::string php;

	void	clear(void)
	{
		index.clear();
		root.clear();
		methods.clear();
		max_body = 0;
		cgi.clear();
		exec.clear();
		auth.clear();
		php.clear();
		port = 0;
	}
};

class Client;
class Server
{
	//friend class Env;
	public:
		std::vector<Client*>	Clients;
		std::queue<int>			_tmp_clients;
		std::vector<t_conf>		config;
		int						_Port;
		std::string				_Error;
		std::string				_Name;
		struct sockaddr_in		_Info;

		Server(void);
		~Server(void);
		int		getMaxFd(void);
		int		getFd(void) const;
		int		getOpenFd(void);
		int		readRequest(std::vector<Client*>::iterator it);
		void	init(fd_set *readSet, fd_set *writeSet, fd_set *rSet, fd_set *wSet);
		void	acceptConnection(void);
		void	ParseRequest(Client &client);
		class		ServerFailure: public std::exception
		{
			public:
				std::string error;
				ServerFailure(std::string Error);
				virtual	~ServerFailure(void) throw();
				virtual const char		*what(void) const throw();
		};

	private:
		int					_Fd;
		int					_MaxFd;
		fd_set					*_ReadSet;
		fd_set					*_WriteSet;
		fd_set					*_RSet;
		fd_set					*_WSet;

};

#endif
