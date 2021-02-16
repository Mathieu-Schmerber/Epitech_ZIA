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
#include <vector>

#if WIN32
    #undef DELETE
#endif

#define RMCHAR(x, y) x.erase(std::remove(x.begin(), x.end(), y), x.end());

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

    /** === Request === **/

    class Request {
    public:
        explicit Request(const std::string &in);
        void setRequestType(const std::string &requestType);
        void setRequestPath(const std::string &requestPath);
        void setRequestVersion(std::string requestVersion);
        void setRequestHeader(const std::pair<std::string, std::string>& header);

        [[nodiscard]] Type getRequestType() const;
        [[nodiscard]] const std::string &getRequestPath() const;
        [[nodiscard]] std::vector<std::pair<std::string, std::string>> getRequestHeaders() const;

    private:
        Type _requestType;
        std::string _path;
        bool _correctVersion{};
        const std::string &_request;
        std::vector<std::pair<std::string, std::string>> _headerlist;
    };

    /** === Request Parser === **/

    class RequestParser {
    public:
        explicit RequestParser(const std::string &in);

        void parseData();
        std::unique_ptr<ZiaRequest::Request> &getRequest();

    private:
        void _parseRequestMethod(const std::string &out);
        void _parseRequestHeaders(const std::string &out);

        std::string _request;
        std::unique_ptr<ZiaRequest::Request> _toReturn;
    };
}
#endif //ZIA_REQUESTPARSER_HPP
