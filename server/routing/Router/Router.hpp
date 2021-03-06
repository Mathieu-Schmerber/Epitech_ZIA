//
// Created by mathi on 24-Feb-21.
//

#ifndef ZIA_ROUTER_HPP
#define ZIA_ROUTER_HPP

#include <map>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

class Router
{
private:
    fs::path _wwwPath;

    [[nodiscard]] std::string getPath(const std::string &route) const;

public:
    explicit Router() = default;
    ~Router() = default;

    void addRoute(const std::string &routePath, const std::string &fromFolder = "");
    void deleteRoute(const std::string &routePath);
    void clearRoute(const std::string &routePath, bool clearFolders = false);

    std::pair<std::string, bool> create(const std::string &routePath, const std::string &filename, const std::string &content = "", bool replace=false);
    std::string get(const std::string &routePath, const std::string &filename);
    void remove(const std::string &routePath, const std::string &filename);

    void init();
    [[nodiscard]] bool initialized() const;

    static std::string getFileExtension(const std::string &file);
};


#endif //ZIA_ROUTER_HPP
