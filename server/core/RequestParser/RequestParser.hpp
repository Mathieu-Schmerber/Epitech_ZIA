/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#ifndef ZIA_REQUESTPARSER_HPP
#define ZIA_REQUESTPARSER_HPP

#include <memory>
#include <string>

#if WIN32
    #undef DELETE
#endif

namespace ZiaRequest {

    static const char *requestTypesNames[9] = {"GET", "POST", "OPTIONS",
        "HEAD", "PUT", "DELETE", "TRACE", "CONNECT", "UNDEFINED"};

    enum Type {
        GET = 0,
        POST,
        OPTIONS,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        CONNECT,
        UNDEFINED
    };

    class Request {
    public:
        explicit Request(const std::string &in);
        void setRequestType(const std::string &requestType);
        void setRequestPath(const std::string &requestPath);
        void setRequestVersion(const std::string &requestVersion);

        [[nodiscard]] Type getRequestType() const;
        [[nodiscard]] const std::string &getRequestPath() const;

    private:
        Type _requestType;
        std::string _path;
        bool _correctVersion;
        const std::string &_request;
    };

    class RequestParser {
    public:
        explicit RequestParser(const std::string &in);

        std::unique_ptr<Request> parseData();

    private:
        const std::string &_request;
    };
}
#endif //ZIA_REQUESTPARSER_HPP
