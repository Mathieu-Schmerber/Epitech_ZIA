//
// Created by Emilien on 17/02/2021.
//

#ifndef ZIA_HTTPMODULE_HPP
#define ZIA_HTTPMODULE_HPP

#include "IModule.hpp"

class HTTPModule : public IModule {
public:
    explicit HTTPModule() = default;

    void loadConfigFile(const std::string &configFilePath) override;
    void dataInput(const std::string &input) override;
    std::string dataOutput() override;
    bool isInputData() override;
private:

};

#endif //ZIA_HTTPMODULE_HPP
