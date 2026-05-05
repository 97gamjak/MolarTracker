#ifndef __EXCEPTIONS__INCLUDE__EXCEPTIONS__NOT_YET_IMPLEMENTED_HPP__
#define __EXCEPTIONS__INCLUDE__EXCEPTIONS__NOT_YET_IMPLEMENTED_HPP__

#include "base.hpp"

/**
 * @brief Exception thrown when a feature is not yet implemented.
 *
 */
class NotYetImplementedException : public MolarTrackerException
{
   public:
    explicit NotYetImplementedException(const std::string& message);
};

#endif   // __EXCEPTIONS__INCLUDE__EXCEPTIONS__NOT_YET_IMPLEMENTED_HPP__