#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include "../../config/config.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <sstream>
#include <algorithm>
#include <sys/stat.h>
#include <iomanip>

class request
{
    // private:
	public:
		location					locationWorkWith;
        std::ofstream				filePost;
        std::string					fileName;
		std::string					postFileName;
		char						**envp;
		char						**av;
		server						server;
        long						sizeReaded;
		int							clientFd;
        std::vector<std::string>	headerLines;
		size_t						headerSize;
        int 						currentLenReaded;
		std::string					fullPath;
		std::string					pathForIndex;
		std::string					scriptExtension;
		int							headerEnds;
		int							endPost;
		std::string					method;
		std::string					requestedPath;
        long						contentLenght;
		std::string					transferEncoding;
		std::string					contentType;
		bool						flagContentLength;
		bool						flagTransferEncoding;
		std::string					redirection;
		std::string					content;
		std::string					headers;
		std::string					extention;
		std::string					queryString;
		std::string					interpreterPath;
		std::string					outputFile;
		bool						CGI;
		std::string					cookies;

    public:
        request();
        request(const request &obj);
        request& operator=(const request &obj);

		void			separatePathFromQuery(std::string URL);
        void   	 		GetMethod(std::string path);
		void			DeleteMethod();
		void			PostMethod(std::string path);
        void			startRequest();
        void			setHeadersInVector();
        void			parsRequest();
        void			stringOfheaders();
		void			matchLocation(std::string path);
		void			joinLocationWithRoot(std::string path, std::string root);
		int				checkIsADirectory();
		void			serveFile();
		void			serveDirectory();
		void			handleGetRequest();
		void			convertHex(std::string& path);
		static bool		compareLength(location& a, location& b);
		void			sortLocationsVector(std::vector<location>& locations);
		void			genAutoIndex(std::ofstream& output);
		void			errorPages(int errorCode);
		void			goToClient(std::string file, std::string num);
		void			checkHeadersErrors();
		void			CheckLineRequest();
		void    		collectHeaderData(std::string& line, std::string stringToFind);
		void			ExtentionType();
		void			methodAllowd(std::string method);
		void			findTheLocation(std::string path);
		void			ifDefaultFileSpecified();
		void			ifNoDefaultFileSpecified();
		void			generateAutoIndex();
		void			performDeleteOnFile();
		void			createTheUploadFile();
		void			noChunckedRequest();
		void			chunckedRequest();
		void			closeConnection();
		void			ft_read();
		void			cgiHandler();
		void			setInterpreterPath();
		void			waitForChildProcess(int pid);
		void			prepareEnv();
		void			SetUpInputOutputFiles();
		std::string 	checkIfRedirect();
		// ~request();

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
