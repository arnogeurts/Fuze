/*
 * Author: Arno Geurts
 *
 * Distributed under the MIT License
 * See the LICENSE file in the root of the repository
 */

#ifndef FUZE_PARAMETER_PROVIDER
#define	FUZE_PARAMETER_PROVIDER

#include <cstdlib>
#include <string>
#include <algorithm>
#include "ParameterProvider.hpp"
#include "../Parameter.hpp"
#include "../Container.hpp"

namespace fuze {

    /**
     * Parameter providers, which tries to find a parameter in the environment
     * variables
     */
    class EnvParameterProvider : public ParameterProvider
    {
    public:
        /**
         * Check whether this parameter provider is able to provide the 
         * given parameter by its name
         */
        bool hasParameter(const std::string& name)
        {
            return getenv(getName().c_str()) != NULL;
        }

        /**
         * Get a reference to the parameter object, which can be casted to 
         * integer, floating point and string types
         */
        const Parameter& getParameter(const std::string& name)
        {
            const char* value = getenv(getName(name).c_str());
            if (value == NULL) {
                throw ParameterException("Unable to get unknown parameter '" + name + "'");
            }
            
            return Parameter(std::string(value));
        }
        
        /**
         * Get the environment variable name, by the given parameter name
         */
        const std::string& getName(const std::string& name) const noexcept
        {
            std::string envName;
            std::transform(name.begin(), name.end(), envName.begin(), ::toupper);
            
            return envName;
        }

        /**
         * Default virtual destructor
         */
        ~EnvParameterProvider() = default;
    };
    
    /**
     * Fuze DI service provider, to add the EnvParameterProvider to the container
     */
    class EnvParameterProviderServiceProvider : public ServiceProvider
    {
    public:
        /**
         * Default destructor
         */
        ~EnvParameterProviderServiceProvider() = default;
        
        /**
         * Register the services to the service container
         */
        void registerServices(Container& container) const
        {            
            // use default constructor and destructor
            // shared, persistent service.
            // tag as paramter provider
            container.registerService<ParameterProvider>(
                "fuze.parameter_provider.env"
            ).tag("fuze.parameter_provider");
        }
        
    };
};


#endif	/* FUZE_PARAMETER_PROVIDER */

