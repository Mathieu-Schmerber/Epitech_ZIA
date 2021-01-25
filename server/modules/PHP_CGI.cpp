/*
** EPITECH PROJECT, 2020
** ZIA
** File description:
** Created by Cyprien
*/

#include "PHP_CGI.hpp"

#if defined(_WIN32) || defined(WIN32)
    #include <windows.h>
    #include <stdio.h>
#endif

extern "C" {
    #if defined(_WIN32) || defined(WIN32)
    __declspec(dllexport) 
    #endif
    IModule* newInstance()
    {
        return new PHP_CGI();
    }
}
