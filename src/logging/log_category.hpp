#ifndef __LOGGING__LOG_CATEGORY_HPP__
#define __LOGGING__LOG_CATEGORY_HPP__

#include <string>

namespace logging
{
    /**
     * @brief helper struct to register log categories at during build step
     *
     */
    class LogCategory
    {
       private:
        std::string _name;

       public:
        explicit LogCategory(const char* name);

        [[nodiscard]] std::string getName() const;

        struct Hash
        {
            std::size_t operator()(const LogCategory& category) const
            {
                return std::hash<std::string>{}(category._name);
            }
        };
    };
}   // namespace logging

#endif   // __LOGGING__LOG_CATEGORY_HPP__