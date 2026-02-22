#include "i_store.hpp"

namespace app
{

    /**
     * @brief Mark the store as potentially dirty, this should be called after
     * making any changes to the store to indicate that there may be dirty data
     * that needs to be committed.
     */
    void IStore::_markPotentiallyDirty() { _isPotentiallyDirty = true; }

    /**
     * @brief Mark the store as clean, this should be called after committing
     * changes to the store to indicate that there is no longer any dirty data
     * that needs to be committed.
     */
    void IStore::_clearPotentiallyDirty() { _isPotentiallyDirty = false; }

    /**
     * @brief Check if the store has any potentially dirty data that needs to be
     * committed
     *
     * This method is used to have a quick check to see if the store has any
     * potentially dirty data that needs to be committed, without having to do a
     * more expensive check of the actual dirty data.
     *
     * @return true
     * @return false
     */
    [[nodiscard]] bool IStore::isPotentiallyDirty() const
    {
        return _isPotentiallyDirty;
    }

}   // namespace app