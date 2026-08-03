#ifndef __UI__INCLUDE__UI__UPDATE__UPDATE_AVAILABLE_DIALOG_HPP__
#define __UI__INCLUDE__UI__UPDATE__UPDATE_AVAILABLE_DIALOG_HPP__

#include "ui/base/dialog.hpp"

class QCheckBox;   // Forward declaration
class QWidget;     // Forward declaration

namespace common
{
    class SemVer;   // Forward declaration
}   // namespace common

namespace ui
{

    /**
     * @brief Dialog shown when a newer version of MolarTracker is available.
     *
     * Displays the available version, a link to the releases page, and a
     * "don't show again for this version" checkbox. The caller should check
     * isDismissedForVersion() after exec() returns to decide whether to
     * persist the dismissal.
     */
    class UpdateAvailableDialog : public Dialog
    {
        Q_OBJECT

       private:
        /// Checkbox for suppressing the dialog for the current version
        QCheckBox* _dismissCheckBox{nullptr};

       public:
        explicit UpdateAvailableDialog(
            const common::SemVer& latestVersion,
            QWidget*              parent = nullptr
        );

        [[nodiscard]] bool isDismissedForVersion() const;

       signals:
        /// Emitted when the user clicks "Download Update"
        void downloadRequested();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__UPDATE__UPDATE_AVAILABLE_DIALOG_HPP__
