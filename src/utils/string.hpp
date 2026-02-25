#ifndef __UTILS__STRING_HPP__
#define __UTILS__STRING_HPP__

#include <string>
#include <vector>

namespace utils
{
    std::string join(
        const std::vector<std::string>& vector,
        const std::string&              delimiter
    );

}   // namespace utils

#endif   // __UTILS__STRING_HPP__