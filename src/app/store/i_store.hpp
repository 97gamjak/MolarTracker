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