/*
 * Author: Arno Geurts
 *
 * Distributed under the MIT License
 * See the LICENSE file in the root of the repository
 */

#ifndef FUZE_PARAMETER
#define	FUZE_PARAMETER

namespace fuze {
        
    /**
     * A parameter container, for parameters in the dependency injection
     * container.
     * The parameter stores its value as an std::string, and  can be cast to 
     * different types
     */
    class Parameter
    {
    private:
        /**
         * The value of the parameter
         */
        std::string value;

    public:
        Parameter()
        : value("")
        {}
        
        /**
         * Construct the parameter with a value
         */
        Parameter(std::string value) 
        : value(std::move(value))
        {}

        /**
         * Default destructor
         */
        ~Parameter() = default;
        
        /**
         * Cast the parameter to an int
         * Try to convert the value string to an int using atoi
         */
        operator int() 
        {
            return atoi(value.c_str());
        }

        /**
         * Cast the parameter to a long int
         * Try to convert the value string to a long int using atol
         */
        operator long int()
        {
            return atol(value.c_str());
        }

        /**
         * Cast the parameter to an double
         * Try to convert the value string to an double using atof
         */
        operator double()
        {
            return atof(value.c_str());
        }

        /**
         * Cast the parameter to a const char*
         * Try to convert the value string to an const char* by applying the
         * c_str method to the value
         */
        operator const char*()
        {
            return value.c_str();
        }

        /**
         * Cast the parameter to an std::string
         * Create a copy of the value object
         */
        operator std::string() 
        {
            return value;
        }
    };
};

#endif	/* FUZE_PARAMETER */

