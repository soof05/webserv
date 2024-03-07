#pragma once

#include <iostream>
#include <fstream>
#include "../config/config.hpp"
#include "./request/request.hpp"
#include "./respond/respond.hpp"
#include <sys/socket.h>

class request;
class client
{
    public:
        server	    server;
        request	    request;
        respond		respond;
		int		    clientFd;
        int         responseStatus;
        int         stageForClient;
        int			stageForRespond;
        std::string getFile;
	client();
    client(const client &obj);
    client& operator=(const client &obj);
    // void    setResponseStatus(int code);
    void    servingTheClient();
    void    resultOfRequest(const char *whatString);
    void    startRespond();
};