/*
** EPITECH PROJECT, 2020
** ZIA
** File description:
** Created by Cyprien
*/


#ifndef ZIA_DYNAMICLIBMANAGER_HPP
#define ZIA_DYNAMICLIBMANAGER_HPP

#include "DLLoader.hpp"
#include "ExceptionsDLLoader.hpp"
#include <vector>
#include <algorithm>
#include <memory>

namespace ModuleLoader {

    class DynamicLibManager {
    private:
        std::vector<ModuleLoader::ADLLoader *> _libs;

    public:
        template<typename T>
        void loadNewLib(const std::string& libName)
        {
            if (!libStocked(libName))
                _libs.push_back(new DLLoader<T>(libName));
            else
                throw DynamicLibAlreadyExist("module " + libName + " is already loaded.");
        }

        template<typename T>
        T *getInstance(std::string libName)
        {
            auto it = std::find_if(_libs.begin(), _libs.end(), [&](auto l) { return l->getLibName() == libName; });

            if (it != _libs.end()) {
                try {
                    return dynamic_cast<DLLoader<T> *>(*it)->getInstance();
                } catch (ModuleLoader::ModuleLoaderException &e) {
                    LOG(ERR) << e.getComponent() << ": " << e.what();
                    throw ModuleLoader::ModuleLoaderException("DynamicLibManager", "Unable to get instance for lib " + libName);
                }
            }
            throw ModuleLoader::ModuleLoaderException("DynamicLibManager", "Lib not registered");
        }

        bool libStocked(std::string libName)
        {
            auto it = std::find_if(_libs.begin(), _libs.end(), [&](auto l) { return l->getLibName() == libName; });

            return it != _libs.end();
        }
    };
}

#endif //ZIA_DYNAMICLIBMANAGER_HPP