#ifndef __EXCEPTIONS__FILE_EXCEPTIONS_HPP__
#define __EXCEPTIONS__FILE_EXCEPTIONS_HPP__

#include "exceptions/base.hpp"

/**
 * @brief Exception thrown when a directory operation fails
 */
class DirException : public MolarTrackerException
{
   public:
    /**
     * @brief Constructs a new Dir Exception object
     * @param message The exception message
     */
    explicit DirException(const std::string& message);
};

#endif   // __EXCEPTIONS__FILE_EXCEPTIONS_HPP__