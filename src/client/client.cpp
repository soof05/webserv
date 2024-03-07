#include "client.hpp"

client::client()
{
    this->stageForClient = 0;
	this->stageForRespond = 0;
}

client::client(const client &obj)
{
    *this = obj;
}

client& client::operator=(const client &obj)
{
    this->clientFd = obj.clientFd;
    this->request = obj.request;
    this->server = obj.server;
    this->stageForClient = obj.stageForClient;
    this->stageForRespond = obj.stageForRespond;
    this->getFile = obj.getFile;
    return *this;
}

void	client::servingTheClient()
{
    //we need to set a flag to know what step are we in like 
    //if we are in paresing and reading a request the flag is 0 if we sending it flag is 1
    //if we done with the client we need to set a flag to drop this client.
	this->request.clientFd = this->clientFd;
	this->request.server = this->server;
    this->request.CGI = false;

    if (this->stageForClient == 0){
        try{
            this->request.startRequest();
        }
        catch (std::exception &excep){
            resultOfRequest(excep.what());
        }
    }
    if (this->stageForClient == 1){
        this->respond.statusRespond = this->responseStatus;
        this->respond.fileToGet = this->getFile;
        this->respond.clientFd = this->clientFd;
        this->respond.redirection = this->request.redirection;
        this->respond.CGI = this->request.CGI;
        this->startRespond();
    }
}

void    client::resultOfRequest(const char *whatString)
{
	this->responseStatus = atoi(whatString);
    this->getFile = this->request.fileName;
    // std::cout << "FILE : " << this->getFile << std::endl;
	this->stageForClient = 1;
}

void	client::startRespond()
{
    //set Headers to send them to client
    if (this->stageForRespond == 0)
    {
        this->respond.sendHeadersToClient();
        // std::cout << "Message sended : " << this->respond.bufferSend.c_str() << std::endl;
        send(this->clientFd, this->respond.bufferSend.c_str(), this->respond.lenToSend, 0);
        if (this->respond.fileFd < 0)
        {
            this->stageForRespond = 2;
            return ;
        }
        this->stageForRespond = 1;
        if (this->responseStatus == 301)
        {
            this->request.endPost = 1;            
            this->stageForRespond = 2;
            close (this->respond.fileFd);
        }
    }
    if (this->stageForRespond == 1){
        char buffer[1024];
        int r = read(this->respond.fileFd, buffer, 1024);
        // std::cout << "r IS : " << r << std::endl;
        send(this->clientFd, buffer, r, 0);
        if (r == 0)
        {
            this->stageForRespond = 2;
            close (this->respond.fileFd);
        }
    }
}

        // std::cout << "file fd : " << this->respond.fileFd << "\n";
        // std::cout << "|" << this->respond.bufferSend.c_str() << "|\n";
        // this->respond.bufferSend.clear();
        // std::fstream	File;
        // File.open(this->respond.fileToGet, std::ios::in);
        // if (!File)
        // {
        //     std::cout << "Error In Open File\n";
        //     exit(1);
        // }
        // std::getline(this->respond.File, this->respond.bufferSend);
        // std::cout << "\n--------line------\n" << this->respond.bufferSend << "\n--------line------\n";

        // write(this->respond.fileFd, l.c_str(), this->respond.bufferSend.size()+1);
                // if (this->respond.File.peek() == std::ifstream::traits_type::eof()){
        //     send(this->clientFd, buffer, 0, 0);
        //     this->respond.File.close();
        //     this->stageForRespond = 2;
        //     return ;
        // }
        // this->respond.File.read(buffer, 1024);
        // std::streampos fileSize = this->respond.File.tellg();
        // std::cout << "len readed is : " << r << "\n";
        // std::cout << "\n-------------------------\n" << buffer << "\n------------------------\n";
        // char buf[r+this->respond.lenToSend];
        // std::string buf = (char *)this->respond.bufferSend.c_str() + buffer;
        // write(this->clientFd, this->respond.bufferSend.c_str(), this->respond.lenToSend, 0);
        // std::string l = this->respond.bufferSend + "\n";
        // std::string se = buffer;
        // send(this->clientFd, l.c_str(), this->respond.bufferSend.size()+1, 0);
                // if (this->respond.File.eof())
                            // this->respond.File.close();
            // std::cout << "Close : " << this->respond.fileFd << "\n";