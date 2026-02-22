#include "param_container.hpp"

namespace settings
{
    /**
     * @brief Construct a new ParamContainer object
     *
     * @param key Unique identifier for the parameter container
     * @param title User-friendly title for the parameter container
     * @param description Description of the parameter container's purpose
     */
    ParamContainer::ParamContainer(
        std::string key,
        std::string title,
        std::string description
    )
        : _key(std::move(key)),
          _title(std::move(title)),
          _description(std::move(description))
    {
    }

    /**
     * @brief Get the unique key of the parameter container
     *
     * @return const std::string& The unique key
     */
    const std::string& ParamContainer::getKey() const { return _key; }

    /**
     * @brief Get the user-friendly title of the parameter container
     *
     * @return const std::string& The title
     */
    const std::string& ParamContainer::getTitle() const { return _title; }

    /**
     * @brief Get the description of the parameter container
     *
     * @return const std::string& The description
     */
    const std::string& ParamContainer::getDescription() const
    {
        return _description;
    }

}   // namespace settings