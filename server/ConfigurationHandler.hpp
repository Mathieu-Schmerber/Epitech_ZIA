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
#include <unordered_map>
#include "document.h"
#include "Log.hpp"

/**
 * \struct t_module
 * \brief Information about module
**/

struct t_module {
    int64_t id = -1;
    std::string name;
} typedef s_module;

/**
 * \class ConfigurationHandler
 * \brief Classes which parse json files for the modules and the server
**/

class ConfigurationHandler
{
    public:
        ConfigurationHandler();
        ~ConfigurationHandler();


/**
 * \fn std::string readFile(const std::string &filepath)
 * \brief Function that return the content of the file
 * \param [in] filepath The filepath of the JSON file to parse
 * \return The content of the file
**/
        std::string readFile(const std::string &filepath);
/**
 * \fn void loadConfiguration(const std::string &filepath)
 * \brief Function that load the configuration of the server
 * \param [in] filepath The filepath of the config file(JSON) to parse
**/
        void loadConfiguration(const std::string &filepath);
        int loadHttpModule(const std::string &filepath);
        int loadHttpsModule(const std::string &filepath);
/**
 * \fn std::vector<t_module> getLoadedModules()
 * \brief Function that return the modules loaded by the configuration file
 * \return The modules to load
**/
        std::vector<t_module> getLoadedModules();
/**
 * \fn int getInt(const std::string &filepath, const std::string& varName)
 * \brief Function that return an int from a config file(JSON)
 * \param [in] filepath The filepath of the config file
 * \param [in] varName The name of the variable to fetch
 * \return The int of the config file
**/
        int getInt(const std::string &filepath, const std::string& varName);
/**
 * \fn std::string getString(const std::string &filepath, const std::string& varName)
 * \brief Function that return an string from a config file(JSON)
 * \param [in] filepath The filepath of the config file
 * \param [in] varName The name of the variable to fetch
 * \return The string of the config file
**/
        std::string getString(const std::string &filepath, const std::string& varName);


    private:
        void loadModules();

        std::vector<t_module> _modules;
        int _numberOfLoadedModules;
        rapidjson::Document _doc;
        rapidjson::Document _docModule;
        rapidjson::Document _docHttp;

};


#endif //ZIA_CONFIGURATIONHANDLER_HPP
