#ifndef __EXCEPTIONS__BASE_HPP__
#define __EXCEPTIONS__BASE_HPP__

#include <exception>
#include <string>

/**
 * @brief Base class for all application exceptions
 */
class MolarTrackerException : public std::exception
{
   private:
    std::string _message;

   public:
    /**
     * @brief Constructs a new Molar Tracker Exception object
     *
     * @param message The exception message
     */
    explicit MolarTrackerException(const std::string& message);

    /**
     * @brief Returns the exception message
     *
     * @return const char* The exception message
     */
    const char* what() const noexcept override;
};

#endif   // __EXCEPTIONS__BASE_HPP__