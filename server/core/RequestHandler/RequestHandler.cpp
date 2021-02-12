/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#include "RequestHandler.hpp"
#include <iostream>

RequestHandler::RequestHandler(int id) : _thread(&RequestHandler::run, this), _running(true), _id(id)
{}

void RequestHandler::run()
{
    std::cout << "Thread " << _id << " started." << std::endl;
    while (_running);
    std::cout << "Thread " << _id << " stopped." << std::endl;
}

RequestHandler::~RequestHandler()
{
    _running = false;
    _thread.join();
}