#include "supervisor.hpp"

supervisor::supervisor(char *fileName, int numArgs)
{
    if (numArgs != 2)
        throw Exception ("Error To Many Arguments.");
    readAndParseConfigFile(fileName);
}

void    supervisor::beginOurWebServ()
{
    multiplexing		network;
    network.startingMultiplexing(this->servers);
}