#include "multiplexing.hpp"

multiplexing::multiplexing()
{
}

void	multiplexing::startingMultiplexing(std::vector<server>& servers)
{
	for (size_t i = 0; i < servers.size(); i++)
    {
		setUpForGetAddrInfo(servers, i);
		searchingForTheOneInTheLinkedList();
		ourServers.insert(std::pair<int, server>(this->sock, servers[i]));
	}
	ClearSets();
	initialMaxFds();
	
	//enter the infinite loop
	InsideLoopInfini();
}

void    multiplexing::setUpForGetAddrInfo(std::vector<server>& servers, int i)
{
    memset(&this->hints, 0, sizeof (this->hints)); // make sure the struct is empty
    this->hints.ai_family = AF_INET;     // set it to IPv4
    this->hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    this->hints.ai_flags = AI_PASSIVE;     // fill in my IP for me // Passive socket is the socket that we use in the server
    std::string host = servers[i].getHost();
    std::string port = std::to_string(servers[i].getPort());
	std::cout << "PORT ID : " << port << " HOST is : " << host << std::endl;
    if (getaddrinfo(host.c_str() ,port.c_str() ,&this->hints ,&this->result) == -1)
		throw Exception ("Error : getaddrinfo failed");
}

void	multiplexing::searchingForTheOneInTheLinkedList()
{
	int yes = 1;  // for setsockopt() SO_REUSEADDR, below

	for(this->tmpResult = this->result; this->tmpResult != NULL; this->tmpResult = this->tmpResult->ai_next)
	{
		if ((this->sock = socket(this->tmpResult->ai_family, this->tmpResult->ai_socktype, this->tmpResult->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}
		// fcntl(this->sock, F_SETFL, O_NONBLOCK);
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			throw Exception ("setsockopt failed");
		if (bind(this->sock, tmpResult->ai_addr, tmpResult->ai_addrlen) == -1)
		{
			close(this->sock);
			throw Exception ("Error : server: bind Failed");
			continue;
		}
		socket_fd.push_back(this->sock);
		break;
	}
	// don't forget to free linked list
	freeaddrinfo(this->result);
	if (this->tmpResult == NULL) 
		throw Exception ("Failed to bind");
		//SOMAXCONN
	if (listen(this->sock, FD_SETSIZE) == -1)
		throw Exception ("Error : listen failed");
}

void	multiplexing::ClearSets()
{
	FD_ZERO(&this->masterRead);    // clear the master and temp sets
	FD_ZERO(&this->masterWrite);
	FD_ZERO(&this->readfds);
	FD_ZERO(&this->writefds);
}

void	multiplexing::initialMaxFds()
{
	std::vector<int>::iterator iter;

	for (iter = socket_fd.begin(); iter != socket_fd.end(); iter++)
		FD_SET(*iter, &this->masterRead);
	// keep track of the biggest file descriptor
	maxfds = *std::prev(iter);
}


void	multiplexing::InsideLoopInfini()
{
	signal(SIGPIPE, SIG_IGN);
    for (;;)
    {
		this->ft_select();
        // run through the existing connections looking for data to read or write
        for(this->currentFdToWorkWith = 0; \
		this->currentFdToWorkWith <= this->maxfds; \
		this->currentFdToWorkWith++)
        {
            if (FD_ISSET(this->currentFdToWorkWith, &this->readfds) \
			|| FD_ISSET(this->currentFdToWorkWith, &this->writefds)) // we got one!!
            {
				// std::cout << "\n------ IM important -----------\n|" << this->currentFdToWorkWith << "|\n------ IM important -----------\n";
				if (this->serveTheClient.count(this->currentFdToWorkWith) == 0)
					this->acceptConnection();
                else
                {
                    if (FD_ISSET(this->currentFdToWorkWith, &this->readfds))
						this->startWorkingOnRequest(findTheRightClient());
                    if (FD_ISSET(this->currentFdToWorkWith, &this->writefds))
						this->sendRespondToClient(findTheRightClient());
				}
            }
        }
    }
}

void	multiplexing::ft_select()
{
	this->readfds = this->masterRead;
	this->writefds = this->masterWrite;
	if (select(this->maxfds + 1, &this->readfds, &this->writefds, NULL, NULL) == -1)
		throw Exception ("select failed");
}

void	multiplexing::acceptConnection()
{
	this->newClient = accept(this->currentFdToWorkWith, NULL, NULL);
	// fcntl(this->newClient, F_SETFL, O_NONBLOCK);
	if (this->newClient == -1)
		throw Exception("Error : Accept Failed");
	FD_SET(this->newClient, &this->masterRead); //add client to the master set.
	if (this->newClient > this->maxfds)
		this->maxfds = this->newClient;
	setTheClientInMap();
}

void	multiplexing::setTheClientInMap()
{
	std::map<int, server>::iterator iter = this->ourServers.lower_bound(this->currentFdToWorkWith);
	client clientTmp;
	clientTmp.server = iter->second;
	clientTmp.stageForClient = 0;
	clientTmp.stageForRespond = 0;
	clientTmp.request.headerEnds = 0;
	clientTmp.request.endPost = 0;
	clientTmp.request.flagContentLength = false;
	clientTmp.request.flagTransferEncoding = false;
	
	this->serveTheClient.insert(std::pair<int, client>(this->newClient, clientTmp));
}

client&	multiplexing::findTheRightClient()
{
	std::map<int, client>::iterator mapIter = this->serveTheClient.lower_bound(this->currentFdToWorkWith);
	mapIter->second.clientFd = this->currentFdToWorkWith;
	// std::cout << "second fd : " << mapIter->second.clientFd << "\n";
	// std::cout << "HELLLO : " << mapIter->second.stageForClient << std::endl;
	return (mapIter->second);
}

void	multiplexing::startWorkingOnRequest(client &client)
{
	// client.stageForClient = 0;
	client.servingTheClient();

	// std::cout << "END : " << client.request.endPost << std::endl;
	// std::cout << "Mehosdkl : " << client.request.method <<std::endl;
	// if ((client.request.contentLenght - client.request.sizeReaded) < 0 && client.request.currentLenReaded < 1024)
	if (client.request.endPost == 1 || (client.request.headerEnds == 1 && (client.request.method == "GET" || client.request.method == "DELETE")))
	{
		client.stageForClient = 1;
		FD_CLR(this->currentFdToWorkWith, &this->masterRead);
		FD_SET(this->currentFdToWorkWith, &this->masterWrite);
	}
}

void	multiplexing::sendRespondToClient(client& client)
{
	if (client.stageForRespond != 2){
		client.servingTheClient();
	}
	if (client.stageForRespond == 2)
	{
		FD_CLR(this->currentFdToWorkWith, &this->masterWrite);
		close (this->currentFdToWorkWith);
		this->serveTheClient.erase(this->currentFdToWorkWith);
		std::remove("autoIndex.html");
	}
}

// void	multiplexing::sendRespondToClient(client& client)
// {
// 	std::cout << "READY TO WRITE\n";
// 	if (client.stageForRespond == 0){
// 		std::cout << "enter start respond from multiplexing\n";
// 		client.startRequest();
// 		client.stageForRespond = 1;
// 	}
// 	if (client.stageForRespond == 2)
// 	{
// 		std::cout << "end sending respond for client\n";
// 		FD_CLR(this->currentFdToWorkWith, &this->masterWrite);
// 		close (this->currentFdToWorkWith);
// 		this->serveTheClient.erase(this->currentFdToWorkWith);
// 	}
// }



/*
411 length required.

when testing a video in siege the Availability:		       93.70 %
and the Response time:		        7.96 secs

empty file now Availability:		       99.92 %
video file now Availability:		       100.00 %

don't forget to check for client when u read the request if he closed the connection 
just close fd_socket with him and erase him from the map
%20
GET jskdjfklsdj/ jsdflk 

*/