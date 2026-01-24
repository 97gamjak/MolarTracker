#ifndef __UI__APPLICATION_HPP__
#define __UI__APPLICATION_HPP__

#include <QApplication>

namespace ui
{
    /**
     * @brief The main application class for Molar Tracker
     *
     * @note Don't missuse this class. It is a simple wrapper around
     * QApplication to setup application-wide settings like the application
     * name.
     */
    class MolarTrackerApplication : public QApplication
    {
       public:
        /**
         * @brief Constructs a new Molar Tracker Application
         *
         * @param argc Argument count
         * @param argv Argument values
         */
        MolarTrackerApplication(int& argc, char** argv);
    };

}   // namespace ui

#endif   // __UI__APPLICATION_HPP__