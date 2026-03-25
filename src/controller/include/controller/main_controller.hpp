#ifndef __CONTROLLER__MAIN_CONTROLLER_HPP__
#define __CONTROLLER__MAIN_CONTROLLER_HPP__

#include <memory>

namespace controller
{
    class MainController
    {
       private:
        struct Impl;
        std::unique_ptr<Impl> _impl;

       public:
        explicit MainController();
        ~MainController();

        void start();
    };

}   // namespace controller

#endif   // __CONTROLLER__MAIN_CONTROLLER_HPP__