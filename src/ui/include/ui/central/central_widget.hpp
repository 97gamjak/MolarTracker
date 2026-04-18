#ifndef __UI__INCLUDE__UI__CENTRAL__CENTRAL_WIDGET_HPP__
#define __UI__INCLUDE__UI__CENTRAL__CENTRAL_WIDGET_HPP__

#include <QStackedWidget>

namespace ui
{
    /**
     * @brief The central widget, this is responsible for displaying the main
     * content of the application in the central area of the main window, and
     * managing the different views that can be displayed in the central area
     *
     */
    class CentralWidget : public QStackedWidget
    {
        Q_OBJECT

       public:
        explicit CentralWidget(QWidget* parent);
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__CENTRAL__CENTRAL_WIDGET_HPP__s