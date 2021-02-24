//
// Created by mathi on 24-Feb-21.
//

#ifndef ZIA_ROUTER_HPP
#define ZIA_ROUTER_HPP

#include <map>
#include <memory>
#include "Route/Route.hpp"

class Router
{
private:
    std::filesystem::path _wwwPath;
    std::map<std::string, std::shared_ptr<Route>> _routes;

    [[nodiscard]] std::string getPath(const std::string &route) const;

public:
    explicit Router() = default;
    ~Router() = default;

    std::shared_ptr<Route> addRoute(const std::string &routePath, const std::string &fromFolder = nullptr);
    [[nodiscard]] std::shared_ptr<Route> getRoute(const std::string &routePath);

    void init();

    [[nodiscard]] bool initialized() const;
};


#endif //ZIA_ROUTER_HPP
