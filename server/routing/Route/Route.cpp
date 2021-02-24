//
// Created by mathi on 24-Feb-21.
//

#include <iostream>
#include "Route.hpp"

Route::Route(const std::string &path) : _path(path) {std::cout << "+ Route: " << path << std::endl;}

void Route::addFile(const std::string &source)
{

}

void Route::deleteFile(const std::string &file)
{

}

void Route::clearFiles()
{

}

std::vector<std::string> Route::get(const std::string &subFile) const {
    return std::vector<std::string>();
}

std::vector<std::string> Route::getAll(const std::string &subFolder) const {
    return std::vector<std::string>();
}
