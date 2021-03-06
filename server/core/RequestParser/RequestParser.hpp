/**
 * \file RequestParser.hpp
 * \brief HTTP Request Parser
 * \author Emilien.D
**/

#ifndef ZIA_REQUESTPARSER_HPP
#define ZIA_REQUESTPARSER_HPP

#include "Log.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#if WIN32
    #undef DELETE
#endif

#define RMCHAR(x, y) x.erase(std::remove(x.begin(), x.end(), y), x.end());

/**
 * \namespace ZiaRequest
 * \brief Store all the classes and const variables for the HTTP Request parser
**/

namespace ZiaRequest {

    [[maybe_unused]] static const char *requestTypesNames[9] = {"GET", "POST", "OPTIONS",
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

    /**
     * \class Request RequestParser.hpp "RequestParser.hpp"
     * \brief Class which define the HTTP request
    **/

    class Request {
    public:
        explicit Request();
        void setRequestType(const std::string &requestType);
        void setRequestPath(const std::string &requestPath);
        void setRequestVersion(const std::string& requestVersion);
        void setRequestHeader(const std::pair<std::string, std::string>& header);
        void addBodyLine(const std::string &body);

        [[nodiscard]] Type getRequestType() const;
        [[nodiscard]] const std::string &getRequestPath() const;
        [[nodiscard]] std::vector<std::pair<std::string, std::string>> getRequestHeaders() const;
        [[nodiscard]] std::string getRequestBody() const;
    private:
        Type _requestType;
        std::string _path;
        bool _correctVersion{};
        std::vector<std::pair<std::string, std::string>> _headerlist;
        std::string _body;
    };

    /**
     * \class RequestParser RequestParser.hpp "RequestParser.hpp"
     * \brief Class which parse the HTTP request
    **/

    class RequestParser {
    public:
        explicit RequestParser();

        [[nodiscard]] ZiaRequest::Request parseData(const std::string &in);
    private:
        void _parseRequestMethod(const std::string& out, ZiaRequest::Request &request);
        void _parseRequestHeaders(const std::string &out, ZiaRequest::Request &request);
        void _parseRequestBody(const std::string &out, ZiaRequest::Request &request);

        std::string _request;
    };
}
#endif //ZIA_REQUESTPARSER_HPP
