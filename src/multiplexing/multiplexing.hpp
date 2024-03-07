#pragma once

#include "../supervisor/supervisor.hpp"
#include "../client/client.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <vector>

class multiplexing
{
    private:
		std::vector<int>		socket_fd;//vector contain fds for our server.
		struct addrinfo			hints;
		struct addrinfo			*result;
		struct addrinfo			*tmpResult;
		int						sock;
		fd_set					masterWrite, masterRead, readfds, writefds;
		int						maxfds; //maximum file descriptor num.
		int						newClient; //newly accepted socket descriptor.    
		int						currentFdToWorkWith;//the current fd to work on
		std::map<int, client>	serveTheClient;//map contain the fd and the client
		std::map<int, server>	ourServers;//map contain the fd of server and the class server

	public:
				multiplexing();
		void	startingMultiplexing(std::vector<server>& servers);
		void	setUpForGetAddrInfo(std::vector<server>& servers, int i);
		void	searchingForTheOneInTheLinkedList();
		void	ClearSets();
		void	initialMaxFds();
        void	InsideLoopInfini();
		void	ft_select();
		void	acceptConnection();
		void	startWorkingOnRequest(client &client);
		void	sendRespondToClient(client &client);
		client&	findTheRightClient();
		void	setTheClientInMap();

		class Exception : public std::exception
		{
			private:
				std::string	errMsg;
			public:
				Exception(const std::string& msg)
				{
					this->errMsg = msg;
				}
				const char* what() const throw()
				{
					return (this->errMsg.c_str());
				}
				std::string	getMsg()
				{
					return (this->errMsg);	
				}
				virtual ~Exception () _NOEXCEPT {};
        };
};