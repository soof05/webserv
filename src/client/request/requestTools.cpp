#include "request.hpp"

void	request::convertHex(std::string& path)
{
	std::string	hexValue;
	size_t	found;
	int 	hex;
	std::string	result, tmp;

	// std::cout << path << std::endl;
	found = path.find("%");
	if (path[found + 1] == '%')
		throw Exception("400");
	if (found == std::string::npos)
		return ;
	while (found != std::string::npos)
	{
		hexValue = path.substr(found + 1, 2);
		std::istringstream	hexStream(hexValue);
		hexStream >> std::hex >> hex;
		tmp = path.substr(0, found);
		if (hex < 127)
		{
			result += tmp;
			result += static_cast<char>(hex);
		}
		else
		{
			hexValue = "%" + hexValue;
			result += hexValue;
		}
		path.erase(0, found + 3);
		tmp = path;
		found = path.find("%");
		if (path[found + 1] == '%')
			throw Exception("400");
	}
	// std::cout << result << std::endl;
	path = result;
	path = result + tmp;
}

bool	request::compareLength(location& a, location& b)
{
	return (a.getPathOfLocation().length() > b.getPathOfLocation().length());
}

void	request::sortLocationsVector(std::vector<location>& locations)
{
	std::sort(locations.begin(), locations.end(), compareLength);
	// std::vector<location>::iterator iter = this->server.getLocations().begin();
	// 	for (; iter != this->server.getLocations().end(); iter++)
	// {
	// 	std::cout << iter->getPathOfLocation() << std::endl;
	// }
}