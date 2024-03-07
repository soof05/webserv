#include "../supervisor/supervisor.hpp"

void    supervisor::readAndParseConfigFile(char *fileName)
{
	std::vector<std::vector<std::string> > serversLines;

	openConfigFile(fileName);
	readAndFillServersLines(serversLines);
	for (std::vector<std::vector<std::string> >::iterator it = serversLines.begin(); it != serversLines.end(); it++)
		fillDataOfClassServer(*it);
	this->configFile.close();
}

/*this function use vector of vector string to read and set up lines 
from configfile for each server in vector string.
this function also this the syntax error of our configfile like (closed brackets 
and starting server with server{)
*/

void	supervisor::readAndFillServersLines(std::vector<std::vector<std::string> >& serversLines)
{
	std::string line;
	while (std::getline(this->configFile, line))
	{
		std::vector<std::string> serverConfig;
		if (line.empty() || line.find_first_not_of("\t") == std::string::npos)//skip line fill with tap or space.
			continue;
		if (line != "server{")
			throw Exception("Server{ not found");
		if (line == "server{")
		{
			serverConfig.push_back(line + "\n");
			while (std::getline(this->configFile, line))
			{
				if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)//line fill with spaces.
					continue;
				serverConfig.push_back(line + "\n");
				if (line[0] == '}' && line.size() == 1)
					break ;
				if (line == "server{")
					throw Exception("Error Server Opened And Not Closed");
			}
			serversLines.push_back(serverConfig);
		}
		if (line[0] != '}')
			throw Exception("Error Server Opened And Not Closed");
	}
}

void	supervisor::fillDataOfClassServer(std::vector<std::string> dataServer)
{
	std::map<int, std::string>	tmpErrorPages;
	server server;
	location	tmpLocation;

	this->workingOnLocation = false;
	for (std::vector<std::string>::iterator iter = dataServer.begin(); iter != dataServer.end(); iter++)
		fillDataOfEachServer(*iter, server, tmpErrorPages, tmpLocation);
	if (!server.gethostFlag() || !server.getPortFlag() || !server.getlocationFlag())
		throw Exception("Error You should Enter The Three Main Thing Port, Host, Location");
	server.setErrorPages(tmpErrorPages);
	if (!server.getServerBeenClosed())
		throw Exception("Error You Did Not Closed location\n");
	servers.push_back(server);
}

void	supervisor::fillDataOfEachServer(std::string& lineFromServer, server& server, std::map<int, std::string> &tmpErrorPages, location &tmpLocation)
{
	std::vector<std::string> lineSplited;
	splitedLine(lineFromServer, lineSplited);

	if (lineSplited[0] != "location" && !this->workingOnLocation)
		collectDataOutSideALocation(lineFromServer, server, tmpErrorPages);
	else
		collectLocationsData(lineFromServer, server, tmpLocation);
}

