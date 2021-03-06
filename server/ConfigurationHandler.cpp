/**
 * \file ConfigurationHandler.cpp
 * \brief Classes which parse json files for the modules and the server
 * \author Alexandre.M
**/

#include "ConfigurationHandler.hpp"

ConfigurationHandler::ConfigurationHandler() : _numberOfLoadedModules(0)
{
}

ConfigurationHandler::~ConfigurationHandler()
= default;

std::string ConfigurationHandler::readFile(const std::string &filepath)
{
    std::string file;
    std::string line;

    std::ifstream myfile(filepath);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            file += line + '\n';
        }
        myfile.close();
    } else {
        LOG(WARN) << "[ConfigurationHandler::readFile] Unable to open file " << filepath;
        return "";
    }
    return file;
}

void ConfigurationHandler::loadConfiguration(const std::string &filepath)
{
    std::string file = readFile(filepath);

    if (file.empty())
        return;

    _doc.Parse(file.c_str());
    loadModules();
}

void ConfigurationHandler::loadModules()
{
    _numberOfLoadedModules = 0;
    _modules.clear();
    if (!_doc.IsObject()) {
        LOG(WARN) << "Config file not well formatted.";
        return;
    }
    if (!_doc.HasMember("modules")) {
        LOG(WARN) << "No modules found.";
        return;
    }
    const rapidjson::Value &modules = _doc["modules"];
    for (rapidjson::SizeType i = 0; i < modules.Size(); i++) {
        if (modules[i].HasMember("id") && modules[i].HasMember("name")
            && modules[i].FindMember("id")->value.IsInt64()
            && modules[i].FindMember("name")->value.IsString()) {
            s_module module;
            LOG(INFO) << "Config loaded module: \"" << modules[i].FindMember("name")->value.GetString()
                      << "\" with id: "
                      << modules[i].FindMember("id")->value.GetInt64();
            module.id = modules[i].FindMember("id")->value.GetInt64();
            module.name = modules[i].FindMember("name")->value.GetString();
            _modules.emplace_back(module);
            _numberOfLoadedModules++;
        }
    }
    LOG(INFO) << "Config loaded " << _numberOfLoadedModules << " modules.";
}

std::vector<t_module> ConfigurationHandler::getLoadedModules()
{
    return _modules;
}

int ConfigurationHandler::getInt(const std::string &filepath, const std::string &varName)
{
    std::string file = readFile(filepath);
    if (file.empty())
        return 0;

    _docModule.Parse(file.c_str());
    if (_docModule.IsObject() && _docModule.HasMember(varName.c_str()) && _docModule[varName.c_str()].IsInt64()) {
        return _docModule[varName.c_str()].GetInt();
    } else {
        LOG(WARN) << "There was an error while loading a config file can't fetch var: \""
                  << varName << "\" in file: " << filepath << " | Set a default value.";
    }
    return 0;
}

std::string ConfigurationHandler::getString(const std::string &filepath, const std::string &varName)
{
    std::string file = readFile(filepath);
    if (file.empty())
        return "";

    _docModule.Parse(file.c_str());
    if (_docModule.IsObject() && _docModule.HasMember(varName.c_str()) && _docModule[varName.c_str()].IsString()) {
        return _docModule[varName.c_str()].GetString();
    } else {
        LOG(WARN) << "There was an error while loading a config file can't fetch var: \""
        << varName << "\" in file: " << filepath << " | Set a default value.";
    }
    return "";
}

