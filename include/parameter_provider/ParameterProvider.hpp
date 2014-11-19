/*
 * Author: Arno Geurts
 *
 * Distributed under the MIT License
 * See the LICENSE file in the root of the repository
 */

#ifndef FUZE_PARAMETER_PROVIDER
#define	FUZE_PARAMETER_PROVIDER

#include <string>
#include "../Parameter.hpp"

namespace fuze {

    /**
     * Interface for the parameter providers of the dependency injection
     * container
     */
    class ParameterProvider 
    {
    public:
        /**
         * Check whether this parameter provider is able to provide the 
         * given parameter by its name
         */
        virtual bool hasParameter(const std::string&) = 0;

        /**
         * Get a reference to the parameter object, which can be casted to 
         * integer, floating point and string types
         */
        virtual const Parameter& getParameter(const std::string&) = 0;

        /**
         * Default virtual destructor, to make sure correct destruction
         * of the inherited classes
         */
        virtual ~ParameterProvider() = default;
    };
    
};


#endif	/* FUZE_PARAMETER_PROVIDER */

