#ifndef __UI__INCLUDE__UI__UTILS__ERROR_HPP__
#define __UI__INCLUDE__UI__UTILS__ERROR_HPP__

#include "ui/base/dialog.hpp"

class QString;   // forward declaration

namespace ui
{
    /**
     * @brief A dialog for displaying error messages.
     */
    class ErrorDialog : public Dialog
    {
        Q_OBJECT

       public:
        explicit ErrorDialog(
            const QString& title,
            const QString& message,
            QWidget*       parent = nullptr
        );

        static void show(
            const QString& title,
            const QString& message,
            QWidget*       parent = nullptr
        );

        static void show(
            const std::string& title,
            const std::string& message,
            QWidget*           parent = nullptr
        );

        static void show(const QString& message, QWidget* parent = nullptr);
        static void show(const std::string& message, QWidget* parent = nullptr);
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__UTILS__ERROR_HPP__