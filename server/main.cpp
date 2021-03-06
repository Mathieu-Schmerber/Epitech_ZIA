/**
 * \file main.cpp
 * \brief file with the main function to start the server
 * \author Emilien.D
**/

#include "Server.hpp"

/**
 * \brief main, start the server
**/

int main(int ac, char **av)
{
    Server _server(ac, av);

    _server.run();
    return (0);
}
