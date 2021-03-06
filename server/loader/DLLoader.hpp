/*
** EPITECH PROJECT, 2020
** ZIA
** File description:
** Created by Cyprien
*/


#ifndef ZIA_DLLOADER_HPP
#define ZIA_DLLOADER_HPP

#include "ADLLoader.hpp"
#include "ExceptionsDLLoader.hpp"
#include <string>
#include <iostream>

#ifdef __unix__
    #include <dlfcn.h>
    #define DYNLIB(name) ("./build/lib/lib" + std::string(name) + std::string(".so")).c_str()
#elif defined(_WIN32) || defined(WIN32)
    #include <cstdio>
    #define DYNLIB(name) (std::string(name) + std::string(".dll")).c_str()
#endif

namespace ModuleLoader {

    template<typename T>
    class DLLoader : public ModuleLoader::ADLLoader {
    public:
        explicit DLLoader(const std::string &libName);

        ~DLLoader() override
        = default;

        typedef T *(*fct)();

        void open();

        T *getInstance() const;

        void close(T instance) const;

    private:
        void *_lib;
    };

    template<typename T>
    DLLoader<T>::DLLoader(const std::string &libName) : ADLLoader(libName), _lib(nullptr)
    {
        try {
            open();
        } catch (ModuleLoader::ModuleLoaderException &e) {
            LOG(ERR) << e.getComponent() << ": " << e.what();
        }
    }

    template<typename T>
    void DLLoader<T>::open()
    {
        #ifdef __unix__
            _lib = dlopen(_libName.c_str(), RTLD_LAZY);
            if (!_lib)
                LOG(ERR) << dlerror();
        #elif defined(_WIN32) || defined(WIN32)
            _lib = LoadLibrary(TEXT(_libName.c_str()));
        #endif
        if (!_lib)
            throw ModuleLoader::DynamicLibError("Invalid open lib: " + _libName);
    }

    template<typename T>
    T *DLLoader<T>::getInstance() const
    {
        if (!_lib)
            throw ModuleLoader::DynamicLibError("Lib Not loaded");

        #ifdef __unix__
            if (!dlsym(_lib, "newInstance"))
                throw ModuleLoader::DynamicLibError("Unable to load function \"newInstance\"");
            T *(*f)();
            *(void **) (&f) = dlsym(_lib, "newInstance");
            return ((*f)());
        #elif defined(_WIN32) || defined(WIN32)
        if (!static_cast<void *>(GetProcAddress(static_cast<HINSTANCE>(_lib), "newInstance")))
            throw ModuleLoader::DynamicLibError("Unable to load function \"newInstance\"");
        fct f = (fct) GetProcAddress(static_cast<HINSTANCE>(_lib), "newInstance");
        return ((f)());
        #endif
    }

    template<typename T>
    void DLLoader<T>::close(T instance) const
    {
        if (!_lib)
            return;
        #ifdef __unix__
            if (!dlsym(_lib, "destroyInstance"))
                throw std::exception();
            T (*f)(T);
            *(void **) (&f) = dlsym(_lib, "destroyInstance");
            ((*f)(instance));
            dlclose(_lib);
        #elif defined(_WIN32) || defined(WIN32)
            FreeLibrary(static_cast<HMODULE>(_lib));
        #endif
    }
}

#if defined(_WIN32) || defined(WIN32)
#undef _WIN32_WINNT
#endif

#endif //ZIA_DLLOADER_HPP
