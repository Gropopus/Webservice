#ifndef Server_HPP
#define Server_HPP

#include "Web_serv.hpp"
#define TIMEOUT 10
#define RETRY	"25"

class Server
{

	public:
		std::vector<Client*>	Clients;
		std::map<std::string, std::string>	content;
	
		Server(void);
		~Server(void);
		int		getMaxFd(void);
		int		getFd(void) const;
		int		getOpenFd(void);
		void	init(fd_set *readSet, fd_set *writeSet, fd_set *rSet, fd_set *wSet);
		void	acceptConnection(void);

		class		ServerFailure: public std::exception
		{
			public:
				std::string error;
				ServerFailure(std::string Error);
				virtual	~ServerFailure(void) throw();
				virtual const char		*what(void) const throw();
		};

	private:
		int						_Fd;
		int						_MaxFd;
		int						_Port;
		struct sockaddr_in		_Info;
		fd_set					*_ReadSet;
		fd_set					*_WriteSet;
		fd_set					*_RSet;
		fd_set					*_WSet;

};

#endif
