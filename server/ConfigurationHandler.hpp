/**
 * \file ConfigurationHandler.hpp
 * \brief Classes which parse json files for the modules and the server
 * \author Alexandre.M
**/

#ifndef ZIA_CONFIGURATIONHANDLER_HPP
#define ZIA_CONFIGURATIONHANDLER_HPP

#ifdef _WIN32

#include <windows.h>

#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "document.h"
#include "Log.hpp"

struct t_module {
    int64_t id = -1;
    std::string name;
} typedef s_module;

class ConfigurationHandler
{
    public:
        ConfigurationHandler();
        ~ConfigurationHandler();

        std::string readFile(const std::string &filepath);
        void loadConfiguration(const std::string &filepath);
        std::vector<t_module> getLoadedModules();

    private:
        void loadModules();

        std::vector<t_module> _modules;
        int _numberOfLoadedModules;
        rapidjson::Document _doc;

};


#endif //ZIA_CONFIGURATIONHANDLER_HPP
