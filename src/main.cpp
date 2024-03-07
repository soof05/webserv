#include "./supervisor/supervisor.hpp"

int main(int ac, char *av[], char *env[])
{
    (void)env;
    try{
        supervisor supervisor(av[1], ac);
        // supervisor.getServersData();
        supervisor.beginOurWebServ();
    }
    catch (std::exception &exception) {
        std::cout << exception.what() << std::endl;
    }
}
