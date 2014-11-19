/*
 * Author: Arno Geurts
 *
 * Distributed under the MIT License
 * See the LICENSE file in the root of the repository
 */

#ifndef FUZE_CONTAINER
#define	FUZE_CONTAINER

#include <map>
#include <utility>
#include <string>
#include <vector>
#include "Parameter.hpp"
#include "parameter_provider/ParameterProvider.hpp"
#include "ServiceDefinition.hpp"
#include "ServiceProvider.hpp"
#include "exceptions.hpp"

namespace fuze {
        
    /**
     * The main dependency injection container, containing the service 
     * definitions from which the services can be loaded
     */
    class Container
    {
    public:
        typedef std::vector<ServiceProvider*> ServiceProviderList;
        typedef std::vector<std::shared_ptr<ParameterProvider>> ParameterProviderList;

    private:
        /**
         * Container for all service definitions
         */
        std::map<std::string, BaseServiceDefinition*> definitions;

        /**
         * Whether the parameter provider list is instantiated
         */
        bool parameterProvidersLoaded = false;

        /**
         * List of parameter providers
         */
        ParameterProviderList parameterProviders;

    public:

        Container() = default;

        /**
         * Construct the container with a list of service providers,
         * which will be registered 
         */
        Container(const ServiceProviderList& list)
        {
            for (auto provider : list) {
                provider->registerServices(*this);
            }
        }

        /**
         * Delete all the service definitions
         */
        ~Container()
        {
            for (auto def : definitions) {
                delete def.second;
            }
        }

        /**
         * Register service by name and use the default constructor and
         * destructor
         */
        template<class T>
        ServiceDefinition<T>& registerService(std::string name)
        {
            auto def = new ServiceDefinition<T>();
            definitions[std::move(name)] = def;

            return *def;
        }
        
        /**
         * Register service by name, with constructor and use the default
         * destructor
         */
        template<class T>
        ServiceDefinition<T>& registerService(std::string name, typename ServiceDefinition<T>::Construct constructor)
        {
            auto def = new ServiceDefinition<T>(std::move(constructor));
            definitions[std::move(name)] = def;

            return *def;
        }

        /**
         * Register service by name, with constructor and destructor
         */
        template<class T>
        ServiceDefinition<T>& registerService(std::string name, typename ServiceDefinition<T>::Construct constructor, typename ServiceDefinition<T>::Destruct destructor)
        {
            auto def = new ServiceDefinition<T>(std::move(constructor), std::move(destructor));
            definitions[std::move(name)] = def;

            return *def;
        }

        /**
         * Get the service by its name
         */
        template<class T>
        std::shared_ptr<T> get(const std::string& name)
        {
            return getServiceDefinition<T>(name)->get(*this);
        }

        /**
         * Get all service definitions tagged with the given name
         */
        template<class T>
        std::vector<std::shared_ptr<T>> tag(const std::string& tag)
        {
            std::vector<std::shared_ptr<T>> result;
            for (auto def : definitions) {
                if (def.second->hasTag(tag)) {
                    result.push_back(get<T>(def.first));
                }
            }

            return result;
        }

        /**
         * Get a parameter by its name
         */
        const Parameter& getParameter(const std::string& name)
        {
            std::shared_ptr<ParameterProvider> p = NULL; 

            if (!parameterProvidersLoaded) {
                // load the parameter providers, using tag system
                parameterProviders = tag<ParameterProvider>("di.parameter_provider");
                parameterProvidersLoaded = true;
            }

            for (auto provider : parameterProviders) {
                if (provider->hasParameter(name)) {
                    p = provider;
                    break;
                }
            }

            if (!p) {
                throw ParameterException("Unable to find parameter");
            }

            return p->getParameter(name);
        }

    private:
        /**
         * Get the service definition of the service registered by the given
         * name
         */
        template<typename T>
        ServiceDefinition<T>* getServiceDefinition(const std::string& name) 
        {
            auto it = definitions.find(name);
            if (it == definitions.end()) {
                throw ServiceException("Trying to get unknown service '" + name + "'");
            }

            if (it->second->getTypeInfo() != typeid(T)) {
                throw ServiceException("Invalid type supplied for service '" + name + "'");
            }

            return static_cast<ServiceDefinition<T>*>(it->second);
        }
    };
    
};

#endif	/* FUZE_CONTAINER */

