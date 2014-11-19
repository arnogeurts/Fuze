/*
 * Author: Arno Geurts
 *
 * Distributed under the MIT License
 * See the LICENSE file in the root of the repository
 */

#ifndef FUZE_SERVICEPROVIDER
#define	FUZE_SERVICEPROVIDER

#include "Container.hpp"

namespace fuze {

    /**
     * Forward declaration of the container
     */
    class Container;

    /**
     * Interface for all service providers
     */
    class ServiceProvider 
    {
    public:          
        /**
         * Default virtual destructor, for correct destruction of inherited
         * classes
         */
        virtual ~ServiceProvider() = default;

        /**
         * Register the services to the service container
         */
        virtual void registerServices(Container&) const = 0;
    };
    
};

#endif	/* FUZE_SERVICEPROVIDER */

