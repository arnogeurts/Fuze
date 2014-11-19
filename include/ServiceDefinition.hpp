/*
 * Author: Arno Geurts
 *
 * Distributed under the MIT License
 * See the LICENSE file in the root of the repository
 */

#ifndef FUZE_SERVICEDEFINITION_HPP
#define	FUZE_SERVICEDEFINITION_HPP

#include <typeinfo>
#include <memory>
#include <functional>
#include <cassert>
#include <iostream>

namespace fuze {

    /**
     * Forward declaration of the Container class
     */
    class Container;

    /**
     * The base class for the service definitions
     */
    class BaseServiceDefinition
    {
    private:
        /**
         * List of tags of this service definition
         */
        std::vector<std::string> tags;

    public:            
        /**
         * Virtual destructor to make sure the override class is also
         * destructed correctly
         */
        virtual ~BaseServiceDefinition() = default;

        /**
         * Get the type info of the service this holder is holding
         * 
         * @return const std::type_info&
         */
        virtual const std::type_info& getTypeInfo() const = 0;

        /**
         * Tag the service definition with the given string
         */
        void tag(std::string tag) 
        {
            tags.push_back(std::move(tag));
        }

        /**
         * Whether the service definition is tagged with the given tag
         */
        bool hasTag(const std::string& tag) 
        {                
            bool result = std::find(tags.begin(), tags.end(), tag) != tags.end();

            return result;
        }
    };

    /**
     * The specialized class for the service definitions
     */
    template<class T>
    class ServiceDefinition : public BaseServiceDefinition
    {
    public:
        typedef std::function<T*(Container&)> Construct;
        typedef std::function<void(T*)> Destruct;

    private:
        /**
         * The instance of the service
         */
        T* sharedInstance = nullptr;

        /**
         * Reference counter for the non-persistent instance
         */
        int referenceCounter = 0;

        /**
         * Whether this service is shared
         */
        bool shared = true;

        /**
         * Whether this service is destructed after all references are 
         * deleted
         */
        bool persistent = true;

        /**
         * Constructor for the service definition
         */
        Construct constructor = [] (Container&) { return new T(); };

        /**
         * Destructor for the service definition, default initiated with 
         * a delete
         */
        Destruct destructor = [] (T* service) { delete service; };

        /**
         * Destructor when the shared_ptr is deleted
         * @param service
         */
        void destructShared(T* service) 
        {
            if (!persistent) {
                if (referenceCounter <= 1) {
                    destructor(service);
                    sharedInstance = nullptr;
                } else {
                    referenceCounter--;
                }
            }
        }

    public:
        /**
         * Construct service definition, use default constructor and  destructor
         */
        ServiceDefinition() noexcept
        {}
        
        /**
         * Construct service definition with only service constructor,
         * use default destructor
         */
        ServiceDefinition(Construct constructor) noexcept
        : constructor(std::move(constructor))
        {}

        /**
         * Construct service definition with service constructor and
         * destructor
         */
        ServiceDefinition(Construct constructor, Destruct destructor) noexcept
        : constructor(std::move(constructor)), destructor(std::move(destructor))
        {}

        /**
         * Destructor, deletes the shared instance
         */
        ~ServiceDefinition()
        {
            delete sharedInstance;
        }

        /** 
         * @override
         * @return const std::type_info&
         */
        const std::type_info& getTypeInfo() const noexcept
        {
            return typeid(T);
        }

        /**
         * Set whether this service is shared
         * 
         * @param sh
         */
        ServiceDefinition& setShared(bool sh) noexcept
        {
            shared = sh;
            return *this;
        }

        /**
         * Set whether 
         * @param p
         */
        ServiceDefinition& setPersistent(bool p) noexcept
        {
            persistent = p;
            return *this;
        }

        /**
         * 
         * @param container
         * @return 
         */
        std::shared_ptr<T> get(Container& container)
        {
            // declare return variable
            std::shared_ptr<T> ptr;
            if (!shared) {
                ptr = std::shared_ptr<T>(constructor(container), destructor);
            } else {
                // construct the 
                if (!sharedInstance) {
                    sharedInstance = constructor(container);
                }

                if (!persistent) {
                    referenceCounter++;
                }

                ptr = std::shared_ptr<T>(sharedInstance, [this] (T* service) {
                    this->destructShared(service);
                });
            }

            return ptr;
        }
    };
};

#include "Container.hpp"

#endif	/* DEX_DI_SERVICEDEFINITION_HPP */

