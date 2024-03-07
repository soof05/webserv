#include "../supervisor/supervisor.hpp"

//this function check each line which data to fill outside location (port, host, ..... )

void	supervisor::collectDataOutSideALocation(std::string& lineFromServer, server& server, std::map<int, std::string> &tmpErrorPages)
{
	std::map<std::string, int>		enumMap;
	std::vector<std::string>		lineSplited;
	int								inputNumber;

	initEnumMapOutLocation(enumMap);
	//give us a vector of string for each line
	splitedLine(lineFromServer, lineSplited);
	//check some error like for this line.
	checkErrorsOutLocation(lineSplited);
	inputNumber = idenitfyInput(enumMap, lineSplited[0]);

	switch (inputNumber)
	{
        case 0:
            break ;
		case 1:
			collectPort(server, lineSplited[1]);
			break ;
		case 2:
			server.setServerName(lineSplited[1]);
			break ;
		case 3:
			collectHost(server, lineSplited[1]);
			break ;
		case 4:
			tmpErrorPages.insert(std::pair<int, std::string>(ConvertToInt(lineSplited[1]), lineSplited[2]));
			break ;
		case 5:
			server.setClientBodySize(ConvertToLong(lineSplited[1]));
			break ;
        case 6:
            server.setServerBeenClosed(true);
            break;
        default:
			throw Exception("Error " + lineSplited[0] + " should not be present here");
	}
}

void	supervisor::collectPort(server &server, std::string& port)
{
	server.setPort(ConvertToInt(port));
	server.setPortFlag(true);
}

void	supervisor::collectHost(server &server, std::string& host)
{
	server.setHost(host);
	server.sethostFlag(true);
}
