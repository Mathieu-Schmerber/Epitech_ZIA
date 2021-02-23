/**
 * \file PHP_CGI.hpp
 * \brief PHP CGI module
 * \author Mathieu.S
**/

#ifndef ZIA_PHP_CGI_HPP
#define ZIA_PHP_CGI_HPP

#include "AModule.hpp"

class PHP_CGI : public AModule {
private:
    std::pair<std::string, int> _in;

    std::string handleRequest(const std::string &request);
    void handleQueue() override;

public:
    explicit PHP_CGI();
    void loadConfigFile(const std::string &configFilePath) override;
};

#endif //ZIA_PHP_CGI_HPP
