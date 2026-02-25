#include "string.hpp"

#include <ranges>
#include <string>
#include <vector>

namespace utils
{
    /**
     * @brief Join a vector of strings with a delimiter
     *
     * @param vector
     * @param delimiter
     * @return std::string
     */
    std::string join(
        const std::vector<std::string>& vector,
        const std::string&              delimiter
    )
    {
        auto joined = std::ranges::views::join_with(vector, delimiter);
        return {joined.begin(), joined.end()};
    }
}   // namespace utils