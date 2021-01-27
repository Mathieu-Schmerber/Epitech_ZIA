//
// Created by alexa on 26/01/2021.
//

#include "ConfigurationHandler.hpp"

ConfigurationHandler::ConfigurationHandler() : _numberOfLoadedModules(0)
{

}

ConfigurationHandler::~ConfigurationHandler()
{

}

std::string ConfigurationHandler::readFile(const std::string &filepath)
{
    std::string file;
    std::string line;

    std::ifstream myfile("config.json");
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            file += line + '\n';
        }
        myfile.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
    return file;
}

void ConfigurationHandler::loadConfiguration(const std::string &filepath)
{
    std::string file = readFile(filepath);

    _doc.Parse(file.c_str());

    loadModules();
}

void ConfigurationHandler::loadModules()
{
    if (_doc.HasMember("modules") == 0) {
        LOG(WARN) << "No modules found.";
        return;
    }
    const rapidjson::Value &modules = _doc["modules"];
    for (rapidjson::SizeType i = 0; i < modules.Size(); i++) {
        if (modules[i].HasMember("id") && modules[i].HasMember("name")
        && modules[i].FindMember("id")->value.IsInt64()
        && modules[i].FindMember("name")->value.IsString()) {
            s_module module;
            LOG(INFO) << "Loaded module: \"" << modules[i].FindMember("name")->value.GetString() << "\" with id: "
            << modules[i].FindMember("id")->value.GetInt64();
            module.id = modules[i].FindMember("id")->value.GetInt64();
            module.name = modules[i].FindMember("name")->value.GetString();
            _modules.emplace_back(module);
            _numberOfLoadedModules++;
        }
    }
    LOG(INFO) << "Loaded: " << _numberOfLoadedModules << " modules.";
}

std::vector<t_module> ConfigurationHandler::getLoadedModules()
{
    return _modules;
}

