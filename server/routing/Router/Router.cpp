//
// Created by mathi on 24-Feb-21.
//

#include "ExceptionCore.hpp"
#include "Router.hpp"

#include <fstream>
#include <cstdio>
#include <sstream>
#include <Log.hpp>
#include <boost/filesystem.hpp>

#if WIN32
#define SLASH "\\"
#else
#define SLASH "/"
#endif

/**
 * \brief Initialises the /www folder
**/
void Router::init()
{
    if (!fs::exists("www") && !fs::create_directory("www"))
        throw ServerError("/www directory could not be created.", 500);
    this->_wwwPath = fs::path("www");
}

bool Router::initialized() const
{
    return !this->_wwwPath.empty();
}

/**
 * \brief concat a route with /www
 * \param route     The path relative to /www
**/
std::string Router::getPath(const std::string &route) const
{
    auto res = fs::path(this->_wwwPath);

    return res.concat(route).string();
}

/**
 * \brief Creates a route folder in the /www path
 * \param routePath     The path relative to /www
 * \param fromFolder    [Optional] Copy the folder content into the newly created route folder
**/
void Router::addRoute(const std::string &routePath, const std::string &fromFolder)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("The router has not been initialized.", 500);
    else if (!fs::exists(destination) && !fs::create_directories(destination))
        throw ServerError(destination + " directory could not be created.", 500);
    if (!fromFolder.empty() && fs::is_directory(fromFolder)) {
        for (auto &file : fs::directory_iterator(fromFolder))
            fs::copy(file, destination);
    }
}

/**
 * \brief Deletes the route folder from the /www path
 * \param routePath     The path relative to /www
**/
void Router::deleteRoute(const std::string &routePath)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("The router has not been initialized.", 500);
    else if (fs::is_directory(destination)) {
        if (!fs::remove(destination))
            throw ServerError("Cannot delete " + destination, 500);
    } else
        throw ClientError("Route " + routePath + " not found.", 404);
}

/**
 * \brief Deletes every files from /www relative path
 * \param routePath     The path relative to /www
 * \param fromFolder    [Optional] Should the call delete sub folders ?
**/
void Router::clearRoute(const std::string &routePath, bool clearFolders)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("The router has not been initialized.", 500);
    else if (fs::is_directory(destination)) {
        for (auto &entry : fs::directory_iterator(destination)) {
            if ((fs::is_directory(entry) && clearFolders) || !fs::is_directory(entry)) {
                if (!fs::remove(entry))
                    throw ServerError("Cannot delete " + entry.path().string(), 500);
            }
        }
    } else
        throw ClientError("Route " + routePath + " not found.", 404);
}

/**
 * \brief Creates a new file and write into it
 * \param routePath     The folder path relative to /www
 * \param filename      The name of the file to create
 * \param content       [Optional] Write content into the newly created file
 * \return pair : (string : path to file create / replaced) / (bool : true depending on whether the file has been replaced or not)
**/
std::pair<std::string, bool>
Router::create(const std::string &routePath, const std::string &filename, const std::string &content, bool replace)
{
    std::string destination = this->getPath(routePath);
    bool overwrite = false;

    if (!this->initialized())
        throw ServerError("The router has not been initialized.", 500);
    else if (fs::is_directory(destination)) {
        if (fs::exists(fs::path(destination + SLASH + filename)) && !replace)
            throw ServerError(fs::path(destination + SLASH + filename).string() + " already exists.", 500);
        if (fs::exists(fs::path(destination + SLASH + filename)) && replace)
            overwrite = true;
        std::ofstream ofs(fs::path(destination + SLASH + filename), std::ofstream::trunc);
        ofs << content;
        ofs.close();
        return std::make_pair(fs::path(destination + SLASH + filename).string(), overwrite);
    } else
        throw ClientError("Route " + routePath + " not found.", 404);
}

/**
 * \brief Gets a file content from route
 * \param routePath     The folder path relative to /www
 * \param filename      The file to get content from
**/
std::string Router::get(const std::string &routePath, const std::string &filename)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("The router has not been initialized.", 500);
    else if (fs::is_directory(destination)) {
        if (!fs::exists(fs::path(destination + SLASH + filename)))
            throw ClientError("File " + fs::path(destination + SLASH + filename).string() + " not found.", 404);
        std::ifstream file(fs::path(destination + SLASH + filename), std::ios::binary);
        std::ostringstream stream;
        stream << file.rdbuf();
        return stream.str();
    } else
        throw ClientError("Route " + routePath + " not found.", 404);
}

/**
 * \brief Deletes the specified file from the specified route
 * \param routePath     The path relative to /www
 * \param filename      The file name to delete
**/
void Router::remove(const std::string &routePath, const std::string &filename)
{
    std::string destination = this->getPath(routePath);

    if (!this->initialized())
        throw ServerError("The router has not been initialized.", 500);
    else if (fs::is_directory(destination)) {
        if (!fs::exists(fs::path(destination + SLASH + filename)))
            throw ClientError("File " + fs::path(destination + SLASH + filename).string() + " not found.", 404);
        if (!fs::remove(fs::path(destination + SLASH + filename)))
            throw ServerError("Cannot delete " + destination, 500);
    } else
        throw ClientError("Route " + routePath + " not found.", 404);
}

/**
 * \brief Return the extension of a file
 * \param file : file or path including the final file
 * \return extension (E.g. "png" for "example.test.png")
**/
std::string Router::getFileExtension(const std::string &file)
{
    return boost::filesystem::extension(file).erase(0, 1);
}
