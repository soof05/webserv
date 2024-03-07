/*This file will contain the class config,
 will parse the config file and check errors, 
 take the parameters that we will need for connection and other steps.
*/

#pragma once

#include <iostream>
#include <map>
#include <vector>

//class of locations to each server

class location
{
	private:
		std::vector<std::string> methods;
		std::string root;
		bool cgi, autoindex;
		std::string defaultFile;
		std::string upload;
		std::string redirection;
		std::string pathOfLocation;
		bool		rootCheck;
	public:
		//default constructor set default for cgi and autoindex.
		location();
		location(const location& obj);
		location& operator=(const location& obj);
	// getters
		std::vector<std::string>&	getMethods();
		std::string					&getRoot();
		bool						getCGI();
		bool						getAutoindex();
		std::string					&getDefaultFile();
		std::string 				&getUpload();
		std::string 				&getRedirection();
		std::string 				&getPathOfLocation();
		bool						getRootCheck();
	// setters
		void	setMethods(const std::vector<std::string> &methods);
		void	setRoot(const std::string &Root);
		void	setCGI(bool cgi);
		void	setAutoindex(bool autoindex);
		void	setDefaultFile(const std::string &defaultFile);
		void	setUpload(const std::string &upload);
		void	setRedirection(const std::string &Redirection);
		void	setPathOfLocation(const std::string &pathOflocation);
		void	setRootCheck(bool root);
		void	clearAllFields();
};

//this class will handle the config file.
class server
{
	private:
		int 						port;
		long						ClientBodySize;
		bool						portFlag;
		bool						hostFlag;
		bool						locationFlag;
		bool						serverBeenClosed;
		std::string					serverName ,host;
		std::vector<location>		locations;
		std::map<int, std::string>	errorPages;
		// location					loca;
	public:
		//default constructor
		server();
		server(const server& obj);
		server& operator=(const server &obj);
		//getters
		int							getPort();
		std::string					getServerName();
		std::string					getHost();
		long						getClientBodySize();
		std::map<int, std::string>&	getErrorPages();
		std::vector<location>&		getLocations();
		bool						getPortFlag();
		bool						gethostFlag();
		bool						getlocationFlag();
		bool						getServerBeenClosed();
		// location&					getLocation();
		//setters
		void	setPort(int port);
		void	setServerName(const std::string& serverName);
		void	setHost(const std::string& Host);
		void	setClientBodySize(long ClientBodySize);
		void	setErrorPages(const std::map<int, std::string>& errorPages);
		void	setLocations(const location& location);
		void	setPortFlag(bool portFlag);
		void	sethostFlag(bool hostFlag);
		void	setlocationFlag(bool locationFlag);
		void	setServerBeenClosed(bool closedOrOpen);
		// void	setLocation(const location& locat);
};

//this function take a line and splited by white spaces and put it into a vector of string 
void	splitedLine(std::string const &line, std::vector<std::string> &out);
//this function initial a map for me to know 
//if there is any outside data does not need to be collected here.
void	initEnumMapOutLocation(std::map<std::string, int>& map);