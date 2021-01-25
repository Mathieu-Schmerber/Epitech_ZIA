/*
** EPITECH PROJECT, 2020
** ZIA
** File description:
** Created by Cyprien
*/


#ifndef ZIA_ADLLOADER_HPP
#define ZIA_ADLLOADER_HPP

namespace Engine {

    class ADLLoader {
    protected:
        std::string _libName;

    public:
        ADLLoader(std::string libName) : _libName(libName)
        {}

        virtual ~ADLLoader() = default;

        [[nodiscard]] std::string getLibName() const
        { return _libName; };
    };
}


#endif //ZIA_ADLLOADER_HPP
