#ifndef __UI__INCLUDE__UI__MENU_BAR__SETTINGS_MENU_HPP__
#define __UI__INCLUDE__UI__MENU_BAR__SETTINGS_MENU_HPP__

#include <QMetaType>
#include <QObject>

class QMenu;
class QMenuBar;
class QAction;

namespace ui
{
    /**
     * @brief Settings menu in the menu bar
     *
     */
    class SettingsMenu : public QObject
    {
        Q_OBJECT

       signals:
        /// Signal emitted when the user requests to open the preferences dialog
        void requestPreferences();
        /// Signal emitted when the user requests the restore-from-backup dialog
        void requestRestoreFromBackup();

       private:
        /// Pointer to the settings menu widget
        QMenu* _settingsMenu = nullptr;

        /// The preferences action in the settings menu
        QAction* _preferencesAction = nullptr;
        /// The restore-from-backup action in the settings menu
        QAction* _restoreBackupAction = nullptr;

       public:
        explicit SettingsMenu(QMenuBar& menuBar);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__MENU_BAR__SETTINGS_MENU_HPP__