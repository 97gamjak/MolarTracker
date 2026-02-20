#ifndef __APP__STORE__I_STORE_HPP__
#define __APP__STORE__I_STORE_HPP__

namespace app
{

    /**
     * @brief Interface for a generic store
     *
     */
    class IStore
    {
       public:
        virtual ~IStore() = default;

        /**
         * @brief Commit the changes to the store, this should be called after
         * making any changes to the store to ensure that the changes are saved
         * and can be accessed by other parts of the application. The exact
         * behavior of this method will depend on the implementation of the
         * store, but it may involve saving the changes to a database, writing
         * them to a file, or simply marking them as committed in memory.
         *
         */
        virtual void commit() = 0;
    };

    /**
     * @brief State of an item in the store
     *
     */
    enum class StoreState
    {
        Clean,
        New,
        Modified,
        Deleted
    };

}   // namespace app

#endif   // __APP__STORE__I_STORE_HPP__