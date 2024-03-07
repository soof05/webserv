#pragma once

#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>

class respond
{
    public:
		int				statusRespond;
		std::string		fileToGet;
		int				clientFd;
		int				fileFd;
		int				lenToSend;
		std::string		bufferSend;
		std::fstream	File;
		std::string		responseType;
		std::string		redirection;
		bool			CGI;
	public:
		respond();
		respond(const respond &obj);
        respond& operator=(const respond &obj);
		void			sendHeadersToClient();
		std::string		findLengthOfFile();
		std::string		findContentType();
		void			openFileToServe();
		void			initResponseType();
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