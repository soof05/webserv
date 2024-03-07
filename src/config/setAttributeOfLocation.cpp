#include "../supervisor/supervisor.hpp"

void	supervisor::collectMethods(std::vector<std::string> line, location& tmpLocation)
{
	std::vector<std::string> tmpVec;

	if (line.size() > 4 || line.size() == 1)
		throw Exception("Error You Are Only Allowed To Enter 3 Methods");
	std::vector<std::string>::iterator iter = line.begin() + 1;
	for (; iter != line.end(); iter++)
	{
		if (*iter == "GET")
			tmpVec.push_back(*iter);
		else if (*iter == "POST")
			tmpVec.push_back(*iter);
		else if (*iter == "DELETE")
			tmpVec.push_back(*iter);
		else
			throw Exception("Error " + *iter + " Is Not A Supported Method");
	}
	checkDoubleMethods(tmpVec);
	tmpLocation.setMethods(tmpVec);
}

int	checkIsADirectory(std::string path)
{
	const char* directory = path.c_str();
	// std::cout << "FULL : " << directory << "\n";
	DIR* directoryStream = opendir(directory);
	if (directoryStream != NULL)
	{
		closedir(directoryStream);
		return (1);
	}
	return (0);
}

void	supervisor::collectUploadPath(std::vector<std::string> line, location &tmpLocation)
{
	if (line.size() > 2  || line.size() == 1 || !checkIsADirectory(line[1]))
		throw Exception("Error: Upload Should Be As Follow\nupload /example");
	tmpLocation.setUpload(line[1]);
}

void	supervisor::collectRedirectionLink(std::vector<std::string> line, location &tmpLocation)
{
	if (line.size() > 2 || line.size() == 1)
		throw Exception("Error: Redirection Should Be As Follow\nredirection www.example.com");
	// std::cout << "we are in locations\n";
	tmpLocation.setRedirection(line[1]);
}

void	supervisor::endOfCollecting(std::vector<std::string> line, location &tmpLocation, server& server)
{
    (void)line;
	server.setLocations(tmpLocation);
	this->workingOnLocation = false;
    if (!tmpLocation.getRootCheck())
		throw Exception("Error You Forget To Set A Root Inside Location");
}

void    supervisor::setAutoIndex(std::vector<std::string> line, location &tmpLocation)
{
    if (line.size() > 2 || line.size() == 1)
        throw Exception("Errro: Autoindex Should Only Set To ON/OFF");
    if (line[1] == "ON")
        tmpLocation.setAutoindex(1);
    else if (line[1] == "OFF")
        tmpLocation.setAutoindex(0);
    else
        throw Exception("Error: Autoindex Should Only Set To ON/OFF");
}

void    supervisor::collectRoot(std::vector<std::string> line, location &tmpLocation)
{
    if (line.size() > 2 || line[1][0] != '/' || line.size() == 1)
        throw Exception("Error: Root Path Should Be As Follow\nroot /example");
    tmpLocation.setRoot(line[1]);
    tmpLocation.setRootCheck(true);
}

void    supervisor::setCgi(std::vector<std::string> line, location &tmpLocation)
{
    if (line.size() > 2 || line.size() == 1)
        throw Exception("Error: CGI Should Only Set To ON/OFF");
    if (line[1] == "ON")
        tmpLocation.setCGI(1);
    else if (line[1] == "OFF")
        tmpLocation.setCGI(0);
    else
        throw Exception("Error: CGI Should Only Set To ON/OFF");
}

void    supervisor::collectDefaultFile(std::vector<std::string> line, location &tmpLocation)
{
    if (line.size() > 2 || line.size() == 1)
        throw Exception("Error: Default File Should Be As Follow\ndefault example.example");
    tmpLocation.setDefaultFile(line[1]);
}
