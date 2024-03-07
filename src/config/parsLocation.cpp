#include "../supervisor/supervisor.hpp"

void	supervisor::collectLocationsData(std::string& lineFromServer, server& server, location &tmpLocation)
{
	std::vector<std::string> lineSplited;
	splitedLine(lineFromServer, lineSplited);

	if (lineSplited[0] == "location" && !this->workingOnLocation)
	{
		tmpLocation.clearAllFields();
        tmpLocation.setRootCheck(false);
        server.setlocationFlag(true);
		if (lineSplited.size() != 3)
			throw Exception("Error At Location Start");
		if (lineSplited[1][0] != '/')
			throw Exception("Error Path Of Location Lust Begin With '/'");
		tmpLocation.setPathOfLocation(lineSplited[1]);
		if (lineSplited[2][0] != '{' || lineSplited[2].size() != 1)
			throw Exception ("Error The Location's Curly Braces { } Are Missing");
		this->workingOnLocation = true;
	}
	else if (this->workingOnLocation)
		collectDataInsideALocation(lineFromServer, server, tmpLocation);
}

void	supervisor::collectDataInsideALocation(std::string& lineFromServer, server& server, location& tmpLocation)
{
	std::vector<std::string> lineSplited;
	splitedLine(lineFromServer, lineSplited);
	std::map<std::string, int> enumMap;
	int			inputNumber;

	initEnumMap(enumMap);
	inputNumber = idenitfyInput(enumMap, lineSplited[0]);
	switch (inputNumber)
	{
		case 0:
			collectMethods(lineSplited, tmpLocation);
			break;
		case 1:
			collectRoot(lineSplited, tmpLocation);
			break;
		case 2:
			collectDefaultFile(lineSplited, tmpLocation);
			break;
		case 3:
			setCgi(lineSplited, tmpLocation);
			break;
		case 4:
			setAutoIndex(lineSplited, tmpLocation);
			break;
		case 5:
			collectUploadPath(lineSplited, tmpLocation);
			break;
		case 6:
			collectRedirectionLink(lineSplited, tmpLocation);
			break;
		case 7:
			endOfCollecting(lineSplited ,tmpLocation, server);
			break;
		default:
			throw Exception("Error: " + lineSplited[0] + " should not be present here");
	}
	lineSplited.clear();
}

void	location::clearAllFields()
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