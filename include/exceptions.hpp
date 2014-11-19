/*
 * Author: Arno Geurts
 *
 * Distributed under the MIT License
 * See the LICENSE file in the root of the repository
 */

#ifndef FUZE_EXCEPTIONS
#define	FUZE_EXCEPTIONS

#include <stdexcept>
#include <string>

namespace fuze {
        
    /**
     * General exception type for exceptions in the fuze library
     */
    class FuzeException : public std::runtime_error
    {
    public:
        FuzeException(const std::string& message) 
        : std::runtime_error(message) 
        {}
    };

    /**
     * Exception thrown in the acquiring of a parameter
     */
    class ParameterException : public FuzeException
    {
    public:
        ParameterException(const std::string& message) 
        : FuzeException(message) 
        {}
    };
    
    /**
     * Exception thrown in the acquiring or instantiation of a service
     */
    class ServiceException : public FuzeException
    {
    public:
        ServiceException(const std::string& message) 
        : FuzeException(message) 
        {}
    };
};

#endif /* FUZE_EXCEPTIONS */

