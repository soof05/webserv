#include "config.hpp"

//default constructor
server::server()
{
	this->ClientBodySize = 50000;
	this->hostFlag = false;
	this->portFlag = false;
	this->locationFlag = false;
	this->serverBeenClosed = false;
}

server::server(const server& obj)
{
	*this = obj;
}
server& server::operator=(const server &obj)
{
	this->port = obj.port;
	this->ClientBodySize = obj.ClientBodySize;
	this->portFlag = obj.portFlag;
	this->hostFlag = obj.hostFlag;
	this->locationFlag = obj.locationFlag;
	this->serverBeenClosed = obj.serverBeenClosed;
	this->serverName = obj.serverName;
	this->host = obj.host;
	this->locations = obj.locations;
	this->errorPages = obj.errorPages;
	// this->loca = obj.loca;
	return (*this);
}

//getters
int							server::getPort()
{
    return (this->port);
}

std::string					server::getServerName()
{
    return (this->serverName);
}

std::string					server::getHost()
{
    return (this->host);
}

long						server::getClientBodySize()
{
    return (this->ClientBodySize);
}

std::map<int, std::string>&	server::getErrorPages()
{
    return (this->errorPages);
}

bool						server::getPortFlag()
{
	return (this->portFlag);
}

bool						server::gethostFlag()
{
	return (this->hostFlag);
}

bool						server::getlocationFlag()
{
	return (this->locationFlag);
}

std::vector<location>&		server::getLocations()
{
	return (this->locations);
}

bool						server::getServerBeenClosed()
{
	return (this->serverBeenClosed);
}

//setters
void	server::setPort(int port)
{
    this->port = port;
}

void	server::setServerName(const std::string& serverName)
{
	this->serverName = serverName;
}

void	server::setHost(const std::string& host)
{
	this->host = host;
}

void	server::setClientBodySize(long ClientBodySize)
{
	this->ClientBodySize = ClientBodySize;
}

void	server::setErrorPages(const std::map<int, std::string>& errorPages)
{
	this->errorPages = errorPages;
}

void	server::setPortFlag(bool portFlag)
{
	this->portFlag = portFlag;
}

void	server::sethostFlag(bool hostFlag)
{
	this->hostFlag = hostFlag;
}

void	server::setlocationFlag(bool locationFlag)
{
	this->locationFlag = locationFlag;
}

void	server::setServerBeenClosed(bool closedOrOpen)
{
	this->serverBeenClosed = closedOrOpen;
}

void	server::setLocations(const location& location)
{
	this->locations.push_back(location);
}

//default constructor of location

location::location()
{
	this->cgi = false;
	this->autoindex = false;
	this->rootCheck = false;
	this->methods.clear();
	this->defaultFile.clear();
	this->upload.clear();
	this->redirection.clear();
	this->root.clear();
	this->pathOfLocation.clear();
}

location::location(const location& obj)
{
	*this = obj;
}

location&	location::operator=(const location& obj)
{
	this->methods = obj.methods;
	this->root = obj.root;
	this->cgi = obj.cgi;
	this->autoindex = obj.autoindex;
	this->defaultFile = obj.defaultFile;
	this->upload = obj.upload;
	this->redirection = obj.redirection;
	this->pathOfLocation = obj.pathOfLocation;
	this->rootCheck = obj.rootCheck;
	return (*this);
}

// getters of class location
std::vector<std::string>&	location::getMethods()
{
	return (this->methods);
}

std::string&					location::getRoot()
{
	return (this->root);
}

bool						location::getCGI()
{
	return (this->cgi);
}

bool						location::getAutoindex()
{
	return (this->autoindex);
}

std::string& 				location::getDefaultFile()
{
	return (this->defaultFile);
}

std::string& 				location::getUpload()
{
	return (this->upload);
}

std::string& 				location::getRedirection()
{
	return (this->redirection);
}

std::string& 				location::getPathOfLocation()
{
	return (this->pathOfLocation);
}


// setters of class location 
void	location::setMethods(const std::vector<std::string>& methods)
{
	this->methods = methods;
}

void	location::setRoot(const std::string& Root)
{
	this->root = Root;
}

void	location::setCGI(bool cgi)
{
	this->cgi = cgi;
}

void	location::setAutoindex(bool autoindex)
{
	this->autoindex = autoindex;
}

void	location::setDefaultFile(const std::string& defaultFile)
{
	this->defaultFile = defaultFile;
}

void	location::setUpload(const std::string& upload)
{
	this->upload = upload;
}

void	location::setRedirection(const std::string& Redirection)
{
	this->redirection = Redirection;
}

void	location::setPathOfLocation(const std::string& pathOflocation)
{
	this->pathOfLocation = pathOflocation;
}

void	location::setRootCheck(bool root)
{
	this->rootCheck = root;
}

bool	location::getRootCheck()
{
	return (this->rootCheck);
}

// void	server::setLocation(const location& location)
// {
// 	this->loca = location;
// }

// location&	server::getLocation()
// {
// 	return (this->loca);
// }