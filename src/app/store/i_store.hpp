#ifndef __APP__STORE__I_STORE_HPP__
#define __APP__STORE__I_STORE_HPP__

namespace app
{

    class IStore
    {
       public:
        virtual ~IStore() = default;

        virtual void commit() = 0;
    };

}   // namespace app

#endif   // __APP__STORE__I_STORE_HPP__