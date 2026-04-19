#ifndef __UI__INCLUDE__UI__SETTINGS__SETTINGS_OVERVIEW_HPP__
#define __UI__INCLUDE__UI__SETTINGS__SETTINGS_OVERVIEW_HPP__

#include <QWidget>
#include <functional>

class QVBoxLayout;

namespace ui
{

    class SettingsOverview : public QWidget
    {
       public:
        using OnNavigate = std::function<void(int stackIndex)>;

       private:
        QVBoxLayout* _cardsLayout{nullptr};
        OnNavigate   _onNavigate;

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