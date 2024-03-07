/*This file contain the class supervisor and all his attributes and member functions
*This file will be responsible for combining between all the units
 in our webserv (multiplexing and serving a clien).
*/

#pragma once

// include all the header file of all the steps in our code (connection, and each client)
#include "../config/config.hpp"
#include "../multiplexing/multiplexing.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

//this class will work like the manager of our webserv
class multiplexing;
class server;
class location;

class supervisor
{
    private:
        std::vector<server> servers;
        std::fstream        configFile;
		
		
		// this variable is to know whether we finish collecting location data or not
		bool				workingOnLocation;


    public:
        //---------------- member functions : -----------------
        //default constructor 
		supervisor(char *FileName, int numArgs);

		void	beginOurWebServ();
		//read the configfile.
        void    readAndParseConfigFile(char *fileName);
		//in this function we read configfile and put it in a (std::vector<std::vector<std::string> >&)
		void	readAndFillServersLines(std::vector<std::vector<std::string> >& serversLines);
		//write something
		void	fillDataOfClassServer(std::vector<std::string> dataServer);
		//write something 
		void	fillDataOfEachServer(std::string& lineFromServer, server& server, std::map<int, std::string> &tmpErrorPages, location &tmpLocation);

		// void	fillDataOfClassServerExceptLocation(std::string& lineFromServer, server& server, std::map<int, std::string> &tmpErrorPages);
		void	collectDataOutSideALocation(std::string& lineFromServer, server& server, std::map<int, std::string> &tmpErrorPages);

		// This function is built to collect data of a single location or multiple locations inside each server
		void	collectLocationsData(std::string& lineFromServer, server& server, location &tmpLocation);

		// This function is built inside the above, it checks if we start collecting  data from a single location, if yes it collects it
		void	collectDataInsideALocation(std::string& lineFromServer, server& server, location& tmpLocation);

		//-------------- set attributes of class server and location from the data inside configfile -----------------
		void	collectPort(server &server, std::string& port);
		void	collectHost(server &server, std::string& host);
		void	collectMethods(std::vector<std::string> line, location& tmpLocation);
		void	collectUploadPath(std::vector<std::string> line, location &tmpLocation);
		void	collectRedirectionLink(std::vector<std::string> line, location &tmpLocation);
		void	endOfCollecting(std::vector<std::string> line, location &tmpLocation, server& server);
		void	setAutoIndex(std::vector<std::string> line, location &tmpLocation);
		void	collectRoot(std::vector<std::string> line, location &tmpLocation);
		void	setCgi(std::vector<std::string> line, location &tmpLocation);
		void	collectDefaultFile(std::vector<std::string> line, location &tmpLocation);

		//---------------- TOOLS --------------
		
		//this function open configfile
		void	openConfigFile(char *fileName);
		
		//function to convert from string to int or long.
		int		ConvertToInt(const std::string &out);
		long	ConvertToLong(const std::string &out);
		
		//function to check if a file end up with (.html).
		void	CheckHtmlExtention(const std::string &fileNameOfErrorPage);

		//this function initial a map with data that needs to be collected inside location.
		void	initEnumMap(std::map<std::string, int>& map);

		//to check if someone enter a method to times in the same location.
		void	checkDoubleMethods(std::vector<std::string>& vec);

		//find the key if it's exist in this map.
		int		idenitfyInput(std::map<std::string, int>& map, std::string key);

		//check some errors outside location.
		void	checkErrorsOutLocation(const std::vector<std::string>& lineSplited);

		//function that write all the data from configfile
		void	getServersData();
		
		// Those function are buil
		//*********** Exception Class ************
		class Exception : public std::exception {
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