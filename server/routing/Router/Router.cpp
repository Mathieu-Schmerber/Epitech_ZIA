//
// Created by mathi on 24-Feb-21.
//

#include <fstream>
#include <sstream>
#include "ExceptionCore.hpp"
#include "Router.hpp"

void Router::init()
{
    if (!fs::exists("www") && !fs::create_directory("www"))
        throw ServerError("Internal Server Error", "/www directory could not be created.");
    this->_wwwPath = fs::path("www");
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

void Router::addRoute(const std::string &routePath, const std::string &fromFolder)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("Internal Server Error", "The router has not been initialized.");
    else if (!fs::exists(destination) && !fs::create_directories(destination))
        throw ServerError("Internal Server Error", destination + " directory could not be created.");
    if (!fromFolder.empty() && fs::is_directory(fromFolder)) {
        for (auto &file : fs::directory_iterator(fromFolder))
            fs::copy(file, destination);
    }
}

void Router::deleteRoute(const std::string &routePath)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("Internal Server Error", "The router has not been initialized.");
    else if (fs::is_directory(destination)) {
        if (!fs::remove(destination))
            throw ServerError("Internal Server Error", "Cannot delete " + destination);
    } else
        throw ServerError("Not Found", "Route " + routePath + " not found.", 404);
}

void Router::clearRoute(const std::string &routePath, bool clearFolders)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("Internal Server Error", "The router has not been initialized.");
    else if (fs::is_directory(destination)) {
        for (auto &entry : fs::directory_iterator(destination)) {
            if ((fs::is_directory(entry) && clearFolders) || !fs::is_directory(entry)) {
                if (!fs::remove(entry))
                    throw ServerError("Internal Server Error", "Cannot delete " + entry.path().string());
            }
        }
    } else
        throw ServerError("Not Found", "Route " + routePath + " not found.", 404);
}

std::string Router::create(const std::string &routePath, const std::string &filename, const std::string &content)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("Internal Server Error", "The router has not been initialized.");
    else if (fs::is_directory(destination)) {
        if (fs::exists(fs::path(destination + "\\" + filename)))
            throw ServerError("Internal Server Error", fs::path(destination + "\\" + filename).string() + " already exists.");
        std::ofstream ofs(fs::path(destination + "\\" + filename));
        ofs << content;
        ofs.close();
        return fs::path(destination + "\\" + filename).string();
    } else
        throw ServerError("Not Found", "Route " + routePath + " not found.", 404);
}

std::string Router::get(const std::string &routePath, const std::string &filename)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("Internal Server Error", "The router has not been initialized.");
    else if (fs::is_directory(destination)) {
        if (!fs::exists(fs::path(destination + "\\" + filename)))
            throw ServerError("Not Found", "File " + fs::path(destination + "\\" + filename).string() + " not found.", 404);
        std::ifstream file(fs::path(destination + "\\" + filename));
        std::ostringstream stream;
        stream << file.rdbuf();
        return stream.str();
    } else
        throw ServerError("Not Found", "Route " + routePath + " not found.", 404);
}

void Router::remove(const std::string &routePath, const std::string &filename)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("Internal Server Error", "The router has not been initialized.");
    else if (fs::is_directory(destination)) {
        if (!fs::exists(fs::path(destination + "\\" + filename)))
            throw ServerError("Not Found", "File " + fs::path(destination + "\\" + filename).string() + " not found.", 404);
        if (!fs::remove(fs::path(destination + "\\" + filename)))
            throw ServerError("Internal Server Error", "Cannot delete " + destination);
    } else
        throw ServerError("Not Found", "Route " + routePath + " not found.", 404);
}