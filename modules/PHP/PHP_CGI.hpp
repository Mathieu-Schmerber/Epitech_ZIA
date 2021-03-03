/**
 * \file PHP_CGI.hpp
 * \brief PHP CGI module
 * \author Mathieu.S
**/

#ifndef ZIA_PHP_CGI_HPP
#define ZIA_PHP_CGI_HPP

#include "AModule.hpp"

#define BUFFER_SIZE 256

class PHP_CGI : public AModuleOutput {
private:
    std::string _cgiPath;

    static std::string getOSCmd(const std::string &cmd);
    static FILE *openOSPipe(const std::string &cmd);
    static void closeOSPipe(FILE *pipe);
    static std::string execute(const std::string &cmd);
    std::string handleRequest(const std::string &request);
    void handleQueue() override;
    [[nodiscard]] std::string getFileExtension() const override { return ".php"; }

public:
    explicit PHP_CGI();
    void loadConfigFile(const std::string &configFilePath) override;
};

#endif //ZIA_PHP_CGI_HPP
