/**
 * \file IResponse.hpp
 * \brief File which defines the HTTP Response Interface
 * \author Emilien.D
**/

#ifndef ZIA_IRESPONSE_HPP
#define ZIA_IRESPONSE_HPP

/**
 * \class IResponse IResponse.hpp "IResponse.hpp"
 * \brief Interface for the HTTP Response
**/

class IResponse {
public:
    virtual ~IResponse() = 0;
};

#endif //ZIA_IRESPONSE_HPP
