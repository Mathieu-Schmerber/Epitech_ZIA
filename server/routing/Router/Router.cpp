//
// Created by mathi on 24-Feb-21.
//

#include "ExceptionCore.hpp"
#include "Router.hpp"
#include "Utils.hpp"

std::shared_ptr<Route> Router::addRoute(const std::string &routePath, const std::string &fromFolder)
{
    if (!this->initialized()) {
        throw ServerError("Internal Server Error", "The router has not been initialized.");
    } else if (!Utils::isInMap(this->_routes, routePath)) {
        std::string destination = this->getPath(routePath);

        if (!fs::exists(this->getPath(routePath)) && !fs::create_directory(destination))
            throw ServerError("Internal Server Error", destination + " directory could not be created.");
        this->_routes.insert({routePath, std::make_shared<Route>(destination)});
        if (!fromFolder.empty() && fs::is_directory(fromFolder)) {
            for (auto &file : fs::directory_iterator(fromFolder))
                fs::copy(file, destination);
        }
    }
    return this->_routes[routePath];
}

std::shared_ptr<Route> Router::getRoute(const std::string &routePath)
{
    if (!this->initialized())
        throw ServerError("Server Error 500", "The router has not been initialized.", 500);
    else if (Utils::isInMap(this->_routes, routePath))
        return this->_routes[routePath];
    return nullptr;
}

void Router::init()
{
    std::cout << "Initializing Router" << std::endl;
    if (!fs::exists("www") && !fs::create_directory("www"))
        throw ServerError("Internal Server Error", "/www directory could not be created.");
    this->_wwwPath = fs::path("www");
    for (auto &folder : fs::directory_iterator(this->_wwwPath)) {
        if (fs::is_directory(folder)) {
            this->addRoute("/" + folder.path().filename().string());
            std::cout << "\t+  " << "/" + folder.path().filename().string() << std::endl;
        }
    }
}

bool Router::initialized() const
{
    return !this->_wwwPath.empty();
}

std::string Router::getPath(const std::string &route) const
{
    auto res = fs::path(this->_wwwPath);

    return res.concat(route).string();
}
