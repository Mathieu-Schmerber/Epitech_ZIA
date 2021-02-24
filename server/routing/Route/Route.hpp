//
// Created by mathi on 24-Feb-21.
//

#ifndef ZIA_ROUTE_HPP
#define ZIA_ROUTE_HPP

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class Route
{
private:
    const std::string _path;

public:
    explicit Route(const std::string &path);

    void addFile(const std::string &source);
    void deleteFile(const std::string &file);
    void clearFiles();

    [[nodiscard]] std::vector<std::string> get(const std::string &subFile) const;
    [[nodiscard]] std::vector<std::string> getAll(const std::string &subFolder = nullptr) const;
};

#endif //ZIA_ROUTE_HPP
