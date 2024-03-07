#include "../supervisor/supervisor.hpp"

//this function open configfile and check if it opened properly
void	supervisor::openConfigFile(char *fileName)
{
	this->configFile.open(fileName, std::ios::in);
	if (!this->configFile)
		throw Exception("Error At Opening file");
	if (this->configFile.eof())
		throw Exception("Error At Opening file");
}

int	supervisor::ConvertToInt(const std::string &out)
{
	char *integer;
	int value = strtol(out.c_str(), &integer, 10);
	if (integer == out.c_str() || *integer != '\0')
		throw Exception("Please Enter Data Type Number In This Place");
	return (value);
}

long	supervisor::ConvertToLong(const std::string &out)
{
	char *integer;
	long value = strtol(out.c_str(), &integer, 10);
	if (integer == out.c_str() || *integer != '\0')
		throw Exception("Please Enter Data Type Number In This Place");
	return (value);
}

void	supervisor::CheckHtmlExtention(const std::string &fileNameOfErrorPage)
{
	std::string extention(".html");
	if (fileNameOfErrorPage.length() < extention.length() || fileNameOfErrorPage.compare(fileNameOfErrorPage.length() - extention.length(), extention.length(), extention) != 0)
		throw Exception("one of the Error pages  has not a .html extention at the end\n");
}

void	supervisor::initEnumMap(std::map<std::string, int>& map)
{
	map.insert(std::make_pair("allow_methods", 0));
	map.insert(std::make_pair("root", 1));
	map.insert(std::make_pair("default", 2));
	map.insert(std::make_pair("CGI", 3));
	map.insert(std::make_pair("autoindex", 4));
	map.insert(std::make_pair("upload", 5));
	map.insert(std::make_pair("redirection", 6));
	map.insert(std::make_pair("}", 7));
}

void	supervisor::checkDoubleMethods(std::vector<std::string>& vec)
{
	std::vector<std::string>::iterator iter = vec.begin();

	for (iter = std::next(iter); iter != vec.end(); iter++)
		if (*iter == *(std::prev(iter)))
			throw Exception("You Can't Enter A Method Twice");
}

int		supervisor::idenitfyInput(std::map<std::string, int>& map, std::string key)
{
	std::map<std::string, int>::iterator input;
	if (map.count(key) > 0)
		input = map.lower_bound(key);
	else
		throw Exception("Error " + key);//change the error msg
	return (input->second);
}

void splitedLine(std::string const &line, std::vector<std::string> &out) 
{
    // construct a stream from the string 
    std::stringstream ss(line);
 
    std::string substring;
    while (ss >> substring)
        out.push_back(substring);
}

void	initEnumMapOutLocation(std::map<std::string, int>& map)
{
    map.insert(std::make_pair("server{", 0));
	map.insert(std::make_pair("port", 1));
	map.insert(std::make_pair("server_name", 2));
	map.insert(std::make_pair("host", 3));
	map.insert(std::make_pair("error_page", 4));
	map.insert(std::make_pair("limitBodySize", 5));
    map.insert(std::make_pair("}", 6));
}

void	supervisor::checkErrorsOutLocation(const std::vector<std::string>& lineSplited)
{
	if (lineSplited[0] == "error_page" && lineSplited.size() != 3)
		throw Exception("In Error Page You Should Enter Three Parameters");
	if (lineSplited[0] == "error_page")
		CheckHtmlExtention(lineSplited[2]);
	if (lineSplited[0] != "server{" && lineSplited[0] != "error_page" && lineSplited[0] != "}" && lineSplited[0] != "location" && lineSplited.size() != 2)
		throw Exception("You Should Enter Just Two Parameters");
}

// function write the data collected from configfile.
void supervisor::getServersData()
{
	std::vector<server>::iterator iter = this->servers.begin();
	for (; iter != this->servers.end(); iter++)
	{
		std::cout << "\n";
		std::cout << "size of locations is " << iter->getLocations().size() << std::endl;
		std::cout << "Port:             " << iter->getPort() << std::endl;
		std::cout << "Host:             " << iter->getHost() << std::endl;
		std::cout << "Client Body Size: " << iter->getClientBodySize() << std::endl;
		std::cout << "Server Name:      " << iter->getServerName() << std::endl;
		std::cout << "Error Pages:      ";
		for (std::map<int, std::string>::iterator itr = iter->getErrorPages().begin(); itr != iter->getErrorPages().end(); itr++)
			std::cout << itr->first << " " << itr->second << std::endl;
		std::cout << "\n";
		for (std::vector<location>::iterator it = iter->getLocations().begin(); it != iter->getLocations().end() ;it++)
		{
			std::cout << "Path Location: " << it->getPathOfLocation() << std::endl;
			std::cout << "Root:          " << it->getRoot() << std::endl;
			std::cout << "CGI:           " << it->getCGI() << std::endl;
			std::cout << "AutoIndex:     " << it->getAutoindex() << std::endl;
			std::cout << "Default File:  " << it->getDefaultFile() << std::endl;
			std::cout << "Upload:        " << it->getUpload() << std::endl;
			std::cout << "Redirection:   " << it->getRedirection() << std::endl;
			std::cout << "Methods    :   ";
			for (std::vector<std::string>::iterator i = it->getMethods().begin(); i != it->getMethods().end(); i++)
			{
				std::cout << *i << " ";
			}
			std::cout << "\n";
		}
	}
}

/*
changes
*/