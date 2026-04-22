#ifndef __UI__INCLUDE__UI__SETTINGS__SETTINGS_OVERVIEW_HPP__
#define __UI__INCLUDE__UI__SETTINGS__SETTINGS_OVERVIEW_HPP__

#include <QWidget>
#include <functional>

class QVBoxLayout;

namespace ui
{

    /**
     * @brief Initial overview page for settings, shows cards for each section
     * with title, description, and navigation button. Clicking a card navigates
     * to the corresponding section in the sidebar/stack.
     */
    class SettingsOverview : public QWidget
    {
       public:
        /// Type alias for the navigation callback function
        using OnNavigate = std::function<void(int stackIndex)>;

       private:
        /// Layout for the section cards
        QVBoxLayout* _cardsLayout{nullptr};

        /// Navigation callback function, called when a card is clicked with the
        /// stack index to navigate to
        OnNavigate _onNavigate;

       public:
        explicit SettingsOverview(const QString& title);

        void addCard(
            const QString& title,
            const QString& description,
            int            stackIndex
        );
        void setOnNavigate(OnNavigate callback);
    };

}   // namespace ui

#endif