//
// Created by Cyprien on 1/25/2021.
//

#ifndef ZIA_IMODULE_HPP
#define ZIA_IMODULE_HPP

#include <string>

class IModule {
public:
    virtual void loadConfigFile(const std::string &configFilePath) = 0;
    virtual void dataInput(const std::string &) = 0;
    virtual std::string dataOutput() = 0;
    virtual bool isInputData() = 0;
    virtual void run() = 0;
};

#endif //ZIA_IMODULE_HPP
