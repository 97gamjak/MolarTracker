#ifndef __EXCEPTIONS__INCLUDE__EXCEPTIONS__OUT_OF_RANGE_HPP__
#define __EXCEPTIONS__INCLUDE__EXCEPTIONS__OUT_OF_RANGE_HPP__

#include "base.hpp"

/**
 * @brief Exception thrown when an operation attempts to access an element
 * outside the valid range of a container or data structure.
 *
 */
class OutOfRange : public MolarTrackerException
{
   public:
    explicit OutOfRange(const std::string& message);
};

#endif   // __EXCEPTIONS__INCLUDE__EXCEPTIONS__OUT_OF_RANGE_HPP__